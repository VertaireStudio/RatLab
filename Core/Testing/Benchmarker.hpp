/************************************/
/*         benchmarker.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "console.hpp"
#include "statistics.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Both are defined further down: the runner builds a 'Bencher' for every batch it measures and
// hands it to a routine, and a 'BenchmarkGroup' registers benchmarks back into the runner.
class Bencher;
class BenchmarkGroup;

// The BenchSize type: How often the setup of a batched benchmark runs relative to the routine
// it feeds. Every mode is measured with its setup; what they differ in is how much of a
// sample a single setup is worth.
enum class BenchSize {
    // Once per iteration, which is the cheapest to write and the only honest choice when the
    // setup is part of what the benchmark is about.
    SmallInput,
    // Once per batch, with the input then taken by value by every call of the routine which
    // follows it.
    LargeInput,
    // Once per call of the routine, i.e. 'iterations' times more often than measured.
    PerIteration,
};

// The Throughput type: What a single iteration of a benchmark produces. It is what lets a
// benchmark report a rate next to a time, e.g. '29.23 Gelem/s' beside '0.0342 ns'.
class Throughput {
    public:
    // One element per iteration, which is what a benchmark without an explicit throughput
    // reports.
    func static Throughput elements() { return Throughput{1ull, "elem"}; }
    // The given amount of elements per iteration.
    func static Throughput elements(const unsigned long long p_amount) {
        return Throughput{p_amount, "elem"};
    }
    // The given amount of bytes per iteration.
    func static Throughput bytes(const unsigned long long p_amount) {
        return Throughput{p_amount, "byte"};
    }
    /*-------------------------------------------------------------------------------*/

    // The amount a single iteration produces.
    unsigned long long amount = 1ull;
    // The name of what a single iteration produces, as it is written in the report.
    const char *unit = "elem";
};

// The BenchmarkId type: The name of a single benchmark and, when it was run with a parameter,
// the value of that parameter. Criterion's id, built out of the group a benchmark is declared
// in and the name it is given there, so that neither half is ever spelled out twice.
class BenchmarkId {
    public:
    // Returns the id of a benchmark which is run without a parameter.
    func static BenchmarkId plain(const char *p_name) { return BenchmarkId{p_name, ""}; }

    // Returns the id of a benchmark which is run with the given parameter, e.g. 'u8/get/128'.
    static BenchmarkId from_parameter(const char *p_name, const unsigned long long p_value);
    static BenchmarkId from_parameter(const char *p_name, const double p_value);
    static BenchmarkId from_parameter(const char *p_name, const char *p_value);
    /*-------------------------------------------------------------------------------*/

    // Returns the full id: the name on its own, and the name and its parameter together.
    std::string full() const {
        return parameter.empty() ? name : name + "/" + parameter;
    }

    // The name of the benchmark itself, without the parameter it was run with.
    std::string name;
    // The parameter it was run with, empty when it was run without one.
    std::string parameter;
};

// The Benchmarker type: A self-contained, dependency-free benchmark framework for RatLab,
// modeled after Criterion, the benchmark tool of Rust.
//
// A run is one function which is handed a runner and registers its benchmarks into it:
//
//     void bench_u8(Benchmarker &p_benchmarker) {
//         BenchmarkGroup group = p_benchmarker.benchmark_group("u8");
//         group.bench_function("get", [](Bencher &p_bencher) {
//             unsigned char counter = 0;
//             p_bencher.iter([&] {
//                 counter = (unsigned char)(counter + 1u);
//                 const u8 source = Benchmarker::black_box<u8>(counter);
//                 return source.get();
//             });
//         });
//     }
//
//     BENCHMARK_GROUP(ratlab, bench_u8)
//     BENCHMARK_MAIN(ratlab)
//
// A group function is named from the file which holds 'main', so unlike every other function
// of the workspace it cannot be 'static': a static function of one translation unit is
// invisible to another.
// Registration happens at runtime, exactly like Criterion's 'criterion_main!': the command
// line is read first, so that a run can be filtered, shortened or turned into a comparison
// against a baseline without any of it being written into the benchmarks themselves.
//
// Every benchmark is measured in the three phases Criterion uses. The amount of iterations a
// single sample performs is first calibrated until a sample is long enough to be timed at
// all. The benchmark is then warmed up for as long as the configuration asks for, so that the
// caches and the clock of the machine have settled on the work. Only then are the samples
// taken, and every one of them is kept: the report is built out of them rather than out of a
// single number, which is why every estimate in it comes with an interval and why a run can
// be compared against another one instead of only being read.
//
// A body is handed a 'Bencher' and spends the iterations of a sample through it. Two things
// have to be held back from the optimizer for a measurement to mean anything, and both are
// offered by this framework: 'black_box' makes an operand opaque for free and keeps the work
// around it inside the loop, and 'do_not_optimize' keeps a result alive by paying for a round
// trip through memory. Neither of them helps a predicate which answers the same thing every
// time, since there is nothing to keep: a body whose work is a constant the compiler can
// prove reports that constant's cost. The details are in 'Benchmarks/u8_bench.cpp', which is
// where they are demonstrated.
//
// The outcome of every run is additionally written to 'REPORT_FILE' as plain text, so that it
// survives the process and can be diffed against another run. '--save-baseline' writes the
// raw samples into a file instead, which is what a later run is compared against.
class Benchmarker {
    public:
    // The configuration of a run. Every value in it has a default meant to be left alone: the
    // numbers it produces are only comparable between runs which used the same one, which is
    // the whole point of having it in one place.
    struct Config {
        // How long a benchmark runs before a single sample is taken, in seconds. It is what
        // lets the caches, the branch predictors and the clock of the machine settle on the
        // work before anything is written down.
        double warm_up_time = 0.3;
        // How long the samples of a benchmark are collected, in seconds.
        double measurement_time = 1.0;
        // The amount of samples taken per benchmark. Every estimate is built out of these.
        unsigned long long sample_size = 100ull;
        // How often the samples are resampled to put an interval around an estimate.
        unsigned long long nresamples = 1000ull;
        // The share of the bootstrapped distribution an interval covers.
        double confidence_level = 0.95;
        // The smallest relative change a comparison calls a change at all.
        double noise_threshold = 0.01;
        // How likely the difference between two sets of samples may be before it is called a
        // change rather than noise.
        double significance_level = 0.05;
        // What a single iteration produces, reported next to the time it takes.
        Throughput throughput = Throughput::elements();

        // Returns the shorter configuration '--quick' asks for: ten samples are enough to
        // tell a real change from noise on a quiet machine, at a fraction of the time.
        func static Config quick() {
            Config shortened;
            shortened.warm_up_time = 0.1;
            shortened.measurement_time = 0.2;
            shortened.sample_size = 10ull;
            shortened.noise_threshold = 0.05;
            return shortened;
        }
    };
    /*---------------------------------------------------------------------------------------*/

    // The measured outcome of a single benchmark, which is what a report is written from and
    // what a later run is compared against.
    struct BenchResult {
        // The full id of the benchmark: the group it was declared in and its own name.
        std::string id;
        // How this one benchmark was measured, and what it produces.
        Config config;
        // Whether the samples below were measured by this run, as opposed to being nothing at
        // all. Only a run which compares instead of measuring reports false.
        bool measured = false;
        // Whether a sample is long enough for its own timing to mean anything. A body whose
        // work the optimizer removed leaves the batches below the resolution of the clock,
        // which is reported rather than quoted as the cost of a benchmark.
        bool resolvable = true;
        // The amount of iterations a single sample performed. It is the same for every sample
        // unless the benchmark drove its own iteration count.
        unsigned long long iterations = 0ull;
        // The time a single sample took, in nanoseconds, one entry per sample.
        std::vector<double> sample_time_ns;
        // The time a single iteration took in every single sample, in nanoseconds. Derived
        // from the two above and kept because every statistic is written in those terms.
        std::vector<double> samples;
        // The indexes of the samples which sat too far away from the middle of the rest.
        std::vector<std::size_t> outliers;
        // The index of the sample which sat the furthest away from the middle of the rest.
        std::size_t worst = 0;
        // The estimated cost of a single iteration, which is the average of the samples.
        Statistics::Estimate typical;
        // The average of the samples.
        Statistics::Estimate mean;
        // The middle of the samples.
        Statistics::Estimate median;
        // How far the samples are spread around their average. A point value: an interval
        // around a standard deviation is a statement about the samples, not about the work.
        double deviation = 0.0;
        // How far half of the samples are from the middle of the rest.
        double absolute_deviation = 0.0;
        // Whether the benchmark decided its own amount of iterations, rather than being given
        // one which was derived from its own cost.
        bool driven = false;
        // How this run compares against the loaded baseline, when one was loaded.
        Statistics::Comparison comparison;
        // What that comparison amounted to.
        Statistics::Verdict verdict = Statistics::Verdict::no_change;
        // Whether a comparison was made at all.
        bool compared = false;
        // The time the whole benchmark took, calibration and warm-up included.
        double total_ns = 0.0;
    };
    /*---------------------------------------------------------------------------------------*/

    private:
    // What a run is supposed to do, decided by the command line before anything is measured.
    enum class Mode {
        // Measure every benchmark which matches the filter.
        measure,
        // Print the benchmarks which match the filter and stop.
        list,
        // Print the usage text and stop.
        help,
        // The arguments did not make sense: the usage text is printed and the run fails.
        failed,
    };

    // A single registered benchmark, waiting to be measured.
    struct BenchEntry {
        // The full id of the benchmark.
        std::string id;
        // The body which performs the work.
        std::function<void(Bencher &)> routine;
        // How this one benchmark is measured, which is the configuration of its group.
        Config config;
    };

    // What a single batch of a benchmark turned out to be.
    struct Batch {
        // How long the whole batch took, in nanoseconds.
        double time_ns = 0.0;
        // How many iterations it really spent, which is only different from what it was asked
        // for when the benchmark drove its own count.
        unsigned long long iterations = 0ull;
        // Whether the body spent any of the iterations at all.
        bool worked = true;
        // Whether the benchmark decided its own amount of iterations, rather than being given
        // one which was derived from its own cost.
        bool driven = false;
    };

