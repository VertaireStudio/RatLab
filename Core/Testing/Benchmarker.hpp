/************************************/
/*         benchmarker.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "console.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// The Benchmarker type: A self-contained, dependency-free benchmark framework for RatLab.
// Benchmark bodies are registered at runtime (or at static-initialization time) and are executed
// on demand by 'run', either from 'main' or from any other runtime entry point.
// The iteration count of every benchmark is calibrated automatically until the requested
// measurement time is reached, which keeps the reported timings stable across machines.
class Benchmarker {
    public:
    // The signature every registered benchmark body has to follow.
    // The body is expected to perform its work 'p_iterations' times and to keep the
    // compiler from optimizing the work away (see 'do_not_optimize').
    typedef void (*BenchBody)(unsigned long long p_iterations);
    /*---------------------------------------------------------------------------------------*/

    // The measured outcome of a single benchmark.
    struct BenchResult {
        // The name of the benchmark, shown in the report. (Must outlive the registration.)
        const char *name;
        // The number of iterations which were performed per sample.
        unsigned long long iterations;
        // The number of samples which were taken to get a stable measurement.
        unsigned long long samples;
        // The fastest measured sample, in nanoseconds.
        double total_ns;
        // The time spent per iteration, in nanoseconds.
        double ns_per_iteration;
        // The amount of iterations which fit into a single second.
        double iterations_per_second;
    };

    private:
    // A single registered benchmark, waiting to be executed at runtime.
    struct BenchEntry {
        // The name of the benchmark, shown in the report. (Must outlive the registration.)
        const char *name;
        // The body which performs the actual work.
        BenchBody body;
        // The minimum amount of time (in milliseconds) a single sample has to take.
        double min_time_ms;
        // The minimum amount of iterations a single sample has to perform.
        unsigned long long min_iterations;
    };

    // The shared registry of every benchmark known to the workspace.
    // A function-local static is used so that registration is safe to perform from any
    // translation unit, no matter in which order the static-initializers happen to run.
    static std::vector<BenchEntry> &registry() {
        static std::vector<BenchEntry> entries;
        return entries;
    }

    // The compiled results of the run which is currently in progress.
    static std::vector<BenchResult> &results() {
        static std::vector<BenchResult> compiled;
        return compiled;
    }

    // The shared output plumbing.
    Console console;

    // The minimum amount of time (in milliseconds) a single sample has to take by default.
    static constexpr const double DEFAULT_MIN_TIME_MS = 25.0;
    // The hard upper limit for the calibrated iteration count, guarding against pathological bodies.
    static constexpr const unsigned long long MAX_ITERATIONS = 1ull << 32;
    // The amount of samples taken per benchmark; the fastest one is reported.
    static constexpr const unsigned long long DEFAULT_SAMPLES = 3ull;
    // The width of the report, used to draw the rules and to align the columns.
    static constexpr const std::size_t REPORT_WIDTH = 74;
    // The longest name which is allowed to widen the name column.
    static constexpr const std::size_t MAX_NAME_WIDTH = 34;
    /*-------------------------------------------------------------------------------*/

    // ── Measurement ─────────────────────────────────────────────────────────────────────────

    // Returns the current value of the monotonic clock, in nanoseconds.
    static double now_ns() {
        return static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count());
    }

    // Prevents the compiler from reordering or merging work across this point.
    // NOTE: Not 'func' - inline assembly is a runtime-only operation.
    static void clobber_memory() {
#if defined(GNUC_ENABLED) || defined(CLANG_ENABLED)
        asm volatile("" : : : "memory");
#else
        std::atomic_signal_fence(std::memory_order_seq_cst);
#endif
    }

    // Executes the given body for the given amount of iterations and returns the elapsed
    // time in nanoseconds.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    static double measure(const BenchEntry &p_entry, const unsigned long long p_iterations) {
        clobber_memory();
        const double start = now_ns();
        p_entry.body(p_iterations);
        clobber_memory();
        return now_ns() - start;
    }

    // Grows the iteration count until a single sample takes at least the requested time.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    static unsigned long long calibrate(const BenchEntry &p_entry) {
        unsigned long long iterations = p_entry.min_iterations < 1ull ? 1ull : p_entry.min_iterations;
        double elapsed_ns = measure(p_entry, iterations);

        // Warm up once more, so that caches, branch predictors and the CPU frequency settle.
        measure(p_entry, iterations);

        while (elapsed_ns < p_entry.min_time_ms * 1000000.0 && iterations < MAX_ITERATIONS) {
            iterations = iterations < 1024ull ? iterations * 2ull : iterations + (iterations / 2ull);
            elapsed_ns = measure(p_entry, iterations);
            if (iterations >= MAX_ITERATIONS) {
                break;
            }
        }

        return iterations;
    }

    // Runs a single benchmark to completion and returns its result.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    static BenchResult run_entry(const BenchEntry &p_entry, const unsigned long long p_samples) {
        const unsigned long long iterations = calibrate(p_entry);

        BenchResult result;
        result.name = p_entry.name;
        result.iterations = iterations;
        result.samples = p_samples;
        result.total_ns = measure(p_entry, iterations);
        for (unsigned long long sample = 1; sample < p_samples; ++sample) {
            // The fastest sample is kept, since the other ones are only polluted by noise.
            const double elapsed_ns = measure(p_entry, iterations);
            if (elapsed_ns < result.total_ns) {
                result.total_ns = elapsed_ns;
            }
        }

        result.ns_per_iteration = static_cast<double>(iterations) > 0.0
                                      ? result.total_ns / static_cast<double>(iterations)
                                      : 0.0;
        result.iterations_per_second = result.ns_per_iteration > 0.0
                                           ? 1000000000.0 / result.ns_per_iteration
                                           : 0.0;
        return result;
    }

    // Returns whether the given name matches the given filter (case sensitive substring match).
    // An empty filter matches everything.
    static bool matches(const char *p_name, const std::string &p_filter) {
        return p_filter.empty() || std::string(p_name).find(p_filter) != std::string::npos;
    }
    /*-------------------------------------------------------------------------------*/

    public:
    // ── Compiler barriers ───────────────────────────────────────────────────────────────────

    // Prevents the compiler from optimizing a value away, keeping the computation which
    // produced it alive. Every value a benchmark computes has to be passed through this
    // function, otherwise the optimizer is free to delete the whole benchmark body.
    // NOTE: Not 'func' - inline assembly is a runtime-only operation.
    template <typename T>
    static void do_not_optimize(T &p_value) {
#if defined(GNUC_ENABLED) || defined(CLANG_ENABLED)
        asm volatile("" : : "g"(p_value) : "memory");
#else
        volatile T *volatile pointer = &p_value;
        (void)pointer;
#endif
    }

    // Prevents the compiler from optimizing a temporary away, keeping the computation which
    // produced it alive. This overload is picked for rvalues (the result of an operator) and
    // for const lvalues.
    // NOTE: Not 'func' - inline assembly is a runtime-only operation.
    template <typename T>
    static void do_not_optimize(const T &p_value) {
#if defined(GNUC_ENABLED) || defined(CLANG_ENABLED)
        asm volatile("" : : "g"(p_value) : "memory");
#else
        volatile const T *volatile pointer = &p_value;
        (void)pointer;
#endif
    }
    /*-------------------------------------------------------------------------------*/

    // ── Helpers ────────────────────────────────────────────────────────────────────────────

    // A small stopwatch, usable inside a benchmark body to measure sub-sections.
    struct Stopwatch {
        private:
        // The clock reading taken when the stopwatch was started.
        double start_ns = 0.0;

        public:
        // Starts (or restarts) the stopwatch.
        void start() { start_ns = Benchmarker::now_ns(); }

        // Returns the time elapsed since the last 'start', in nanoseconds.
        double elapsed_ns() const { return Benchmarker::now_ns() - start_ns; }

        // Returns the time elapsed since the last 'start', in milliseconds.
        double elapsed_ms() const { return elapsed_ns() / 1000000.0; }
    };

    // The value which benchmark results are written into, so that nothing is optimized away.
    // Pass it through 'do_not_optimize' once a benchmark is done with it.
    static double &blackhole() {
        static double value = 0.0;
        return value;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Runtime registration ────────────────────────────────────────────────────────────────

    // Registers a benchmark to be executed by 'run'.
    // 'p_min_time_ms' is the minimum duration of a single sample, 'p_min_iterations' the
    // amount of iterations the calibration starts from.
    // Safe to call from any translation unit at any time before (or during) a run.
    static void register_benchmark(const char *p_name, BenchBody p_body,
                                   const double p_min_time_ms = DEFAULT_MIN_TIME_MS,
                                   const unsigned long long p_min_iterations = 1ull) {
        registry().push_back(BenchEntry{p_name, p_body, p_min_time_ms, p_min_iterations});
    }

    // Registers a benchmark the moment the object is constructed.
    // Declare one at namespace scope to register a benchmark without touching 'main':
    //     static const Benchmarker::AutoBenchmark registration("name", &body);
    class AutoBenchmark {
        public:
        AutoBenchmark(const char *p_name, BenchBody p_body,
                      const double p_min_time_ms = DEFAULT_MIN_TIME_MS,
                      const unsigned long long p_min_iterations = 1ull) {
            register_benchmark(p_name, p_body, p_min_time_ms, p_min_iterations);
        }
    };

    // Returns the number of registered benchmarks.
    static std::size_t benchmark_count() { return registry().size(); }

    // Returns whether at least one benchmark has been registered.
    static bool has_benchmarks() { return !registry().empty(); }
    /*-------------------------------------------------------------------------------*/

    // ── Execution ───────────────────────────────────────────────────────────────────────────

    // Prints every registered benchmark which matches the given filter, one per line.
    static void list(const std::string &p_filter = std::string()) {
        Console console;
        std::size_t shown = 0;
        for (const BenchEntry &entry : registry()) {
            if (matches(entry.name, p_filter)) {
                ++shown;
            }
        }

        std::printf("%s%s%llu benchmarks%s\n", console.paint(Console::bold()),
                    console.paint(Console::cyan()), (unsigned long long)shown,
                    console.paint(Console::reset()));
        for (const BenchEntry &entry : registry()) {
            if (matches(entry.name, p_filter)) {
                std::printf("  %s\n", entry.name);
            }
        }
    }

    // Returns the width of the name column, so that every benchmark lines up.
    static std::size_t measure_name_width(const std::string &p_filter) {
        std::size_t width = 0;
        for (const BenchEntry &entry : registry()) {
            if (!matches(entry.name, p_filter)) {
                continue;
            }
            const std::size_t length = std::strlen(entry.name);
            if (length > width) {
                width = length;
            }
        }
        return width < MAX_NAME_WIDTH ? width : MAX_NAME_WIDTH;
    }

    // Prints the result table, sorted from the fastest to the slowest benchmark, followed by
    // the summary of the run.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    int run(const std::string &p_filter = std::string(),
            const double p_min_time_ms = DEFAULT_MIN_TIME_MS) {
        const std::size_t name_width = measure_name_width(p_filter);
        const double start_ns = now_ns();

        std::printf("%s%sRatLab benchmarks%s  %s%s%s%s\n", console.paint(Console::bold()),
                    console.paint(Console::cyan()), console.paint(Console::reset()),
                    console.separator(), Console::platform_name(), console.separator(),
                    Console::cpp_standard_name());

        results().clear();
        for (const BenchEntry &entry : registry()) {
            if (!matches(entry.name, p_filter)) {
                continue;
            }
            BenchEntry effective = entry;
            effective.min_time_ms = p_min_time_ms;
            results().push_back(run_entry(effective, DEFAULT_SAMPLES));
        }
        const double total_ns = now_ns() - start_ns;

        // The fastest benchmark ends up on top, which is the one worth looking at first.
        std::sort(results().begin(), results().end(),
                  [](const BenchResult &p_a, const BenchResult &p_b) {
                      return p_a.ns_per_iteration < p_b.ns_per_iteration;
                  });

        if (results().empty()) {
            std::printf("\n  %sno benchmark matched the filter%s\n",
                        console.paint(Console::yellow()), console.paint(Console::reset()));
            return 1;
        }

        std::printf("\n  %s%-*s  %15s  %10s  %10s%s\n", console.paint(Console::dim()),
                    (int)name_width, "benchmark", "iterations", "ns/iter", "iter/s",
                    console.paint(Console::reset()));

        unsigned long long rank = 0;
        for (const BenchResult &result : results()) {
            ++rank;
            char buffer_iterations[24];
            char buffer_rate[32];
            Console::format_count(buffer_iterations, sizeof(buffer_iterations), result.iterations);
            Console::format_rate(buffer_rate, sizeof(buffer_rate), result.iterations_per_second);

            // The time per iteration stays a plain number of nanoseconds: it is the column
            // every result gets compared by, and magnitude prefixes would only hide it.
            std::printf("  %s%2llu%s  %-*s  %15s  %10.3f  %10s\n",
                        console.paint(Console::dim()), rank, console.paint(Console::reset()),
                        (int)name_width, result.name, buffer_iterations,
                        result.ns_per_iteration, buffer_rate);
        }

        print_summary(total_ns, p_min_time_ms);
        return 0;
    }

    // Parses the runtime arguments and runs the requested benchmarks.
    // Supported arguments: '--filter <text>', '--filter=<text>', '--min-time <ms>', '--list',
    // '--help'.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    int run(const int p_argc, char **p_argv) {
        const char *program = p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_benchmarks";
        std::string filter;
        bool list_only = false;
        double min_time_ms = DEFAULT_MIN_TIME_MS;

        for (int index = 1; index < p_argc; ++index) {
            const char *argument = p_argv[index] == nullptr ? "" : p_argv[index];
            if (std::strcmp(argument, "--list") == 0) {
                list_only = true;
            } else if (std::strcmp(argument, "--filter") == 0 && index + 1 < p_argc) {
                filter = p_argv[++index];
            } else if (std::strncmp(argument, "--filter=", 9) == 0) {
                filter = argument + 9;
            } else if (std::strcmp(argument, "--min-time") == 0 && index + 1 < p_argc) {
                min_time_ms = std::atof(p_argv[++index]);
            } else if (std::strcmp(argument, "--help") == 0 || std::strcmp(argument, "-h") == 0) {
                print_usage(program);
                return 0;
            } else {
                std::printf("Unknown argument: %s\n", argument);
                print_usage(program);
                return 1;
            }
        }

        if (list_only) {
            list(filter);
            return 0;
        }

        return run(filter, min_time_ms);
    }

    // Returns the result of the benchmark with the given name, or 'nullptr' when it was not run.
    static const BenchResult *find_result(const char *p_name) {
        for (const BenchResult &result : results()) {
            if (std::strcmp(result.name, p_name) == 0) {
                return &result;
            }
        }
        return nullptr;
    }

    // Prints the footer of a benchmark run: the environment, the time spent measuring and
    // the fastest benchmark.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    void print_summary(const double p_total_ns, const double p_min_time_ms) const {
        char buffer_time[32];
        char buffer_rate[32];
        const BenchResult &fastest = results()[0];
        Console::format_duration(buffer_time, sizeof(buffer_time), p_total_ns);
        Console::format_rate(buffer_rate, sizeof(buffer_rate), fastest.iterations_per_second);

        std::printf("\n");
        console.rule(REPORT_WIDTH);
        std::printf("  %llu %s%s%s%s%llu %s%s%.0f ms minimum%s%s\n",
                    (unsigned long long)results().size(),
                    Console::plural(results().size(), "benchmark", "benchmarks"),
                    console.separator(), Console::compiler_name(), console.separator(),
                    (unsigned long long)DEFAULT_SAMPLES,
                    Console::plural(DEFAULT_SAMPLES, "sample", "samples"), console.separator(),
                    p_min_time_ms, console.separator(), buffer_time);
        std::printf("  %sfastest%s  %s%s%s  %s%.4f ns/iter%s  %s\n",
                    console.paint(Console::dim()), console.paint(Console::reset()),
                    console.paint(Console::bold()), fastest.name, console.paint(Console::reset()),
                    console.paint(Console::green()), fastest.ns_per_iteration,
                    console.paint(Console::reset()), buffer_rate);
    }

    // Prints the usage text of the benchmark runner.
    static void print_usage(const char *p_program) {
        Console console;
        std::printf("%sUsage:%s %s [options]\n", console.paint(Console::bold()),
                    console.paint(Console::reset()), p_program);
        std::printf("  %-18s Only run the benchmarks whose name contains <text>\n", "--filter <text>");
        std::printf("  %-18s Minimum duration of a single sample (default %.0f ms)\n",
                    "--min-time <ms>", DEFAULT_MIN_TIME_MS);
        std::printf("  %-18s List the registered benchmarks without running them\n", "--list");
        std::printf("  %-18s Show this text\n", "--help, -h");
    }
};
