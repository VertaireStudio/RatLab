/************************************/
/*         benchmarker.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#if defined(_WIN32)
    #include <io.h>
    #define BENCHMARKER_ISATTY(stream) (_isatty(_fileno(stream)) != 0)
#else
    #include <unistd.h>
    #define BENCHMARKER_ISATTY(stream) (::isatty(::fileno(stream)) != 0)
#endif

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

    // The minimum amount of time (in milliseconds) a single sample has to take by default.
    static constexpr const double DEFAULT_MIN_TIME_MS = 25.0;
    // The hard upper limit for the calibrated iteration count, guarding against pathological bodies.
    static constexpr const unsigned long long MAX_ITERATIONS = 1ull << 32;
    // The amount of samples taken per benchmark; the fastest one is reported.
    static constexpr const unsigned long long DEFAULT_SAMPLES = 3ull;
    /*-------------------------------------------------------------------------------*/

    // ── Colors ──────────────────────────────────────────────────────────────────────────────

    func static const char *bold() { return "\x1b[1m"; }
    func static const char *dim() { return "\x1b[2m"; }
    func static const char *red() { return "\x1b[31m"; }
    func static const char *green() { return "\x1b[32m"; }
    func static const char *yellow() { return "\x1b[33m"; }
    func static const char *cyan() { return "\x1b[36m"; }
    func static const char *reset() { return "\x1b[0m"; }

    // Colors are stripped entirely when stdout is not a terminal (CI logs stay plain).
    func static const char *paint(const bool p_use_color, const char *p_code) {
        return p_use_color ? p_code : "";
    }
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
#elif defined(MSVC_ENABLED)
        std::atomic_thread_fence(std::memory_order_seq_cst);
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

    // Runs a single benchmark to completion and stores its result.
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

    // Prints every registered benchmark which matches the given filter.
    static void list(const std::string &p_filter = std::string()) {
        std::printf("Registered benchmarks: %llu\n", (unsigned long long)registry().size());
        for (const BenchEntry &entry : registry()) {
            if (matches(entry.name, p_filter)) {
                std::printf("  %s (min time %.1f ms)\n", entry.name, entry.min_time_ms);
            }
        }
    }

    // Runs every registered benchmark which matches the given filter, prints the result
    // table and returns 0. Benchmarks never 'fail', so the return value only reports
    // whether at least one benchmark was actually run.
    // 'p_min_time_ms' overrides the minimum sample duration of every benchmark.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    int run(const std::string &p_filter = std::string(),
            const double p_min_time_ms = DEFAULT_MIN_TIME_MS) {
        const bool use_color = BENCHMARKER_ISATTY(stdout);
        std::size_t executed = 0;

        std::printf("%s%-34s %14s %12s %12s %14s%s\n",
                    paint(use_color, bold()), "Benchmark", "Iterations", "Total (ms)",
                    "ns/iter", "Iter/s", paint(use_color, reset()));
        std::printf("--------------------------------------------------------------\n");

        results().clear();
        for (const BenchEntry &entry : registry()) {
            if (!matches(entry.name, p_filter)) {
                continue;
            }
            BenchEntry effective = entry;
            effective.min_time_ms = p_min_time_ms;
            const BenchResult result = run_entry(effective, DEFAULT_SAMPLES);
            results().push_back(result);
            std::printf("%-34s %14llu %12.3f %12.4f %14.0f\n", result.name, result.iterations,
                        result.total_ns / 1000000.0, result.ns_per_iteration,
                        result.iterations_per_second);
            std::fflush(stdout);
            ++executed;
        }

        std::printf("--------------------------------------------------------------\n");
        print_summary(use_color, p_min_time_ms, executed);
        return executed > 0 ? 0 : 1;
    }

    // Parses the runtime arguments and runs the requested benchmarks.
    // Supported arguments: '--filter <text>', '--filter=<text>', '--list', '--min-time <ms>',
    // '--help'.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    int run(const int p_argc, char **p_argv) {
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
                print_usage(p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_benchmarks");
                return 0;
            } else {
                std::printf("Unknown argument: %s\n", argument);
                print_usage(p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_benchmarks");
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

    // Prints the platform information every measurement was taken on.
    static void print_environment(const bool p_use_color, const double p_min_time_ms) {
        std::printf("%sPlatform: %s%s\n", paint(p_use_color, dim()),
#if defined(LINUX_ENABLED)
                    "Linux",
#elif defined(WINDOWS_ENABLED)
                    "Windows",
#elif defined(MACOS_ENABLED)
                    "macOS",
#else
                    "Unknown",
#endif
                    paint(p_use_color, reset()));
        std::printf("%sCompiler: C++%d, min time per sample: %.1f ms, samples: %llu%s\n",
                    paint(p_use_color, dim()), CPP_VERSION, p_min_time_ms, DEFAULT_SAMPLES,
                    paint(p_use_color, reset()));
    }

    // Prints the usage text of the benchmark runner.
    static void print_usage(const char *p_program) {
        std::printf("Usage: %s [--filter <text>] [--min-time <ms>] [--list] [--help]\n", p_program);
        std::printf("  --filter <text>    Only run the benchmarks whose name contains <text>\n");
        std::printf("  --min-time <ms>    Minimum duration of a single sample (default %.1f ms)\n",
                    DEFAULT_MIN_TIME_MS);
        std::printf("  --list             List the registered benchmarks without running them\n");
        std::printf("  --help             Show this text\n");
    }

    // Prints the footer of a benchmark run: the environment, the amount of executed
    // benchmarks and the fastest one.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    static void print_summary(const bool p_use_color, const double p_min_time_ms,
                              const std::size_t p_executed) {
        print_environment(p_use_color, p_min_time_ms);

        if (p_executed == 0) {
            std::printf("\n%sNo benchmark was executed.%s\n", paint(p_use_color, yellow()),
                        paint(p_use_color, reset()));
            return;
        }

        const BenchResult *fastest = &results()[0];
        for (const BenchResult &result : results()) {
            if (result.ns_per_iteration < fastest->ns_per_iteration) {
                fastest = &result;
            }
        }

        std::printf("\n%sBenchmarks run: %llu%s\n", paint(p_use_color, bold()),
                    (unsigned long long)p_executed, paint(p_use_color, reset()));
        std::printf("%sFastest: %s (%s%.4f ns/iter, %.0f iter/s)%s\n", paint(p_use_color, dim()),
                    fastest->name, paint(p_use_color, green()), fastest->ns_per_iteration,
                    fastest->iterations_per_second, paint(p_use_color, reset()));
    }
};