    // The samples of a baseline, by the id of the benchmark they were taken from.
    typedef std::map<std::string, std::vector<double>> Baseline;
    /*-------------------------------------------------------------------------------*/

    // The configuration every benchmark of this run is measured with, unless the group it was
    // declared in says otherwise.
    Config config;
    // The measurements of this run, in the order the benchmarks were registered.
    std::vector<BenchResult> results;
    // The samples of the loaded baseline, empty when the run does not compare against one.
    Baseline baseline;
    // What this run is supposed to do.
    Mode mode = Mode::measure;
    // Only the benchmarks whose id contains this are looked at. An empty filter is everything.
    std::string filter;
    // The file the samples of this run are written into, empty when they are not saved.
    std::string save_path;
    // The file this run is compared against, empty when it is not compared.
    std::string load_path;
    // Whether the measurements are left out of the report and only the comparison is kept.
    bool comparison_only = false;
    // Whether every sample of every benchmark is printed.
    bool verbose = false;
    // Whether only the cost of every benchmark is printed.
    bool quiet = false;
    // The name the runner was started under, used by the usage text.
    std::string program = "ratlab_benchmarks";
    // The amount of benchmarks which matched the filter.
    unsigned long long matched = 0ull;
    // The time the whole run took, in nanoseconds.
    double total_ns = 0.0;
    // The shared output plumbing.
    Console console;
    /*-------------------------------------------------------------------------------*/

    // How long a single calibration batch has to take before its timing is believed, in
    // nanoseconds. Long enough for the clock to dominate over its own overhead, short enough
    // that calibrating a suite of benchmarks stays cheap.
    static constexpr const double CALIBRATION_TIME_NS = 1000000.0;
    // How much a batch has to get slower when its iteration count is doubled for the doubling
    // to be believed to have measured anything. A batch which is faster after being given twice
    // the work was not timed to begin with.
    static constexpr const double GROWTH_TOLERANCE = 1.5;
    // How long a batch has to be before its own timing is trusted to have grown when it was
    // given twice the work. Below this the reading is so close to the overhead of the clock
    // that doubling the work does not show up in it, and a batch which does grow would be
    // taken for one which does not.
    static constexpr const double GROWTH_FLOOR_NS = 100000.0;
    // The shortest batch whose timing is worth anything, in nanoseconds. Below this a reading
    // is mostly the overhead of the clock itself, which is why a benchmark below it is reported
    // as being faster than the clock resolves rather than quoted as a cost.
    static constexpr const double MINIMUM_BATCH_NS = 100.0;
    // The hard upper limit of the calibrated iteration count, guarding against a body which
    // does not scale with the amount of work it was given.
    static constexpr const unsigned long long MAX_ITERATIONS = 1ull << 32;
    // The fewest samples which are collected even when the measurement time is already spent.
    static constexpr const unsigned long long MINIMUM_SAMPLES = 10ull;
    // The width of the summary, used to draw the rule above it.
    static constexpr const std::size_t REPORT_WIDTH = 74;
    // The directory the report of every run is written into, relative to the working
    // directory of the process. It is created when it does not exist yet.
    static constexpr const char *REPORT_DIRECTORY = "Misc";
    // The report itself, overwritten each time so that it always holds the most recent run.
    static constexpr const char *REPORT_FILE = "Misc/ratlab_benchmarks.txt";
    /*-------------------------------------------------------------------------------*/

    // ── Measurement ─────────────────────────────────────────────────────────────────────────

    // Prevents the compiler from reordering or merging work across this point, which is what
    // keeps the work of a sample from being pulled across the edges of its own timing.
    // NOTE: Not 'func' - inline assembly is a runtime-only operation.
    static void clobber_memory() {
#if defined(GNUC_ENABLED) || defined(CLANG_ENABLED)
        asm volatile("" : : : "memory");
#else
        std::atomic_signal_fence(std::memory_order_seq_cst);
#endif
    }

    // ── Filtering ───────────────────────────────────────────────────────────────────────────

    // Returns whether the given id matches the given filter (a case sensitive substring, the
    // rule the tester uses as well; a regular expression would be a dependency).
    static bool matches(const std::string &p_id, const std::string &p_filter) {
        return p_filter.empty() || p_id.find(p_filter) != std::string::npos;
    }

    // ── Registration ────────────────────────────────────────────────────────────────────────

    // Registers a benchmark under the given id and measures it, unless this run is only
    // listing benchmarks. Everything a run reports comes out of here.
    template <typename F>
    void bench_entry(const BenchmarkId &p_id, F &&p_routine, const Config &p_config) {
        const std::string id = p_id.full();
        if (!matches(id, filter)) {
            return;
        }
        matched += 1ull;

        if (mode == Mode::list) {
            std::printf("  %s\n", id.c_str());
            return;
        }

        BenchEntry entry;
        entry.id = id;
        entry.routine = std::forward<F>(p_routine);
        entry.config = p_config;

        BenchResult result;
        result.id = id;
        result.config = p_config;

        measure(entry, result);
        compare(result);
        results.push_back(result);
        print_result(result);
    }

    // Returns the id of a benchmark which is run with the given parameter, which only the
    // types a parameter can be written as are supported for.
    template <typename T>
    static BenchmarkId parameter_id(const char *p_name, const T &p_input) {
        if constexpr (std::is_integral_v<T>) {
            return BenchmarkId::from_parameter(p_name, static_cast<unsigned long long>(p_input));
        } else if constexpr (std::is_floating_point_v<T>) {
            return BenchmarkId::from_parameter(p_name, static_cast<double>(p_input));
        } else if constexpr (std::is_convertible_v<T, const char *>) {
            return BenchmarkId::from_parameter(p_name, static_cast<const char *>(p_input));
        } else {
            static_assert(sizeof(T) == 0,
                          "a benchmark parameter is written into the id of its benchmark, so it "
                          "has to be a number or a string");
            return BenchmarkId::plain(p_name);
        }
    }
    /*-------------------------------------------------------------------------------*/

    public:
    // ── Construction ────────────────────────────────────────────────────────────────────────

    // Constructor, with the default configuration.
    Benchmarker() = default;
    // Constructor, with the given configuration.
    explicit Benchmarker(const Config &p_config) : config(p_config) {}

    // Deleted copy constructor: a run owns its own measurements and its own output.
    Benchmarker(const Benchmarker &) = delete;
    // Deleted copy assignment: a run owns its own measurements and its own output.
    Benchmarker &operator=(const Benchmarker &) = delete;
    /*-------------------------------------------------------------------------------*/

    // Returns the configuration every benchmark of this run is measured with. Changing it
    // between two benchmarks is how a group is tuned without touching the command line.
    Config &settings() { return config; }
    // Returns the configuration of this run.
    const Config &settings() const { return config; }
    /*-------------------------------------------------------------------------------*/

    // Registers a benchmark and measures it right away. The routine is handed a 'Bencher',
    // which owns the amount of iterations a sample performs and spends them with one of its
    // own methods.
    template <typename F>
    void bench_function(const char *p_name, F &&p_routine) {
        bench_entry(BenchmarkId::plain(p_name), std::forward<F>(p_routine), config);
    }

    // Registers a benchmark which is run with the given input, which is handed to the routine
    // by reference. The input has to outlive the call, which is the rule Criterion sets too.
    template <typename T, typename F>
    void bench_with_input(const char *p_name, const T &p_input, F &&p_routine) {
        // The input is handed over as a black boxed reference: the optimizer cannot see
        // through it, so the work cannot be folded away, while the caller can still not change
        // it while it is being measured.
        const T *input = &p_input;
        bench_entry(parameter_id(p_name, p_input),
                    [input, p_routine](Bencher &p_bencher) {
                        p_routine(p_bencher, *black_box(input));
                    },
                    config);
    }

    // Returns a group of benchmarks which share an id prefix and a configuration, which is
    // Criterion's group. Everything inside it is reported as '<prefix>/<name>'.
    BenchmarkGroup benchmark_group(const char *p_id);
    /*-------------------------------------------------------------------------------*/

    // ── The clock ───────────────────────────────────────────────────────────────────────────

