/************************************/
/*           tester.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(_WIN32)
    #include <io.h>
    #define TESTER_ISATTY(stream) (_isatty(_fileno(stream)) != 0)
#else
    #include <unistd.h>
    #define TESTER_ISATTY(stream) (::isatty(::fileno(stream)) != 0)
#endif

// The Tester type: A self-contained, dependency-free test framework for RatLab.
// Test bodies are registered at runtime (or at static-initialization time) and are executed
// on demand by 'run', either from 'main' or from any other runtime entry point.
// Every check made inside a test body is counted, and a non-zero exit code is returned
// whenever at least one check fails.
class Tester {
    public:
    // The signature every registered test body has to follow.
    // The body receives the Tester instance which collects and reports the checks.
    typedef void (*TestBody)(Tester &p_tester);
    /*---------------------------------------------------------------------------------------*/

    private:
    // A single registered test case, waiting to be executed at runtime.
    struct TestCase {
        // The name of the test case, shown in the report. (Must outlive the registration.)
        const char *name;
        // The body which performs the actual checks.
        TestBody body;
    };

    // The shared registry of every test case known to the workspace.
    // A function-local static is used so that registration is safe to perform from any
    // translation unit, no matter in which order the static-initializers happen to run.
    static std::vector<TestCase> &registry() {
        static std::vector<TestCase> cases;
        return cases;
    }

    // Totals across every executed test case.
    unsigned long long check_total = 0;
    unsigned long long pass_count = 0;
    unsigned long long fail_count = 0;
    // Totals of the test case which is currently being executed.
    unsigned long long case_check_total = 0;
    unsigned long long case_fail_count = 0;
    // The name of the test case which is currently being executed.
    const char *case_name = "";
    // The number of test cases which have been executed in this run.
    unsigned long long case_total = 0;
    // Whether ANSI color codes are written to the output.
    bool use_color = false;
    // Whether the progress-dot line is still open (no newline printed yet).
    bool line_open = false;
    /*-------------------------------------------------------------------------------*/

    // ── Colors ──────────────────────────────────────────────────────────────────────────────

    func static const char *green() { return "\x1b[32m"; }
    func static const char *red() { return "\x1b[31m"; }
    func static const char *yellow() { return "\x1b[33m"; }
    func static const char *cyan() { return "\x1b[36m"; }
    func static const char *bold() { return "\x1b[1m"; }
    func static const char *reset() { return "\x1b[0m"; }

    // Colors are stripped entirely when stdout is not a terminal (CI logs stay plain).
    func const char *paint(const char *p_code) const { return use_color ? p_code : ""; }
    /*-------------------------------------------------------------------------------*/

    // ── Value printing ───────────────────────────────────────────────────────────────────────

    // Resolves the raw scalar stored behind a wrapper type exposing 'get()' (u8, u16, i8, ...).
    // Types without 'get()' keep 'type = void' and are reported as '<unprintable>'.
    template <typename T, typename = void>
    struct Underlying {
        typedef void type;
    };

    template <typename T>
    struct Underlying<T, decltype(static_cast<void>(std::declval<const T &>().get()))> {
        typedef decltype(std::declval<const T &>().get()) type;
    };

    // Writes the value of a wrapper type (anything exposing 'get()') into a text buffer.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    template <typename T>
    static void write_value(char *r_buffer, const std::size_t p_size, const T &p_value) {
        typedef typename Underlying<T>::type scalar;
        if constexpr (std::is_same<scalar, void>::value) {
            std::snprintf(r_buffer, p_size, "<unprintable>");
        } else if constexpr (std::is_floating_point<scalar>::value) {
            std::snprintf(r_buffer, p_size, "%.9g", static_cast<double>(p_value.get()));
        } else {
            std::snprintf(r_buffer, p_size, "%lld", static_cast<long long>(p_value.get()));
        }
    }

    // Writes a plain boolean as a word instead of a number.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    static void write_value(char *r_buffer, const std::size_t p_size, const bool &p_value) {
        std::snprintf(r_buffer, p_size, "%s", p_value ? "true" : "false");
    }

    // Writes a plain arithmetic value (no wrapper involved) into a text buffer.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    template <typename T>
    static void write_value(char *r_buffer, const std::size_t p_size, const T &p_value, const int) {
        if constexpr (std::is_floating_point<T>::value) {
            std::snprintf(r_buffer, p_size, "%.9g", static_cast<double>(p_value));
        } else if constexpr (std::is_integral<T>::value) {
            std::snprintf(r_buffer, p_size, "%lld", static_cast<long long>(p_value));
        } else {
            std::snprintf(r_buffer, p_size, "<unprintable>");
        }
    }
    /*-------------------------------------------------------------------------------*/

    // ── Check bookkeeping ────────────────────────────────────────────────────────────────────

    // Counts a passing check and advances the progress dots.
    // NOTE: Not 'func' - stdout flushing cannot be part of constant evaluation.
    void count_pass() {
        check_total += 1;
        case_check_total += 1;
        pass_count += 1;
        std::printf("%s.%s", paint(green()), paint(reset()));
        std::fflush(stdout);
        line_open = true;
    }

    // Counts a failing check; closes the dot line so the failure block starts cleanly.
    // NOTE: Not 'func' - stdout flushing cannot be part of constant evaluation.
    void count_fail() {
        check_total += 1;
        case_check_total += 1;
        fail_count += 1;
        case_fail_count += 1;
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
    }

    // Records a comparison result. On failure, both operands are shown alongside the test name.
    // NOTE: Not 'func' - the failing branch writes to stdout.
    template <typename T>
    void record(const bool p_passed, const char *p_relation, const T &p_a, const T &p_b) {
        if (p_passed) {
            count_pass();
            return;
        }

        count_fail();

        char buffer_a[64];
        char buffer_b[64];
        write_value(buffer_a, sizeof(buffer_a), p_a, 0);
        write_value(buffer_b, sizeof(buffer_b), p_b, 0);

        std::printf("    %sFAIL%s [%s] (#%llu) condition '%s %s %s' did not hold\n",
                    paint(red()), paint(reset()), case_name, check_total,
                    buffer_a, p_relation, buffer_b);
    }

    // Records a bare boolean expectation ('test_true' / 'test_false').
    // NOTE: Not 'func' - the failing branch writes to stdout.
    void record_flag(const bool p_passed, const char *p_expectation, const bool p_got) {
        if (p_passed) {
            count_pass();
            return;
        }

        count_fail();
        std::printf("    %sFAIL%s [%s] (#%llu) expected %s, got %s\n",
                    paint(red()), paint(reset()), case_name, check_total,
                    p_expectation, p_got ? "true" : "false");
    }

    // Starts a new test case; every check made from now on belongs to it.
    // NOTE: Not 'func' - the progress line has to be flushed.
    void begin_case(const char *p_name) {
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
        case_name = p_name;
        case_check_total = 0;
        case_fail_count = 0;
        case_total += 1;
        std::printf("%s[ RUN      ]%s %s", paint(bold()), paint(reset()), p_name);
        std::fflush(stdout);
    }

    // Ends the current test case and prints its tally.
    // NOTE: Not 'func' - the progress line has to be flushed.
    void end_case() {
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
        if (case_check_total == 0) {
            std::printf("%s[      OK ]%s %s %s(no checks performed)%s\n", paint(bold()), paint(reset()),
                        case_name, paint(yellow()), paint(reset()));
        } else if (case_fail_count == 0) {
            std::printf("%s[       OK ]%s %s %s(%llu checks passed)%s\n", paint(bold()), paint(reset()),
                        case_name, paint(green()), case_check_total, paint(reset()));
        } else {
            std::printf("%s[  FAILED  ]%s %s %s(%llu of %llu checks failed)%s\n", paint(bold()), paint(reset()),
                        case_name, paint(red()), case_fail_count, case_check_total, paint(reset()));
        }
    }

    // Returns whether the given name matches the given filter (case sensitive substring match).
    // An empty filter matches everything.
    static bool matches(const char *p_name, const std::string &p_filter) {
        return p_filter.empty() || std::string(p_name).find(p_filter) != std::string::npos;
    }
    /*-------------------------------------------------------------------------------*/

    public:
    // Constructor.
    // NOTE: Not 'func' - terminal detection is a runtime-only query.
    Tester() { use_color = TESTER_ISATTY(stdout); }
    // Deleted copy constructor: a test run owns its own counters.
    Tester(const Tester &) = delete;
    // Deleted copy assignment: a test run owns its own counters.
    Tester &operator=(const Tester &) = delete;
    /*-------------------------------------------------------------------------------*/

    // ── Comparisons ─────────────────────────────────────────────────────────────────────────

    // Fails unless both values are equal.
    template <typename T>
    void test_equal(const T &p_a, const T &p_b) { record(p_a == p_b, "==", p_a, p_b); }

    // Fails unless both values differ.
    // NOTE: Expressed through 'operator==' since the custom types do not define 'operator!='
    //       on every overload set.
    template <typename T>
    void test_not_equal(const T &p_a, const T &p_b) { record(!(p_a == p_b), "!=", p_a, p_b); }

    // Fails unless the first value is greater than the second.
    template <typename T>
    void test_greater(const T &p_a, const T &p_b) { record(p_a > p_b, ">", p_a, p_b); }

    // Fails unless the first value is greater than or equal to the second.
    template <typename T>
    void test_greater_or_equal(const T &p_a, const T &p_b) { record(p_a >= p_b, ">=", p_a, p_b); }

    // Fails unless the first value is smaller than the second.
    template <typename T>
    void test_less(const T &p_a, const T &p_b) { record(p_a < p_b, "<", p_a, p_b); }

    // Fails unless the first value is smaller than or equal to the second.
    template <typename T>
    void test_less_or_equal(const T &p_a, const T &p_b) { record(p_a <= p_b, "<=", p_a, p_b); }
    /*-------------------------------------------------------------------------------*/

    // ── Booleans ────────────────────────────────────────────────────────────────────────────

    // Fails unless the condition holds. (Preferred over 'test_equal(condition, true)'.)
    void test_true(const bool p_condition) {
        record_flag(p_condition, "true", p_condition);
    }

    // Fails unless the condition does not hold.
    void test_false(const bool p_condition) {
        record_flag(!p_condition, "false", p_condition);
    }

    // Fails unless both conditions hold. (Matching two conditions at once.)
    void test_match(const bool p_condition_a, const bool p_condition_b) {
        record_flag(p_condition_a && p_condition_b, "both true", p_condition_b);
    }
    /*-------------------------------------------------------------------------------*/

    // ── Floating point ──────────────────────────────────────────────────────────────────────

    // Fails unless both values lie within the given tolerance of each other.
    void test_approximately(const float p_a, const float p_b, const float p_tolerance) {
        float difference = p_a - p_b;
        if (difference < 0.0f) {
            difference = -difference;
        }
        record(difference <= p_tolerance, "~=", p_a, p_b);
    }

    // Fails unless both values lie within the given tolerance of each other.
    void test_approximately(const double p_a, const double p_b, const double p_tolerance) {
        double difference = p_a - p_b;
        if (difference < 0.0) {
            difference = -difference;
        }
        record(difference <= p_tolerance, "~=", p_a, p_b);
    }
    /*-------------------------------------------------------------------------------*/

    // ── Runtime registration ────────────────────────────────────────────────────────────────

    // Registers a test case to be executed by 'run'.
    // Safe to call from any translation unit at any time before (or during) a run.
    static void register_test(const char *p_name, TestBody p_body) {
        registry().push_back(TestCase{p_name, p_body});
    }

    // Registers a test case the moment the object is constructed.
    // Declare one at namespace scope to register a test without touching 'main':
    //     static const Tester::AutoTest registration("name", &body);
    class AutoTest {
        public:
        AutoTest(const char *p_name, TestBody p_body) { register_test(p_name, p_body); }
    };

    // Returns the number of registered test cases.
    static std::size_t test_count() { return registry().size(); }

    // Returns whether at least one test case has been registered.
    static bool has_tests() { return !registry().empty(); }
    /*-------------------------------------------------------------------------------*/

    // ── Execution ───────────────────────────────────────────────────────────────────────────

    // Prints every registered test case which matches the given filter.
    static void list(const std::string &p_filter = std::string()) {
        std::printf("Registered test cases: %llu\n", (unsigned long long)registry().size());
        for (const TestCase &test : registry()) {
            if (matches(test.name, p_filter)) {
                std::printf("  %s\n", test.name);
            }
        }
    }

    // Executes every registered test case which matches the given filter, and returns
    // 0 when all of the executed checks passed, otherwise 1.
    // NOTE: Not 'func' - tests are performed at runtime.
    int run(const std::string &p_filter = std::string()) {
        for (const TestCase &test : registry()) {
            if (!matches(test.name, p_filter)) {
                continue;
            }
            begin_case(test.name);
            test.body(*this);
            end_case();
        }

        if (case_total == 0) {
            std::printf("%sNo test case matched the filter '%s'.%s\n",
                        paint(yellow()), p_filter.c_str(), paint(reset()));
        }

        print_results();
        return all_passed() ? 0 : 1;
    }

    // Parses the runtime arguments and executes the requested test cases.
    // Supported arguments: '--filter <text>', '--filter=<text>', '--list', '--help'.
    // NOTE: Not 'func' - tests are performed at runtime.
    int run(const int p_argc, char **p_argv) {
        std::string filter;
        bool list_only = false;

        for (int index = 1; index < p_argc; ++index) {
            const char *argument = p_argv[index] == nullptr ? "" : p_argv[index];
            if (std::strcmp(argument, "--list") == 0) {
                list_only = true;
            } else if (std::strcmp(argument, "--filter") == 0 && index + 1 < p_argc) {
                filter = p_argv[++index];
            } else if (std::strncmp(argument, "--filter=", 9) == 0) {
                filter = argument + 9;
            } else if (std::strcmp(argument, "--help") == 0 || std::strcmp(argument, "-h") == 0) {
                print_usage(p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_tests");
                return 0;
            } else {
                std::printf("%sUnknown argument: %s%s\n", paint(red()), argument, paint(reset()));
                print_usage(p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_tests");
                return 1;
            }
        }

        if (list_only) {
            list(filter);
            return 0;
        }

        return run(filter);
    }

    // Returns whether every executed check passed and at least one check was performed.
    bool all_passed() const { return fail_count == 0 && check_total > 0; }

    // Returns whether the test case which is currently being executed has a failing check.
    bool case_passed() const { return case_fail_count == 0; }

    // Returns the number of failing checks of the whole run.
    unsigned long long failures() const { return fail_count; }

    // Prints the summary of the whole run.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    void print_results() const {
        double pass_rate = 0.0;
        if (check_total > 0) {
            pass_rate = 100.0 * static_cast<double>(pass_count) / static_cast<double>(check_total);
        }

        std::printf("\n");
        std::printf("==============================================\n");
        std::printf("           RATLAB TEST RESULTS\n");
        std::printf("==============================================\n");
        std::printf("  Test cases run ..... %llu\n", case_total);
        std::printf("  Checks total ....... %llu\n", check_total);
        std::printf("  Checks passed ...... %llu\n", pass_count);
        std::printf("  Checks failed ...... %llu\n", fail_count);
        std::printf("  Pass rate .......... %.1f%%\n", pass_rate);
        std::printf("----------------------------------------------\n");
        if (all_passed()) {
            std::printf("  RESULT: %sSUCCESS%s (all checks passed)\n", paint(green()), paint(reset()));
        } else if (check_total == 0) {
            std::printf("  RESULT: %sNO CHECKS PERFORMED%s\n", paint(yellow()), paint(reset()));
        } else {
            std::printf("  RESULT: %sFAILURE%s (%llu failing checks)\n",
                        paint(red()), paint(reset()), fail_count);
        }
        std::printf("==============================================\n");
    }

    // Prints the usage text of the test runner.
    static void print_usage(const char *p_program) {
        std::printf("Usage: %s [--filter <text>] [--list] [--help]\n", p_program);
        std::printf("  --filter <text>  Only run the test cases whose name contains <text>\n");
        std::printf("  --list           List the registered test cases without running them\n");
        std::printf("  --help           Show this text\n");
    }
};