    // Returns the current value of the monotonic clock, in nanoseconds. It is the one clock
    // every measurement of a run is taken from, and it is public because a routine which
    // drives its own iteration count times itself with it.
    // NOTE: Not 'func' - the clock is read at runtime.
    static double now_ns() {
        return static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    /*-------------------------------------------------------------------------------*/

    // ── Obstacles to the optimizer ─────────────────────────────────────────────────────────

    // Hands the given value to the compiler and gets it back, which is what stops the value
    // from being a constant the expression around it is evaluated at while the workspace is
    // being compiled. It emits no code at all, so it is free to use inside a measured loop.
    //
    // It also keeps the expression it is written around inside the loop it is measured in.
    // A volatile assembly statement is not moved, so work which would otherwise be hoisted out
    // of the loop as the same on every iteration has to stay and be done again. That is a
    // property of 'volatile' and not of the empty string: an assembly statement without it is
    // dead as soon as nothing reads its output, and a benchmark drops its results constantly —
    // the result of the last iteration of a sample is thrown away, and so is any result no
    // body looks at. Measured over 'u8/arithmetic' on GCC 13.3, a plain assembly statement
    // reports 0.27 ns per iteration for a body of ten operator overloads including four
    // divides, which is the cost of the loop and nothing else; the same body behind a volatile
    // one reports 6.39 ns.
    template <typename T>
    static T black_box(T p_value) {
        static_assert(std::is_trivially_copyable_v<T>,
                      "black_box hands the value to the compiler through a register or a cell of "
                      "memory, which is only defined for trivially copyable types");
#if defined(GNUC_ENABLED) || defined(CLANG_ENABLED)
        // An empty assembly statement with the value as both its input and its output: the
        // compiler has to materialize the value, and it cannot know where it came from.
        //
        // 'volatile' is what makes that true at runtime. Without it the statement is dead as
        // soon as its output goes unused, and a benchmark is nothing but unused results: it
        // deletes the statement, and the work it was written to protect goes with it.
        asm volatile("" : "+rm"(p_value) : :);
        return p_value;
#else
        // MSVC has no inline assembly to ask, so the value takes the round trip through a
        // volatile cell instead. It is not free, which is the one thing a RatLab measurement
        // cannot be compared across compilers on.
        do_not_optimize(p_value);
        return p_value;
#endif
    }

    // Keeps the given value alive by writing it into a volatile cell and reading it back.
    // Both accesses are side effects the optimizer may not drop, so whatever produced the
    // value has to have run.
    //
    // It is the stronger of the two obstacles and it is paid for: a round trip through memory
    // is a store and a load, and the measurement includes both. A body only needs it where
    // 'black_box' is not enough, e.g. when the result of an iteration is the same value on
    // every iteration, as '++value; --value;' is for every input there is.
    template <typename T>
    static void do_not_optimize(const T &p_value) {
        static_assert(std::is_trivially_copyable_v<T>,
                      "do_not_optimize copies the value through a volatile cell, which is only "
                      "defined for trivially copyable types");
        volatile T cell = p_value;
        const T read = cell;
        (void)read;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Command line ────────────────────────────────────────────────────────────────────────

    // Reads the runtime arguments into the configuration of this run, the way Criterion's
    // 'configure_from_args' does, and returns the runner so that the call can be chained.
    // NOTE: Not 'func' - the arguments are read at runtime.
    Benchmarker &configure_from_args(const int p_argc, char **p_argv);

    // Runs the given group of benchmarks and reports the run.
    // NOTE: Not 'func' - benchmarks are measured at runtime.
    int run(void (*p_group)(Benchmarker &));

    // Reads the runtime arguments, runs the given group of benchmarks and reports the run.
    // This is what 'BENCHMARK_MAIN' hands its arguments to.
    // NOTE: Not 'func' - benchmarks are measured at runtime.
    static int run(const int p_argc, char **p_argv, void (*p_group)(Benchmarker &)) {
        Benchmarker benchmarker;
        benchmarker.configure_from_args(p_argc, p_argv);
        return benchmarker.run(p_group);
    }
    /*-------------------------------------------------------------------------------*/

    // ── Results ────────────────────────────────────────────────────────────────────────────

    // Returns the amount of benchmarks which have been measured.
    std::size_t result_count() const { return results.size(); }

    // Returns the result of the benchmark with the given id, or nullptr when there is none.
    const BenchResult *find_result(const std::string &p_id) const {
        for (const BenchResult &result : results) {
            if (result.id == p_id) {
                return &result;
            }
        }
        return nullptr;
    }

    // Returns the fastest benchmark of this run, or nullptr when none of them measured anything.
    // It is the one a summary is about: the rest of them are read in the blocks above it.
    const BenchResult *fastest() const {
        const BenchResult *best = nullptr;
        for (const BenchResult &result : results) {
            if (!resolvable(result)) {
                continue;
            }
            if (best == nullptr || result.typical.point < best->typical.point) {
                best = &result;
            }
        }
        return best;
    }
    /*-------------------------------------------------------------------------------*/

    private:
    // Prints the usage text of the benchmark runner.
    // NOTE: Not 'func' - the usage text is printed at runtime.
    static void print_usage(const char *p_program) {
        Console usage;
        std::printf("%sUsage:%s %s [options]\n", usage.paint(Console::bold()),
                    usage.paint(Console::reset()), p_program);
        std::printf("  %-30s Only run the benchmarks whose id contains <text>\n",
                    "--bench <text>");
        std::printf("  %-30s Same, without a separate argument\n", "--filter=<text>");
        std::printf("  %-30s List the benchmarks without measuring them\n", "--list");
        std::printf("  %-30s Warm-up per benchmark, e.g. '300ms' (default 0.3 s)\n",
                    "--warm-up-time <time>");
        std::printf("  %-30s Measurement per benchmark (default 0.5 s)\n",
                    "--measurement-time <time>");
        std::printf("  %-30s Samples per benchmark (default 100)\n", "--sample-size <count>");
        std::printf("  %-30s Resamples behind every interval (default 1000)\n",
                    "--nresamples <count>");
        std::printf("  %-30s Share of the distribution an interval covers (default 0.95)\n",
                    "--confidence-level <level>");
        std::printf("  %-30s Share of changes called noise (default 0.01)\n",
                    "--noise-threshold <fraction>");
        std::printf("  %-30s Likelihood a difference is called noise (default 0.05)\n",
                    "--significance-level <level>");
        std::printf("  %-30s Write the samples of this run into <file>\n",
                    "--save-baseline <file>");
        std::printf("  %-30s Compare this run against the samples in <file>\n",
                    "--load-baseline <file>");
        std::printf("  %-30s Compare against the baseline, report nothing else\n",
                    "--comparison-only");
        std::printf("  %-30s Warm up, measure and sample less\n", "--quick");
        std::printf("  %-30s Print every sample of every benchmark\n", "--verbose");
        std::printf("  %-30s Print only what each benchmark costs\n", "--quiet, -q");
        std::printf("  %-30s Show this text\n", "--help, -h");
    }

    // Reads a duration given in seconds, with an optional unit after it: '0.5', '500ms',
    // '3s'. Falls back to the given default whenever the text is not a duration at all, so
    // that a mistyped argument cannot quietly turn a run into a single sample.
    // NOTE: Not 'func' - the text is parsed at runtime.
    static double parse_duration(const char *p_text, const double p_fallback) {
        if (p_text == nullptr) {
            return p_fallback;
        }
        char *end = nullptr;
        const double value = std::strtod(p_text, &end);
        if (end == p_text) {
            return p_fallback;
        }
        const double scale = std::strcmp(end, "ns") == 0    ? 1e-9
                             : std::strcmp(end, "us") == 0 ? 1e-6
                             : std::strcmp(end, "ms") == 0 ? 1e-3
                                                           : 1.0;
        return value * scale;
    }

    // Reads an unsigned amount out of the given text, falling back to the given default when
    // the text is not a number at all.
    // NOTE: Not 'func' - the text is parsed at runtime.
    static unsigned long long parse_count(const char *p_text, const unsigned long long p_fallback) {
        if (p_text == nullptr) {
            return p_fallback;
        }
        char *end = nullptr;
        const unsigned long long value = std::strtoull(p_text, &end, 10);
        return end == p_text ? p_fallback : value;
    }

    // Reads a plain number out of the given text, falling back to the given default when the
    // text is not a number at all.
    // NOTE: Not 'func' - the text is parsed at runtime.
    static double parse_number(const char *p_text, const double p_fallback) {
        if (p_text == nullptr) {
            return p_fallback;
        }
        char *end = nullptr;
        const double value = std::strtod(p_text, &end);
        return end == p_text ? p_fallback : value;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Files ──────────────────────────────────────────────────────────────────────────────

    // Writes the measurements of this run into the given file, so that a later run can be
    // compared against them. The file is plain text and holds the raw samples rather than the
    // estimates: the samples are what a comparison is built out of, and a file of them can be
    // read and diffed by hand.
    // NOTE: Not 'func' - the file is written at runtime.
    bool export_baseline(const std::string &p_path) const;

    // Reads the samples of the benchmarks in the given file into 'baseline'. A file which
    // cannot be read is reported and leaves the run without a comparison.
    // NOTE: Not 'func' - the file is read at runtime.
    bool import_baseline(const std::string &p_path);

    // Writes the outcome of the finished run into 'REPORT_FILE': the environment it was
    // measured in, the configuration it was measured with, one block per benchmark, and the
    // summary of the run. The report holds no escape sequences and can therefore be diffed
    // between two runs.
    // Returns whether the report could be written.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    bool export_report() const;
    /*-------------------------------------------------------------------------------*/

    // ── Measurement ────────────────────────────────────────────────────────────────────────

    // Runs the routine of the given benchmark once, with the given amount of iterations, and
    // reports what the batch turned out to be.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    Batch run_batch(const BenchEntry &p_entry, const unsigned long long p_iterations,
                    const double p_expected_ns) const;

    // Doubles the amount of iterations until a single batch lasts long enough for its timing
    // to mean something, and returns the amount a batch of that size performs.
    //
    // A body whose work the optimizer removed entirely never gets there: doubling its
    // iteration count buys no time at all. That is worth noticing, because a benchmark whose
    // cost is zero is a benchmark which measures nothing, and it is what keeps a zero from
    // being calibrated all the way up to 'MAX_ITERATIONS'.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    unsigned long long calibrate(const BenchEntry &p_entry, double &r_batch_ns) const;

    // Returns the amount of iterations a single sample performs: as many as fit into the share
    // of the measurement time one sample is allowed to take. That is what makes 'sample_size'
    // samples take 'measurement_time' as a whole, whatever an iteration turns out to cost.
    // NOTE: Not 'func' - the amount is derived from a measurement.
    static unsigned long long plan_sample(const Config &p_config,
                                          const unsigned long long p_calibrated,
                                          const double p_batch_ns);

    // Measures one benchmark from end to end: calibration, warm-up, samples, statistics.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    void measure(const BenchEntry &p_entry, BenchResult &r_result);

    // Takes the samples of a benchmark, which is the only part of it that is ever reported.
    // NOTE: Not 'func' - benchmarks are performed at runtime.
    void collect(const BenchEntry &p_entry, BenchResult &r_result,
                 const unsigned long long p_iterations, const double p_expected_ns);

    // Turns the samples of a benchmark into every estimate which is reported for it.
    // NOTE: Not 'func' - the estimates are computed from measurements.
    static void analyze(BenchResult &r_result);

    // Compares the given result against the loaded baseline, when there is one for it.
    // NOTE: Not 'func' - the comparison is computed from measurements.
    void compare(BenchResult &r_result) const;

    // Returns whether the given result is a cost worth quoting: a benchmark which spent no
    // iterations measured nothing, and one whose batches are shorter than the calibration is
    // willing to time is below what the clock can resolve. Both are still reported.
    static bool resolvable(const BenchResult &p_result);

    // ── Reporting ──────────────────────────────────────────────────────────────────────────

    // Writes one labelled line of a block. The labels share a field wide enough for the longest
    // of them, which is what lines the brackets of every line up under one another.
    // NOTE: Not 'func' - the line is written at runtime.
    static void write_line(std::FILE *p_file, const char *p_label, const char *p_value) {
        std::fprintf(p_file, "  %-9s %s\n", p_label, p_value);
    }

    // Writes the whole block of a benchmark: its name, every estimate made out of its samples,
    // and how far it moved relative to the baseline. It is written to the console with colors
    // and to the report file without them, which is what 'p_color' decides, and a run which
    // asked for comparisons alone leaves everything but the comparison out of it.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    void write_block(std::FILE *p_file, const BenchResult &p_result, const bool p_color,
                     const bool p_only_comparison) const;

    // Prints the block of a benchmark to the console, honouring '--quiet' and '--verbose'.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    void print_result(const BenchResult &p_result) const;

    // Writes the line which counts the samples taken so far, in place. It is redrawn on a
    // terminal and dropped anywhere else, where a line per sample would be noise.
    // NOTE: Not 'func' - the line is written at runtime.
    void progress(const BenchResult &p_result, const unsigned long long p_taken,
                  const double p_elapsed_ns) const;

    // Writes the line which names the phase a benchmark is in, the way Criterion does, so
    // that a long run keeps saying what it is doing instead of going quiet.
    // NOTE: Not 'func' - the line is written at runtime.
    void phase(const std::string &p_id, const char *p_what) {
        // What the run is doing right now, on the line the progress bar takes over and the
        // result block then replaces. It is only ever that one line: a phase per benchmark would
        // say what the block underneath it says anyway, once per line, and a run has sixteen of
        // them.
        if (!console.interactive || quiet) {
            return;
        }
        console.clear_line();
        std::printf("  %-32s %s", p_id.c_str(), p_what);
        std::fflush(stdout);
    }

    // Prints the summary of the whole run: what was measured, how long it took, and how it
    // moved against the baseline when there is one.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    void print_summary() const;

    // Writes the summary of the whole run into the report file.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    void write_summary(std::FILE *p_file) const;

    // Writes what a comparison of the whole run found: '1 improved, 1 regressed, 2 of 4
    // unchanged', or the short form when nothing moved, since a run which only has one of the
    // three outcomes to say should not say all three.
    // NOTE: Not 'func' - the wording is built at runtime.
    static void format_verdicts(char *r_buffer, const std::size_t p_size,
                                const std::size_t p_improved, const std::size_t p_regressed,
                                const std::size_t p_unchanged) {
        const std::size_t total = p_improved + p_regressed + p_unchanged;
        if (p_improved == 0ull && p_regressed == 0ull) {
            std::snprintf(r_buffer, p_size, "%llu of %llu unchanged",
                          (unsigned long long)p_unchanged, (unsigned long long)total);
        } else {
            std::snprintf(r_buffer, p_size, "%llu improved, %llu regressed, %llu of %llu unchanged",
                          (unsigned long long)p_improved, (unsigned long long)p_regressed,
                          (unsigned long long)p_unchanged, (unsigned long long)total);
        }
    }

    // Counts the verdicts of the whole run, written into the three given counters.
    void tally(std::size_t &r_improved, std::size_t &r_regressed, std::size_t &r_unchanged) const {
        r_improved = 0;
        r_regressed = 0;
        r_unchanged = 0;
        for (const BenchResult &result : results) {
            if (!result.compared) {
                continue;
            }
            switch (result.verdict) {
                case Statistics::Verdict::improved: ++r_improved; break;
                case Statistics::Verdict::regressed: ++r_regressed; break;
                case Statistics::Verdict::no_change: ++r_unchanged; break;
            }
        }
    }

    // Writes one line which says that no benchmark matched the filter.
    // NOTE: Not 'func' - the line is written at runtime.
    void print_empty() const {
        // The filter is named: a run which measured nothing is nearly always a filter which was
        // spelled wrong, and the one thing which fixes that is seeing what was asked for.
        std::printf("\n  %s%-12s  %sno benchmark matched '%s'%s\n", console.paint(Console::dim()),
                    "measured", console.paint(Console::yellow()), filter.c_str(),
                    console.paint(Console::reset()));
    }
    /*-------------------------------------------------------------------------------*/

    friend class BenchmarkGroup;
};
/*-------------------------------------------------------------------------------------------------------------*/
// The measurement itself is written out below, where 'Bencher' is a complete type: the runner
// builds one per batch and hands it to the routine it is measuring.
/*-------------------------------------------------------------------------------------------------------------*/

// The Bencher type: What a benchmark body is handed to spend the iterations of a sample with.
// It owns the amount of iterations and every way of spending them, and it keeps the results
// of a routine alive so that the work is not deleted behind the runner's back.
class Bencher {
    public:
    // Returns the amount of iterations a single sample performed. It is decided by the runner
    // before the routine is called, and it is the same for every sample of a benchmark unless
    // the body decided it: a batched body counts the calls it made, and 'iter_custom' reports
    // what it spent.
    unsigned long long iterations() const {
        return performed_iterations > 0ull ? performed_iterations : iteration_count;
    }
    /*-------------------------------------------------------------------------------*/

    // Returns the current value of the monotonic clock, in nanoseconds, which is what a routine
    // driving its own iteration count times itself with.
    // NOTE: Not 'func' - the clock is read at runtime.
    static double now_ns();

    // Returns whether the benchmark drove its own iteration count. It is the mark of a
    // benchmark which cannot be split into identical iterations, and it is what the report
    // says the amount below means.
    bool drove_its_own_count() const { return drove_count; }
    /*-------------------------------------------------------------------------------*/

    // Runs the given routine once per iteration and keeps every result alive.
    //
    // The loop lives here and not in the body, so that the body does not have to know how many
    // iterations it is asked for, and so that the optimizer cannot read the amount and unroll
    // or delete the loop around it. What the routine returns is handed to the compiler barrier
    // before it is dropped: without that, a body whose result nobody uses is a body whose work
    // is gone.
    template <typename F>
    no_inline void iter(F &&p_routine) {
        for (unsigned long long index = 0; index < iteration_count; ++index) {
            run_once(p_routine);
        }
        iterated = true;
    }

    // Runs a setup and a routine per iteration ('SmallInput'), one setup per batch whose input
    // the routine then takes by value ('LargeInput'), or one setup per call of the routine
    // ('PerIteration'). Every mode is measured with its setup.
    //
    // The two modes which set the input up less often than the routine runs hand the amount of
    // routine calls back as the iteration count, which is what the sample is then divided by. It
    // is the one thing which makes their numbers comparable with a 'SmallInput' one: without it
    // a 'LargeInput' sample is reported as a single iteration which happens to cost a whole
    // batch of them.
    template <typename F, typename R>
    no_inline void iter_batched(F &&p_setup, R &&p_routine,
                                const BenchSize p_size = BenchSize::SmallInput) {
        switch (p_size) {
            case BenchSize::SmallInput:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    run_with(p_setup(), p_routine);
                }
                break;
            case BenchSize::LargeInput:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    const auto input = p_setup();
                    for (unsigned long long inner = 0; inner < iteration_count; ++inner) {
                        run_with(input, p_routine);
                    }
                }
                performed_iterations += per_input_calls();
                break;
            case BenchSize::PerIteration:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    for (unsigned long long inner = 0; inner < iteration_count; ++inner) {
                        run_with(p_setup(), p_routine);
                    }
                }
                performed_iterations += per_input_calls();
                break;
        }
        iterated = true;
    }

    // The same, with a routine which takes its input by reference and changes it as it goes.
    // A routine which keeps what it was given needs this one rather than the other.
    template <typename F, typename R>
    no_inline void iter_batched_ref(F &&p_setup, R &&p_routine,
                                    const BenchSize p_size = BenchSize::SmallInput) {
        switch (p_size) {
            case BenchSize::SmallInput:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    auto input = p_setup();
                    run_with(input, p_routine);
                }
                break;
            case BenchSize::LargeInput:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    auto input = p_setup();
                    for (unsigned long long inner = 0; inner < iteration_count; ++inner) {
                        run_with(input, p_routine);
                    }
                }
                performed_iterations += per_input_calls();
                break;
            case BenchSize::PerIteration:
                for (unsigned long long index = 0; index < iteration_count; ++index) {
                    for (unsigned long long inner = 0; inner < iteration_count; ++inner) {
                        auto input = p_setup();
                        run_with(input, p_routine);
                    }
                }
                performed_iterations += per_input_calls();
                break;
        }
        iterated = true;
    }

    // Hands the routine the amount of iterations it may run and lets it say how many it ran.
    // It keeps being called until a whole sample has taken as long as a sample is allowed to
    // take, and returns the total, which is the iteration count this sample is reported with.
    // It is the way to measure a benchmark which cannot be split into identical iterations,
    // e.g. one which spends its time in a whole computation.
    //
    // A sample which drove its own count is reported as an average, not as a line fitted
    // through its samples. A routine which spends until its time is gone holds every sample to
    // the same duration, so the amount of work it managed in that time is the only thing which
    // varies, and a line through samples of a constant duration measures nothing at all.
    template <typename F>
    unsigned long long iter_custom(F &&p_routine) {
        unsigned long long performed = 0ull;
        const double start_ns = Benchmarker::now_ns();
        double elapsed_ns = 0.0;
        do {
            performed += p_routine(iteration_count);
            elapsed_ns = Benchmarker::now_ns() - start_ns;
        } while (elapsed_ns < expected_time_ns);
        performed_iterations += performed;
        drove_count = true;
        iterated = true;
        return performed;
    }
    /*-------------------------------------------------------------------------------*/

    private:
    friend class Benchmarker;

    // Constructor, which only the runner uses.
    // NOTE: Not 'func' - a bencher is built for every single batch.
    Bencher(const unsigned long long p_iterations, const double p_expected_ns)
        : iteration_count(p_iterations), expected_time_ns(p_expected_ns) {}

    // Returns the amount of calls a batched body spent on its routine, which is the square of
    // the amount it was given whenever the input is set up once per batch rather than once per
    // call. It saturates rather than wrapping: a count which came out of an overflow would be
    // a number the report could not be believed on.
    unsigned long long per_input_calls() const {
        const unsigned long long limit = 0xFFFFFFFFull;
        if (iteration_count <= 1ull || iteration_count > limit) {
            return iteration_count;
        }
        return iteration_count * iteration_count;
    }

    // Runs one unit of work which needs nothing, and keeps whatever it produced alive. Only a
    // result which can be copied out of a register can be kept that way; a result which cannot
    // is left to the routine itself.
    template <typename F>
    always_inline static void run_once(F &&p_call) {
        if constexpr (std::is_void_v<decltype(p_call())>) {
            p_call();
        } else if constexpr (std::is_trivially_copyable_v<decltype(p_call())>) {
            const auto kept = Benchmarker::black_box(p_call());
            (void)kept;
        } else {
            p_call();
        }
    }

    // Runs one unit of work which needs its input, and keeps whatever it produced alive.
    template <typename R, typename T>
    always_inline static void run_with(T &&p_input, R &&p_routine) {
        if constexpr (std::is_void_v<decltype(p_routine(p_input))>) {
            p_routine(p_input);
        } else if constexpr (std::is_trivially_copyable_v<decltype(p_routine(p_input))>) {
            const auto kept = Benchmarker::black_box(p_routine(p_input));
            (void)kept;
        } else {
            p_routine(p_input);
        }
    }
    /*-------------------------------------------------------------------------------*/

    // The amount of iterations a single sample performs.
    unsigned long long iteration_count = 1ull;
    // How long a single sample is allowed to take, in nanoseconds. Only a routine which drives
    // its own iteration count is told about it.
    double expected_time_ns = 0.0;
    // The amount of iterations a routine which drove its own count actually spent.
    unsigned long long performed_iterations = 0ull;
    // Whether the body spent any of the iterations at all, which is worth knowing: a body
    // which measured nothing would otherwise be reported as a very fast benchmark.
    bool iterated = false;
    // Whether the benchmark decided its own amount of iterations rather than being given one.
    bool drove_count = false;
};

// ── The clock, for the routines which drive their own iteration count ───────────────────────

inline double Bencher::now_ns() {
    return Benchmarker::now_ns();
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── BenchmarkId ──────────────────────────────────────────────────────────────────────────────

inline BenchmarkId BenchmarkId::from_parameter(const char *p_name,
                                               const unsigned long long p_value) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%llu", p_value);
    return BenchmarkId{p_name, buffer};
}

inline BenchmarkId BenchmarkId::from_parameter(const char *p_name, const double p_value) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%g", p_value);
    return BenchmarkId{p_name, buffer};
}

inline BenchmarkId BenchmarkId::from_parameter(const char *p_name, const char *p_value) {
    return BenchmarkId{p_name, p_value};
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── Benchmarker: the three phases ────────────────────────────────────────────────────────────

inline Benchmarker::Batch Benchmarker::run_batch(const BenchEntry &p_entry,
                                                  const unsigned long long p_iterations,
                                                  const double p_expected_ns) const {
    Bencher bencher(p_iterations, p_expected_ns);
    clobber_memory();
    const double start_ns = now_ns();
    p_entry.routine(bencher);
    clobber_memory();

    Batch batch;
    batch.time_ns = now_ns() - start_ns;
    batch.iterations = bencher.iterations();
    batch.worked = bencher.iterated;
    batch.driven = bencher.drove_its_own_count();
    return batch;
}

inline unsigned long long Benchmarker::calibrate(const BenchEntry &p_entry,
                                                 double &r_batch_ns) const {
    unsigned long long iterations = 1ull;
    double elapsed_ns = run_batch(p_entry, iterations, 0.0).time_ns;

    // A batch of one iteration measures the loop around the work, not the work, so the count
    // is doubled until the clock has something to report. It is doubled rather than grown by a
    // factor, since every count in between is thrown away anyway.
    //
    // Two conditions end the doubling before the calibration time is reached. A batch which
    // costs nothing stays at nothing however many iterations it is given, and a batch which
    // does not get measurably slower when it is given twice the work was not timed to begin
    // with: a body whose cost does not grow with the work it was handed, e.g. one which spends
    // a fixed amount of time in a whole computation, would otherwise be doubled all the way to
    // the limit.
    //
    // The second condition only applies once a batch is long enough for its own timing to
    // carry information, which is what 'GROWTH_FLOOR_NS' is there for.
    while (elapsed_ns < CALIBRATION_TIME_NS && iterations < MAX_ITERATIONS) {
        iterations *= 2ull;
        const double previous_ns = elapsed_ns;
        elapsed_ns = run_batch(p_entry, iterations, 0.0).time_ns;

        if (elapsed_ns <= 0.0) {
            break;
        }
        if (previous_ns >= GROWTH_FLOOR_NS && elapsed_ns < previous_ns * GROWTH_TOLERANCE) {
            break;
        }
    }

    r_batch_ns = elapsed_ns;
    return iterations;
}

inline unsigned long long Benchmarker::plan_sample(const Config &p_config,
                                                   const unsigned long long p_calibrated,
                                                   const double p_batch_ns) {
    if (p_batch_ns <= 0.0 || p_config.sample_size == 0ull) {
        return p_calibrated;
    }
    const double share_ns =
        p_config.measurement_time * 1000000000.0 / static_cast<double>(p_config.sample_size);
    const double planned = static_cast<double>(p_calibrated) * share_ns / p_batch_ns;
    if (planned < 1.0) {
        return 1ull;
    }
    return planned >= static_cast<double>(MAX_ITERATIONS)
               ? MAX_ITERATIONS
               : static_cast<unsigned long long>(planned);
}

inline bool Benchmarker::resolvable(const BenchResult &p_result) {
    // A benchmark which spent no iterations measured nothing, and one whose batches are shorter
    // than the calibration is willing to time is below what the clock can resolve. Both are
    // reported, and neither is put forward as the cost of a benchmark.
    return p_result.measured && p_result.resolvable && p_result.typical.point > 0.0;
}

inline void Benchmarker::measure(const BenchEntry &p_entry, BenchResult &r_result) {
    const double start_ns = now_ns();
    const Config &settings = p_entry.config;
    char buffer[96];

    // The amount of iterations a sample performs is decided first, since everything after this
    // is counted in samples rather than in iterations.
    double batch_ns = 0.0;
    const unsigned long long calibrated = calibrate(p_entry, batch_ns);
    const unsigned long long per_sample = plan_sample(settings, calibrated, batch_ns);
    const double share_ns =
        settings.sample_size > 0ull
            ? settings.measurement_time * 1000000000.0 / static_cast<double>(settings.sample_size)
            : 0.0;
    r_result.iterations = per_sample;

    if (!quiet) {
        std::snprintf(buffer, sizeof(buffer), "warming up for %.3g s", settings.warm_up_time);
        phase(p_entry.id, buffer);
    }

    // The warm-up runs exactly the batches the samples will be, which is what keeps the setup
    // of a body (an opaque operand, a container to fill) in the state every later sample is
    // taken in. Only the time it takes is bounded.
    double warm_ns = run_batch(p_entry, per_sample, share_ns).time_ns;
    while (warm_ns < settings.warm_up_time * 1000000000.0) {
        warm_ns += run_batch(p_entry, per_sample, share_ns).time_ns;
    }

    if (!quiet) {
        std::snprintf(buffer, sizeof(buffer), "collecting %llu samples in %.3g s",
                      settings.sample_size, settings.measurement_time);
        phase(p_entry.id, buffer);
    }

    collect(p_entry, r_result, per_sample, share_ns);

    if (!r_result.measured) {
        std::printf("  %s%s%s spent no iterations, there was nothing to measure\n",
                    console.paint(Console::yellow()), p_entry.id.c_str(),
                    console.paint(Console::reset()));
    } else if (!r_result.resolvable) {
        // A batch shorter than this is mostly the overhead of reading the clock, so the
        // number below says how long the work took to within the resolution of the machine
        // rather than what it costs.
        Console::format_duration(buffer, sizeof(buffer), r_result.typical.point);
        std::printf("  %s%s%s is faster than the clock resolves: %s per iteration\n",
                    console.paint(Console::yellow()), p_entry.id.c_str(),
                    console.paint(Console::reset()), buffer);
    }

    analyze(r_result);
    r_result.total_ns = now_ns() - start_ns;
}

inline void Benchmarker::collect(const BenchEntry &p_entry, BenchResult &r_result,
                                 const unsigned long long p_iterations,
                                 const double p_expected_ns) {
    const Config &settings = p_entry.config;
    double measured_ns = 0.0;
    const double budget_ns = settings.measurement_time * 1000000000.0;

    // The collection stops as soon as the measurement time is spent, which by construction it
    // is not before the last sample: the iteration count was derived from it. It does fire
    // when a sample turned out slower than the calibration predicted, and a handful of
    // samples is the floor for an interval around them to mean anything.
    while (r_result.samples.size() < settings.sample_size &&
           (r_result.samples.size() < MINIMUM_SAMPLES || measured_ns < budget_ns)) {
        const Batch batch = run_batch(p_entry, p_iterations, p_expected_ns);
        const unsigned long long performed = batch.iterations > 0ull ? batch.iterations : p_iterations;

        r_result.measured = true;
        r_result.sample_time_ns.push_back(batch.time_ns);
        r_result.samples.push_back(batch.time_ns / static_cast<double>(performed));
        // A benchmark which drove its own count only does so in some of its samples at most,
        // and saying that it did when a single sample says so is the most a batch can know.
        r_result.driven = r_result.driven || batch.driven;
        measured_ns += batch.time_ns;

        progress(r_result, r_result.samples.size(), measured_ns);
    }

    // Whether the samples are worth quoting at all is decided by how long they took, not by how
    // long the calibration took: a sample is planned to fill its share of the measurement time,
    // and a body whose work the optimizer removed leaves every one of them empty.
    r_result.resolvable =
        !r_result.samples.empty() && measured_ns / static_cast<double>(r_result.samples.size()) >=
                                         MINIMUM_BATCH_NS;

    console.clear_line();
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── Benchmarker: statistics ─────────────────────────────────────────────────────────────────

inline void Benchmarker::analyze(BenchResult &p_result) {
    if (!p_result.measured) {
        return;
    }
    const Config &settings = p_result.config;

    p_result.outliers = Statistics::outliers(p_result.samples);
    p_result.worst = Statistics::worst_sample(p_result.samples);

    p_result.mean =
        Statistics::estimate(p_result.samples, &Statistics::mean, settings.confidence_level,
                             settings.nresamples);
    p_result.median =
        Statistics::estimate(p_result.samples, &Statistics::median, settings.confidence_level,
                             settings.nresamples);
    p_result.deviation = Statistics::deviation(p_result.samples);
    p_result.absolute_deviation = Statistics::absolute_deviation(p_result.samples);

    // The cost of an iteration is the average of the samples, which is the whole time of every
    // sample divided by the whole amount of work every sample did.
    //
    // NOTE: It is not a line fitted through the samples. A benchmark which drove its own
    // iteration count spends until its sample is over whatever it managed to do in that time,
    // so its samples are all of the same length and a line through them has nothing to fit.
    p_result.typical = p_result.mean;
}

inline void Benchmarker::compare(BenchResult &r_result) const {
    if (baseline.empty() || !r_result.measured) {
        return;
    }
    const auto entry = baseline.find(r_result.id);
    if (entry == baseline.end() || entry->second.empty()) {
        return;
    }

    r_result.comparison = Statistics::compare(r_result.samples, entry->second,
                                              r_result.config.noise_threshold,
                                              r_result.config.significance_level,
                                              r_result.config.nresamples,
                                              r_result.config.confidence_level);
    r_result.verdict = Statistics::verdict(r_result.comparison, r_result.config.noise_threshold);
    r_result.compared = true;
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── Benchmarker: reporting ───────────────────────────────────────────────────────────────────

inline void Benchmarker::write_block(std::FILE *p_file, const BenchResult &p_result,
                                     const bool p_color, const bool p_only_comparison) const {
    // A benchmark with nothing to compare against has nothing to say in a run which was asked
    // for comparisons alone, and it is measured all the same, which is the price of a
    // comparison.
    if (p_only_comparison && !p_result.compared) {
        return;
    }
    // The report file holds the same block without the escape sequences, which is the only
    // difference between the two.
    const char *bold = p_color ? Console::bold() : "";
    const char *dim = p_color ? Console::dim() : "";
    const char *green = p_color ? Console::green() : "";
    const char *red = p_color ? Console::red() : "";
    const char *yellow = p_color ? Console::yellow() : "";
    const char *reset = p_color ? Console::reset() : "";
    char value[96];
    char lower[64];
    char upper[64];

    std::fprintf(p_file, "%s%s%s\n", bold, p_result.id.c_str(), reset);

    if (!p_only_comparison) {
        if (!p_result.measured) {
            write_line(p_file, "time:", "not measured");
        } else if (!p_result.resolvable) {
            write_line(p_file, "time:", "faster than the clock resolves");
        } else {
            const double amount = static_cast<double>(p_result.config.throughput.amount);

            // The estimate and the amount of work it was taken over share a line. The iteration
            // count is what the estimate is per, and a run compared against another one is only
            // readable when both of them are in the same place.
            Console::format_duration_interval(value, sizeof(value), p_result.typical.lower,
                                              p_result.typical.point, p_result.typical.upper);
            Console::format_count(lower, sizeof(lower), p_result.iterations);
            std::fprintf(p_file, "  %-9s %s  %s%s iters%s%s\n", "time:", value, p_result.driven ? dim : "",
                         lower, p_result.driven ? " (body-driven)" : "", reset);

            Console::format_throughput_interval(
                value, sizeof(value),
                p_result.typical.lower > 0.0 ? amount * 1000000000.0 / p_result.typical.lower : 0.0,
                amount * 1000000000.0 / p_result.typical.point,
                p_result.typical.upper > 0.0 ? amount * 1000000000.0 / p_result.typical.upper : 0.0,
                p_result.config.throughput.unit);
            write_line(p_file, "thrpt:", value);

            // The middle of the samples, then how far they sit around the average and around the
            // middle. Both spreads are point values: an interval around a standard deviation is
            // a statement about the samples rather than about the work in them, and the interval
            // around the average is the line above.
            Console::format_duration_interval(value, sizeof(value), p_result.median.lower,
                                              p_result.median.point, p_result.median.upper);
            Console::format_duration_precise(lower, sizeof(lower), p_result.deviation);
            Console::format_duration_precise(upper, sizeof(upper), p_result.absolute_deviation);
            std::fprintf(p_file, "  %-9s %s  sd %s  mad %s\n", "median:", value, lower, upper);

            // Every sample is written when the run asked to see them. Without that, the outliers
            // are one line: how many of them there were, and the one which reached furthest out
            // of the distribution. Which sample it was says nothing the samples do not say
            // better, and listing them costs a line each on a noisy machine.
            if (verbose) {
                for (std::size_t index = 0; index < p_result.samples.size(); ++index) {
                    const bool outlier = std::find(p_result.outliers.begin(), p_result.outliers.end(),
                                                   index) != p_result.outliers.end();
                    char label[48];
                    std::snprintf(label, sizeof(label), "sample %zu", index + 1);
                    Console::format_duration_precise(value, sizeof(value), p_result.samples[index]);
                    // The marker is two columns wide either way, so the values below stay in one
                    // column whether the sample belongs to the distribution or not.
                    std::fprintf(p_file, "  %-9s %s%s%s%s\n", label, outlier ? red : dim,
                                 outlier ? "! " : "  ", value, reset);
                }
            }

            if (!p_result.outliers.empty()) {
                Console::format_duration_precise(value, sizeof(value),
                                                 p_result.samples[p_result.worst]);
                std::fprintf(p_file, "  %-9s %s%llu of %llu, worst %s%s\n", "outliers:", dim,
                             (unsigned long long)p_result.outliers.size(),
                             (unsigned long long)p_result.samples.size(), value, reset);
            }
        }
    }

    if (p_result.compared) {
        Console::format_change(lower, sizeof(lower), p_result.comparison.change.lower);
        Console::format_change(value, sizeof(value), p_result.comparison.change.point);
        Console::format_change(upper, sizeof(upper), p_result.comparison.change.upper);
        std::fprintf(p_file, "  %-9s [%s %s %s]", "change:", lower, value, upper);

        // The probability is the whole of the parenthesis, and the significance level it is held
        // against is in the header of the report rather than on every line of it. What is worth
        // saying here is the one case where the two facts look like they disagree: a difference
        // the samples do separate, which is still not a difference because it is smaller than
        // the run's own noise. 'No change' next to a probability of 0.0000 reads as a broken
        // comparison unless the line says why.
        Console::format_probability(value, sizeof(value), p_result.comparison.p_value);
        if (p_result.comparison.p_value < p_result.config.significance_level &&
            !p_result.comparison.significant) {
            std::fprintf(p_file, "  (p = %s, under %.1f%% noise)\n", value,
                         p_result.config.noise_threshold * 100.0);
        } else {
            std::fprintf(p_file, "  (p = %s)\n", value);
        }

        // The verdict is what a reader takes away from a comparison, so it says the change in
        // words and puts the two costs next to each other.
        const char *verdict = "No change in performance";
        const char *color = yellow;
        if (p_result.verdict == Statistics::Verdict::improved) {
            verdict = "Performance has improved";
            color = green;
        } else if (p_result.verdict == Statistics::Verdict::regressed) {
            verdict = "Performance has regressed";
            color = red;
        }

        char current[64];
        char previous[64];
        Console::format_duration_precise(current, sizeof(current), p_result.typical.point);
        const auto entry = baseline.find(p_result.id);
        if (entry == baseline.end() || entry->second.empty()) {
            std::fprintf(p_file, "  %s%s.%s\n", color, verdict, reset);
            return;
        }
        Console::format_duration_precise(previous, sizeof(previous),
                                         Statistics::mean(entry->second));
        std::fprintf(p_file, "  %s%s.%s  %s vs %s\n", color, verdict, reset, current, previous);
    }
}

inline void Benchmarker::print_result(const BenchResult &p_result) const {
    console.clear_line();
    if (comparison_only) {
        // A run which only compares prints the comparison and nothing else: it is the one to
        // put in a diff, where an estimate which moved is noise in the comparison itself.
        write_block(stdout, p_result, console.color, true);
        return;
    }
    if (quiet) {
        // The quiet run is one line per benchmark: what it costs, and nothing else.
        char time[64];
        char rate[64];
        Console::format_duration_precise(time, sizeof(time), p_result.typical.point);
        Console::format_rate_of(rate, sizeof(rate), p_result.typical.point,
                                p_result.config.throughput.amount, p_result.config.throughput.unit);
        std::printf("  %-40s %14s  %s\n", p_result.id.c_str(), time, rate);
        return;
    }
    write_block(stdout, p_result, console.color, false);
}

inline void Benchmarker::progress(const BenchResult &p_result, const unsigned long long p_taken,
                                  const double p_elapsed_ns) const {
    if (!console.interactive || quiet || !p_result.measured) {
        return;
    }
    // A bar of twenty cells, redrawn in place. It is the only progress a run ever showed, and
    // the only one which does not become a line per sample once it is written to a log.
    const double fraction =
        p_result.config.sample_size > 0ull
            ? static_cast<double>(p_taken) / static_cast<double>(p_result.config.sample_size)
            : 0.0;
    char bar[21];
    for (int index = 0; index < 20; ++index) {
        bar[index] = static_cast<double>(index) < fraction * 20.0 ? '#' : '.';
    }
    bar[20] = '\0';

    char buffer[32];
    Console::format_duration(buffer, sizeof(buffer), p_elapsed_ns);
    console.clear_line();
    std::printf("  %-32s [%s] %3llu%%  %s", p_result.id.c_str(), bar,
                (unsigned long long)(fraction * 100.0), buffer);
    std::fflush(stdout);
}

inline void Benchmarker::print_summary() const {
    char buffer[96];
    Console::format_duration(buffer, sizeof(buffer), total_ns);

    std::printf("\n");
    console.rule(REPORT_WIDTH);
    std::printf("  %llu %s%s%llu %s%s%s\n", matched, Console::plural(matched, "benchmark", "benchmarks"),
                console.separator(), config.sample_size,
                Console::plural(config.sample_size, "sample", "samples"), console.separator(),
                buffer);

    // What the run has to say for itself, in the order it is read: the verdicts first, since
    // they are what a run of this shape is for, then the fastest, and where both of them and
    // the samples ended up.
    if (!load_path.empty()) {
        std::size_t improved = 0, regressed = 0, unchanged = 0;
        tally(improved, regressed, unchanged);
        char verdicts[96];
        format_verdicts(verdicts, sizeof(verdicts), improved, regressed, unchanged);
        std::printf("  %s%-12s  %s  %s\n", console.paint(Console::dim()), "against",
                    load_path.c_str(), verdicts);
    }

    // The fastest of a run is an estimate rather than a comparison, so a run which was asked
    // for comparisons alone leaves it out.
    const BenchResult *best = comparison_only ? nullptr : fastest();
    if (best != nullptr) {
        char time[64];
        char rate[64];
        Console::format_duration_precise(time, sizeof(time), best->typical.point);
        Console::format_rate_of(rate, sizeof(rate), best->typical.point,
                                best->config.throughput.amount, best->config.throughput.unit);
        std::printf("  %s%-12s  %s%s%s  %s  %s%s%s\n", console.paint(Console::dim()), "fastest",
                    console.paint(Console::bold()), best->id.c_str(), console.paint(Console::reset()),
                    time, console.paint(Console::green()), rate, console.paint(Console::reset()));
    }
}

inline void Benchmarker::write_summary(std::FILE *p_file) const {
    char buffer[96];
    Console::format_duration(buffer, sizeof(buffer), total_ns);

    // A run which measured nothing has no samples to report having taken, so it does not claim
    // the hundred of them it was configured for.
    if (matched == 0ull) {
        std::fprintf(p_file, "\n0 benchmarks  %s\n", buffer);
        return;
    }
    std::fprintf(p_file, "\n%llu %s, %llu %s each, %s total\n", matched,
                 Console::plural(matched, "benchmark", "benchmarks"), config.sample_size,
                 Console::plural(config.sample_size, "sample", "samples"), buffer);

    // The fastest of a run is an estimate rather than a comparison, so a run which was asked
    // for comparisons alone leaves it out.
    const BenchResult *best = comparison_only ? nullptr : fastest();
    if (best != nullptr) {
        char time[64];
        char rate[64];
        Console::format_duration_precise(time, sizeof(time), best->typical.point);
        Console::format_rate_of(rate, sizeof(rate), best->typical.point,
                                best->config.throughput.amount, best->config.throughput.unit);
        std::fprintf(p_file, "fastest: %s  %s/iter  %s\n", best->id.c_str(), time, rate);
    }

    if (!load_path.empty()) {
        std::size_t improved = 0, regressed = 0, unchanged = 0;
        tally(improved, regressed, unchanged);
        char verdicts[96];
        format_verdicts(verdicts, sizeof(verdicts), improved, regressed, unchanged);
        std::fprintf(p_file, "against %s  %s\n", load_path.c_str(), verdicts);
    }
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── Benchmarker: the baseline files ──────────────────────────────────────────────────────────

inline bool Benchmarker::export_baseline(const std::string &p_path) const {
    std::FILE *file = std::fopen(p_path.c_str(), "w");
    if (file == nullptr) {
        std::printf("  %s%scould not write %s%s\n", console.paint(Console::red()),
                    console.paint(Console::bold()), p_path.c_str(), console.paint(Console::reset()));
        return false;
    }

    char stamp[32];
    Console::format_timestamp(stamp, sizeof(stamp));
    std::fprintf(file, "# RatLab benchmark baseline\n");
    std::fprintf(file, "# date: %s\n", stamp);
    std::fprintf(file, "# platform: %s\n", Console::platform_name());
    std::fprintf(file, "# compiler: %s\n", Console::compiler_name());
    std::fprintf(file, "# standard: %s\n", Console::cpp_standard_name());

    // The raw samples are what is stored, one benchmark after the other, because a comparison
    // is built out of them and because a file of them can be read and diffed by hand.
    for (const BenchResult &result : results) {
        if (!result.measured) {
            continue;
        }
        std::fprintf(file, "\nbenchmark %s\n", result.id.c_str());
        std::fprintf(file, "iterations %llu\n", result.iterations);
        std::fprintf(file, "samples %llu\n", (unsigned long long)result.samples.size());
        std::fprintf(file, "time");
        for (std::size_t index = 0; index < result.samples.size(); ++index) {
            std::fprintf(file, "%s%.6f", index % 8ull == 0ull ? "\n    " : " ", result.samples[index]);
        }
        std::fprintf(file, "\n");
    }

    std::fclose(file);
    std::printf("  %s%-12s  %s%s%s\n", console.paint(Console::dim()), "baseline",
                console.paint(Console::bold()), p_path.c_str(), console.paint(Console::reset()));
    return true;
}

inline bool Benchmarker::import_baseline(const std::string &p_path) {
    std::FILE *file = std::fopen(p_path.c_str(), "r");
    if (file == nullptr) {
        std::printf("  %s%scould not read %s%s\n", console.paint(Console::red()),
                    console.paint(Console::bold()), p_path.c_str(), console.paint(Console::reset()));
        return false;
    }

    // The file is line based: a block per benchmark, a line of samples, and as many
    // continuations of it as it takes. Anything which does not parse is skipped rather than
    // guessed at, so that a file written by another version cannot be read as something it is
    // not.
    char line[2048];
    std::string id;
    while (std::fgets(line, sizeof(line), file) != nullptr) {
        const char *cursor = line;
        while (*cursor == ' ' || *cursor == '\t') {
            ++cursor;
        }

        if (*cursor == '#' || *cursor == '\n' || *cursor == '\0') {
            continue;
        }

        if (std::strncmp(cursor, "benchmark ", 10) == 0) {
            char name[512];
            if (std::sscanf(cursor + 10, "%511s", name) == 1) {
                id = name;
                baseline[id] = std::vector<double>();
            }
            continue;
        }
        if (id.empty() || std::strncmp(cursor, "iterations ", 11) == 0 ||
            std::strncmp(cursor, "samples ", 8) == 0) {
            continue;
        }
        if (std::strncmp(cursor, "time", 4) == 0) {
            cursor += 4;
        }

        // Whatever is left of the line is a run of samples, whether it starts the list or
        // continues it.
        double value = 0.0;
        int consumed = 0;
        while (std::sscanf(cursor, "%lf%n", &value, &consumed) == 1 && consumed > 0) {
            baseline[id].push_back(value);
            cursor += consumed;
        }
    }

    std::fclose(file);
    return true;
}

inline bool Benchmarker::export_report() const {
    if (!Console::ensure_directory(REPORT_DIRECTORY)) {
        std::printf("  %s%scould not create %s%s\n", console.paint(Console::red()),
                    console.paint(Console::bold()), REPORT_DIRECTORY, console.paint(Console::reset()));
        return false;
    }

    std::FILE *file = std::fopen(REPORT_FILE, "w");
    if (file == nullptr) {
        std::printf("  %s%scould not write %s%s\n", console.paint(Console::red()),
                    console.paint(Console::bold()), REPORT_FILE, console.paint(Console::reset()));
        return false;
    }

    char stamp[32];
    Console::format_timestamp(stamp, sizeof(stamp));

    if (results.empty()) {
        // A run which measured nothing has no configuration worth recording, and a report whose
        // numbers say nothing is worse than no report at all.
        std::fprintf(file, "RatLab benchmarks\n================\n\nno benchmark matched '%s'\n",
                     filter.c_str());
    } else {
        // Four lines of context, one fact each: what was run, how it was measured, what a
        // difference has to beat to be called one, and what it was measured against. A report is
        // read long after the run it came from, and every one of these is a value the numbers
        // below cannot be read without.
        std::fprintf(file, "RatLab benchmarks\n");
        std::fprintf(file, "================\n\n");
        std::fprintf(file, "%s  %s  %s  %s\n", stamp, Console::platform_name(),
                     Console::compiler_name(), Console::cpp_standard_name());
        std::fprintf(file, "filter %s  %.3g s warm-up  %.3g s measured  %llu samples each\n",
                     filter.empty() ? "(none)" : filter.c_str(), config.warm_up_time,
                     config.measurement_time, config.sample_size);
        std::fprintf(file, "%.0f%% intervals over %llu resamples  %.1f%% noise  significance %.3f\n",
                     config.confidence_level * 100.0, config.nresamples,
                     config.noise_threshold * 100.0, config.significance_level);
        std::fprintf(file, "baseline %s\n\n", load_path.empty() ? "(none)" : load_path.c_str());

        for (const BenchResult &result : results) {
            write_block(file, result, false, comparison_only);
            std::fprintf(file, "\n");
        }
    }

    write_summary(file);
    std::fclose(file);

    std::printf("  %s%-12s  %s%s%s\n", console.paint(Console::dim()), "report",
                console.paint(Console::bold()), REPORT_FILE, console.paint(Console::reset()));
    return true;
}
/*-------------------------------------------------------------------------------------------------------------*/

// ── Benchmarker: the run itself ──────────────────────────────────────────────────────────────

inline Benchmarker &Benchmarker::configure_from_args(const int p_argc, char **p_argv) {
    program = p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_benchmarks";

    // '--quick' is a set of defaults and not an override: it is applied before the arguments
    // are read, so that a number spelled out next to it wins. A flag which silently discarded
    // the arguments beside it is a flag nobody trusts, and '--quick --noise-threshold 0.1'
    // reads as a short run with a wider noise rather than as a short run.
    bool shorten = false;
    for (int index = 1; index < p_argc && !shorten; ++index) {
        shorten = p_argv[index] != nullptr && std::strcmp(p_argv[index], "--quick") == 0;
    }
    if (shorten) {
        config = Config::quick();
    }
    for (int index = 1; index < p_argc; ++index) {
        const char *argument = p_argv[index] == nullptr ? "" : p_argv[index];

        // '--name=value' and '--name value' both carry a value, and only the first of them can
        // carry one which starts with a dash.
        char name[64];
        const char *inline_value = nullptr;
        const char *equals = std::strchr(argument, '=');
        if (equals != nullptr) {
            const std::size_t length = static_cast<std::size_t>(equals - argument);
            const std::size_t written = length < sizeof(name) - 1ull ? length : sizeof(name) - 1ull;
            std::memcpy(name, argument, written);
            name[written] = '\0';
            inline_value = equals + 1;
        } else {
            std::snprintf(name, sizeof(name), "%s", argument);
        }

        // Returns the value of the argument, whether it came with it or is the next one.
        const auto take = [&]() -> const char * {
            if (inline_value != nullptr) {
                return inline_value;
            }
            return index + 1 < p_argc && p_argv[index + 1] != nullptr ? p_argv[++index] : nullptr;
        };

        if (std::strcmp(name, "--bench") == 0 || std::strcmp(name, "--filter") == 0) {
            const char *text = take();
            filter = text == nullptr ? std::string() : std::string(text);
        } else if (std::strcmp(name, "--list") == 0) {
            mode = Mode::list;
        } else if (std::strcmp(name, "--warm-up-time") == 0) {
            config.warm_up_time = parse_duration(take(), config.warm_up_time);
        } else if (std::strcmp(name, "--measurement-time") == 0) {
            config.measurement_time = parse_duration(take(), config.measurement_time);
        } else if (std::strcmp(name, "--sample-size") == 0) {
            config.sample_size = parse_count(take(), config.sample_size);
        } else if (std::strcmp(name, "--nresamples") == 0) {
            config.nresamples = parse_count(take(), config.nresamples);
        } else if (std::strcmp(name, "--confidence-level") == 0) {
            config.confidence_level = parse_number(take(), config.confidence_level);
        } else if (std::strcmp(name, "--noise-threshold") == 0) {
            config.noise_threshold = parse_number(take(), config.noise_threshold);
        } else if (std::strcmp(name, "--significance-level") == 0) {
            config.significance_level = parse_number(take(), config.significance_level);
        } else if (std::strcmp(name, "--save-baseline") == 0) {
            const char *text = take();
            save_path = text == nullptr ? std::string() : std::string(text);
        } else if (std::strcmp(name, "--load-baseline") == 0) {
            const char *text = take();
            load_path = text == nullptr ? std::string() : std::string(text);
        } else if (std::strcmp(name, "--comparison-only") == 0) {
            comparison_only = true;
        } else if (std::strcmp(name, "--quick") == 0) {
            // Already applied above, before any other argument could be read.
        } else if (std::strcmp(name, "--verbose") == 0) {
            verbose = true;
        } else if (std::strcmp(name, "--quiet") == 0 || std::strcmp(name, "-q") == 0) {
            quiet = true;
        } else if (std::strcmp(name, "--help") == 0 || std::strcmp(name, "-h") == 0) {
            mode = Mode::help;
        } else {
            std::printf("  %sunknown argument%s: %s\n", console.paint(Console::red()),
                        console.paint(Console::reset()), argument);
            mode = Mode::failed;
        }
    }

    return *this;
}

inline int Benchmarker::run(void (*p_group)(Benchmarker &)) {
    if (mode == Mode::help) {
        print_usage(program.c_str());
        return 0;
    }
    if (mode == Mode::failed) {
        print_usage(program.c_str());
        return 1;
    }
    if (!load_path.empty() && !import_baseline(load_path)) {
        return 1;
    }

    if (mode == Mode::list) {
        // The group is run to find out what it holds, which is the price of a framework which
        // registers its benchmarks at runtime: nothing is measured to list them.
        p_group(*this);
        std::printf("\n%llu %s%s%s\n", matched, Console::plural(matched, "benchmark", "benchmarks"),
                    filter.empty() ? "" : " matching ", filter.c_str());
        return matched > 0ull ? 0 : 1;
    }

    char environment[256];
    std::snprintf(environment, sizeof(environment), "%s%s%s%s%s%s%s%llu %s", console.separator(),
                  Console::platform_name(), console.separator(), Console::compiler_name(),
                  console.separator(), Console::cpp_standard_name(), console.separator(),
                  config.sample_size,
                  Console::plural(config.sample_size, "sample", "samples"));
    std::printf("%s%sRatLab benchmarks%s  %s\n", console.paint(Console::bold()),
                console.paint(Console::cyan()), console.paint(Console::reset()), environment);

    const double start_ns = now_ns();
    p_group(*this);
    total_ns = now_ns() - start_ns;

    if (matched == 0ull) {
        print_empty();
    } else {
        print_summary();
    }

    // A run which was asked for comparisons alone and got none has said nothing at all, which
    // is worth a line: the usual reason is a baseline taken before the benchmarks were named.
    if (comparison_only && !results.empty()) {
        std::size_t compared = 0ull;
        for (const BenchResult &result : results) {
            if (result.compared) {
                ++compared;
            }
        }
        if (compared == 0ull) {
            std::printf("  %s%-12s  %sno benchmark could be compared%s\n",
                        console.paint(Console::dim()), "against",
                        console.paint(Console::yellow()), console.paint(Console::reset()));
        }
    }

    if (!save_path.empty()) {
        export_baseline(save_path);
    }
    export_report();
    return matched > 0ull ? 0 : 1;
}
/*-------------------------------------------------------------------------------------------------------------*/

// The BenchmarkGroup type: A set of benchmarks which share an id prefix and a configuration.
// It is Criterion's group, and it is what keeps a family of ids short: everything inside a
// group named 'u8' is reported as 'u8/<name>'.
class BenchmarkGroup {
    public:
    // Registers a benchmark which is run without an input and measures it right away.
    template <typename F>
    void bench_function(const char *p_name, F &&p_routine) {
        p_benchmarker->bench_entry(BenchmarkId::plain(name(p_name).c_str()),
                                   std::forward<F>(p_routine), config);
    }

    // Registers a benchmark which is run with the given input and measures it right away. The
    // input has to outlive the call.
    template <typename T, typename F>
    void bench_with_input(const char *p_name, const T &p_input, F &&p_routine) {
        p_benchmarker->bench_with_input(name(p_name).c_str(), p_input, std::forward<F>(p_routine));
    }

    // Returns a group nested inside this one, which is what keeps a family of benchmarks apart
    // from the family it belongs to.
    BenchmarkGroup benchmark_group(const char *p_id);

    // Sets what a single iteration of this group produces, which is the unit its throughput is
    // reported in.
    void throughput(const Throughput &p_throughput) { config.throughput = p_throughput; }
    // Sets how long the samples of this group are collected, in seconds.
    void measurement_time(const double p_seconds) { config.measurement_time = p_seconds; }
    // Sets how long the benchmarks of this group are warmed up, in seconds.
    void warm_up_time(const double p_seconds) { config.warm_up_time = p_seconds; }
    // Sets how many samples the benchmarks of this group are measured with.
    void sample_size(const unsigned long long p_samples) { config.sample_size = p_samples; }

    // Ends the group. There is nothing to end: the report belongs to the run as a whole, and
    // every benchmark of this group is in it by the time this is called.
    void finish() {}
    /*-------------------------------------------------------------------------------*/

    private:
    friend class Benchmarker;

    // Constructor, which only the runner and another group use.
    BenchmarkGroup(Benchmarker &p_benchmarker, const std::string &p_prefix,
                    const Benchmarker::Config &p_config)
        : p_benchmarker(&p_benchmarker), prefix(p_prefix), config(p_config) {}

    // Returns the full name of a benchmark of this group.
    std::string name(const char *p_name) const {
        return prefix.empty() ? std::string(p_name) : prefix + "/" + p_name;
    }

    // The runner the benchmarks of this group are registered into.
    Benchmarker *p_benchmarker = nullptr;
    // The id every benchmark of this group is reported under.
    std::string prefix;
    // How the benchmarks of this group are measured.
    Benchmarker::Config config;
};

inline BenchmarkGroup Benchmarker::benchmark_group(const char *p_id) {
    return BenchmarkGroup(*this, p_id, config);
}

inline BenchmarkGroup BenchmarkGroup::benchmark_group(const char *p_id) {
    return BenchmarkGroup(*p_benchmarker, name(p_id), config);
}
/*-------------------------------------------------------------------------------------------------------------*/

// Declares the group of benchmarks a run is made of, with one group function per argument:
//
//     BENCHMARK_GROUP(ratlab, bench_u8, bench_u16)
//
// This is Criterion's 'criterion_group!': a benchmark file becomes part of a run by being
// named here, once. Nothing else about it has to change.
#define BENCHMARK_GROUP(p_group, ...)                                                                   \
    static void p_group##_benchmarks(Benchmarker &p_benchmarker) {                                    \
        static void (*const p_functions[])(Benchmarker &) = {__VA_ARGS__};                             \
        for (std::size_t p_index = 0; p_index < sizeof(p_functions) / sizeof(p_functions[0]);           \
             ++p_index) {                                                                                \
            p_functions[p_index](p_benchmarker);                                                        \
        }                                                                                               \
    }

// Writes the entry point of a benchmark run, for the group declared above it:
//
//     BENCHMARK_MAIN(ratlab)
//
// This is Criterion's 'criterion_main!'. Everything after the program name is handed to the
// runner unchanged, which is how a run is filtered, shortened or compared from a shell.
#define BENCHMARK_MAIN(p_group)                                                                        \
    int main(const int p_argc, char **p_argv) {                                                        \
        return Benchmarker::run(p_argc, p_argv, p_group##_benchmarks);                                \
    }
