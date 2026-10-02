/************************************/
/*           tester.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "console.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// The Tester type: A self-contained, dependency-free test framework for RatLab.
// Test bodies are registered at runtime (or at static-initialization time) and are executed
// on demand by 'run', either from 'main' or from any other runtime entry point.
// Every check made inside a test body is counted, and a non-zero exit code is returned
// whenever at least one check fails.
// The outcome of every run is additionally written to 'REPORT_FILE' as plain text, so that it
// survives the process and can be compared between two runs.
class Tester {
    public:
    // The signature every registered test body has to follow.
    // The body receives the Tester instance which collects and reports the checks.
    typedef void (*TestBody)(Tester &p_tester);
    /*---------------------------------------------------------------------------------------*/

    private:
    // A single check which did not hold, kept until the report of its test case is written.
    struct Failure {
        // The number of the check inside the whole run.
        unsigned long long index;
        // The description of what went wrong.
        char text[192];
    };

    // A single registered test case, waiting to be executed at runtime.
    struct TestCase {
        // The name of the test case, shown in the report. (Must outlive the registration.)
        const char *name;
        // The body which performs the actual checks.
        TestBody body;
    };

    // The compiled outcome of a single executed test case, kept until the whole run has been
    // reported. Every executed test case appends one, which is what makes the outcome of a
    // finished run available for the report file.
    struct CaseResult {
        // The name of the test case, shown in the report. (Must outlive the registration.)
        const char *name;
        // The amount of checks which were performed by the test case.
        unsigned long long checks;
        // The amount of checks which did not hold.
        unsigned long long failing_checks;
        // The time the test case took, in nanoseconds.
        double ns;
        // The failing checks, empty when every check of the test case held.
        std::vector<Failure> failures;
    };

    // The shared registry of every test case known to the workspace.
    // A function-local static is used so that registration is safe to perform from any
    // translation unit, no matter in which order the static-initializers happen to run.
    static std::vector<TestCase> &registry() {
        static std::vector<TestCase> cases;
        return cases;
    }

    // The shared output plumbing.
    Console console;
    // Totals across every executed test case.
    unsigned long long check_total = 0;
    unsigned long long pass_count = 0;
    unsigned long long fail_count = 0;
    // The number of test cases which have been executed in this run.
    unsigned long long case_total = 0;
    // The name of the test case which is currently being executed.
    const char *case_name = "";
    // The totals of the test case which is currently being executed.
    unsigned long long case_check_total = 0;
    unsigned long long case_fail_count = 0;
    // The time the test case which is currently being executed took, in nanoseconds.
    double case_ns = 0.0;
    // The failing checks of the test case which is currently being executed.
    std::vector<Failure> case_failures;
    // The outcome of every test case executed in this run, in the order they were executed.
    std::vector<CaseResult> case_results;
    // The width of the name column, derived from the longest executed test case name.
    std::size_t name_width = 0;
    // The width of the whole report, used to draw the rules.
    static constexpr const std::size_t REPORT_WIDTH = 74;
    // The longest name which is allowed to widen the name column.
    static constexpr const std::size_t MAX_NAME_WIDTH = 46;
    // The directory the report of every run is written into, relative to the working directory
    // of the process. It is created when it does not exist yet.
    static constexpr const char *REPORT_DIRECTORY = "Misc";
    // The report itself, overwritten each time so that it always holds the most recent run.
    static constexpr const char *REPORT_FILE = "Misc/ratlab_tests.txt";
    // Whether only the failing test cases are reported.
    bool quiet = false;
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

    // Counts a passing check.
    // NOTE: Not 'func' - the counters belong to a running test case.
    void count_pass() {
        check_total += 1;
        case_check_total += 1;
        pass_count += 1;
    }

    // Counts a failing check and keeps its description until the report is written.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    void count_fail(const char *p_message) {
        check_total += 1;
        case_check_total += 1;
        fail_count += 1;
        case_fail_count += 1;

        Failure failure;
        failure.index = check_total;
        std::snprintf(failure.text, sizeof(failure.text), "%s", p_message);
        case_failures.push_back(failure);
    }

    // Records a comparison result. On failure, both operands are kept alongside the test name.
    // NOTE: Not 'func' - the failing branch formats a message.
    template <typename T>
    void record(const bool p_passed, const char *p_relation, const T &p_a, const T &p_b) {
        if (p_passed) {
            count_pass();
            return;
        }

        char buffer_a[64];
        char buffer_b[64];
        write_value(buffer_a, sizeof(buffer_a), p_a, 0);
        write_value(buffer_b, sizeof(buffer_b), p_b, 0);

        char message[192];
        std::snprintf(message, sizeof(message), "'%s %s %s' did not hold", buffer_a, p_relation, buffer_b);
        count_fail(message);
    }

    // Records a bare boolean expectation ('test_true' / 'test_false').
    // NOTE: Not 'func' - the failing branch formats a message.
    void record_flag(const bool p_passed, const char *p_expectation, const bool p_got) {
        if (p_passed) {
            count_pass();
            return;
        }

        char message[192];
        std::snprintf(message, sizeof(message), "expected '%s', got '%s'",
                      p_expectation, p_got ? "true" : "false");
        count_fail(message);
    }
    /*-------------------------------------------------------------------------------*/

    // ── Reporting ───────────────────────────────────────────────────────────────────────────

    // Returns the current value of the monotonic clock, in nanoseconds.
    static double now_ns() {
        return static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count());
    }

    // Returns whether the given name matches the given filter (case sensitive substring match).
    // An empty filter matches everything.
    static bool matches(const char *p_name, const std::string &p_filter) {
        return p_filter.empty() || std::string(p_name).find(p_filter) != std::string::npos;
    }

    // Returns the width of the name column, so that every test case lines up.
    static std::size_t measure_name_width(const std::string &p_filter) {
        std::size_t width = 0;
        for (const TestCase &test : registry()) {
            if (!matches(test.name, p_filter)) {
                continue;
            }
            const std::size_t length = std::strlen(test.name);
            if (length > width) {
                width = length;
            }
        }
        return width < MAX_NAME_WIDTH ? width : MAX_NAME_WIDTH;
    }

    // Prints the single line of the test case which was just executed, followed by one line
    // per failing check. Passing test cases are skipped entirely while 'quiet' is set.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    void print_case() const {
        const bool passed = case_fail_count == 0;
        if (quiet && passed) {
            return;
        }

        char buffer_checks[32];
        char buffer_time[32];
        Console::format_checks(buffer_checks, sizeof(buffer_checks), case_check_total);
        Console::format_duration(buffer_time, sizeof(buffer_time), case_ns);

        console.clear_line();
        std::printf("  %s%s%s  %-*s  %*s  %*s\n",
                    passed ? console.paint(Console::green()) : console.paint(Console::red()),
                    passed ? console.glyph_ok() : console.glyph_fail(),
                    console.paint(Console::reset()),
                    (int)name_width, case_name,
                    9, buffer_checks,
                    10, buffer_time);

        for (const Failure &failure : case_failures) {
            std::printf("      %s!%s %s%4llu%s  %s\n",
                        console.paint(Console::red()), console.paint(Console::reset()),
                        console.paint(Console::dim()), failure.index, console.paint(Console::reset()),
                        failure.text);
        }
    }

    // Executes a single test case and reports it.
    // NOTE: Not 'func' - tests are performed at runtime.
    void run_case(const TestCase &p_test) {
        case_name = p_test.name;
        case_check_total = 0;
        case_fail_count = 0;
        case_failures.clear();
        case_total += 1;

        console.progress(case_name, name_width);
        const double start_ns = now_ns();
        p_test.body(*this);
        case_ns = now_ns() - start_ns;
        print_case();

        // The outcome is kept, since the report file has to describe every executed test case,
        // not only the one which happens to be the last one.
        CaseResult result;
        result.name = p_test.name;
        result.checks = case_check_total;
        result.failing_checks = case_fail_count;
        result.ns = case_ns;
        result.failures = case_failures;
        case_results.push_back(result);
    }

    // Prints the summary of the whole run.
    // NOTE: Not 'func' - the summary is a runtime-only operation.
    void print_summary(const double p_total_ns) const {
        char buffer_time[32];
        char buffer_count[24];
        Console::format_duration(buffer_time, sizeof(buffer_time), p_total_ns);
        Console::format_count(buffer_count, sizeof(buffer_count), check_total);

        std::printf("\n");
        console.rule(REPORT_WIDTH);
        std::printf("  %llu %s%s%s%s%llu passed%s%llu failed%s%s\n",
                    case_total, Console::plural(case_total, "test case", "test cases"),
                    console.separator(), buffer_count, console.separator(),
                    pass_count, console.separator(), fail_count, console.separator(), buffer_time);

        const char *verdict = "FAILURE";
        const char *color = console.paint(Console::red());
        if (all_passed()) {
            verdict = "SUCCESS";
            color = console.paint(Console::green());
        } else if (check_total == 0) {
            verdict = "NO CHECKS PERFORMED";
            color = console.paint(Console::yellow());
        }
        std::printf("  %s%s%s%s\n", color, console.paint(Console::bold()), verdict,
                    console.paint(Console::reset()));
    }

    // ── Report file ────────────────────────────────────────────────────────────────────────

    // Writes the outcome of the finished run into 'REPORT_FILE': the environment it was
    // executed in, one line per executed test case followed by its failing checks, and the
    // summary of the run. The report is plain text, holds no escape sequences, and can
    // therefore be diffed between two runs. Test cases which passed are always listed, even
    // when the console report skipped them.
    // The report directory is created when the process does not run from the workspace root.
    // Returns whether the report could be written.
    // NOTE: Not 'func' - the report is a runtime-only operation.
    bool export_report(const std::string &p_filter, const double p_total_ns) const {
        if (!Console::ensure_directory(REPORT_DIRECTORY)) {
            std::printf("  %s%scould not create %s%s\n", console.paint(Console::red()),
                        console.paint(Console::bold()), REPORT_DIRECTORY,
                        console.paint(Console::reset()));
            return false;
        }

        std::FILE *file = std::fopen(REPORT_FILE, "w");
        if (file == nullptr) {
            std::printf("  %s%scould not write %s%s\n", console.paint(Console::red()),
                        console.paint(Console::bold()), REPORT_FILE,
                        console.paint(Console::reset()));
            return false;
        }

        char buffer_stamp[32];
        Console::format_timestamp(buffer_stamp, sizeof(buffer_stamp));

        std::fprintf(file, "RatLab tests\n");
        std::fprintf(file, "============\n\n");
        std::fprintf(file, "date      : %s\n", buffer_stamp);
        std::fprintf(file, "platform  : %s\n", Console::platform_name());
        std::fprintf(file, "compiler  : %s\n", Console::compiler_name());
        std::fprintf(file, "standard  : %s\n", Console::cpp_standard_name());
        std::fprintf(file, "filter    : %s\n\n", p_filter.empty() ? "(none)" : p_filter.c_str());

        if (case_results.empty()) {
            std::fprintf(file, "no test case matched the filter\n");
        } else {
            for (const CaseResult &result : case_results) {
                char buffer_checks[32];
                char buffer_time[32];
                Console::format_checks(buffer_checks, sizeof(buffer_checks), result.checks);
                Console::format_duration(buffer_time, sizeof(buffer_time), result.ns);

                std::fprintf(file, "  %-6s  %-*s  %*s  %*s\n",
                             result.failing_checks == 0 ? "PASSED" : "FAILED",
                             (int)measure_name_width(p_filter), result.name,
                             9, buffer_checks, 10, buffer_time);
                for (const Failure &failure : result.failures) {
                    std::fprintf(file, "      !%4llu  %s\n", failure.index, failure.text);
                }
            }
        }

        char buffer_time[32];
        char buffer_checks[32];
        Console::format_duration(buffer_time, sizeof(buffer_time), p_total_ns);
        Console::format_checks(buffer_checks, sizeof(buffer_checks), check_total);

        std::fprintf(file, "\n%llu %s, %s, %llu passed, %llu failed, %s total\n",
                     case_total, Console::plural(case_total, "test case", "test cases"),
                     buffer_checks, pass_count, fail_count, buffer_time);
        std::fprintf(file, "%s\n", check_total == 0 ? "NO CHECKS PERFORMED"
                          : all_passed()           ? "SUCCESS"
                                                     : "FAILURE");

        std::fclose(file);
        std::printf("  %sreport%s  %s%s%s\n", console.paint(Console::dim()),
                    console.paint(Console::reset()), console.paint(Console::bold()), REPORT_FILE,
                    console.paint(Console::reset()));
        return true;
    }
    /*-------------------------------------------------------------------------------*/

    public:
    // Constructor.
    Tester() = default;

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

    // Prints every registered test case which matches the given filter, one per line.
    static void list(const std::string &p_filter = std::string()) {
        Console console;
        std::size_t shown = 0;
        for (const TestCase &test : registry()) {
            if (matches(test.name, p_filter)) {
                ++shown;
            }
        }

        std::printf("%s%s%llu test cases%s\n", console.paint(Console::bold()),
                    console.paint(Console::cyan()), (unsigned long long)shown,
                    console.paint(Console::reset()));
        for (const TestCase &test : registry()) {
            if (matches(test.name, p_filter)) {
                std::printf("  %s\n", test.name);
            }
        }
    }

    // Executes every registered test case which matches the given filter, and returns
    // 0 when all of the executed checks passed, otherwise 1.
    // NOTE: Not 'func' - tests are performed at runtime.
    int run(const std::string &p_filter = std::string(), const bool p_quiet = false) {
        quiet = p_quiet;
        name_width = measure_name_width(p_filter);
        case_results.clear();

        std::printf("%s%sRatLab tests%s  %s%llu registered\n", console.paint(Console::bold()),
                    console.paint(Console::cyan()), console.paint(Console::reset()),
                    console.separator(), (unsigned long long)registry().size());

        const double start_ns = now_ns();
        for (const TestCase &test : registry()) {
            if (matches(test.name, p_filter)) {
                run_case(test);
            }
        }
        const double total_ns = now_ns() - start_ns;

        if (case_total == 0) {
            std::printf("\n  %sno test case matched the filter '%s'%s\n",
                        console.paint(Console::yellow()), p_filter.c_str(),
                        console.paint(Console::reset()));
        }

        print_summary(total_ns);
        export_report(p_filter, total_ns);
        return all_passed() ? 0 : 1;
    }

    // Parses the runtime arguments and executes the requested test cases.
    // Supported arguments: '--filter <text>', '--filter=<text>', '--list', '--quiet', '--help'.
    // NOTE: Not 'func' - tests are performed at runtime.
    int run(const int p_argc, char **p_argv) {
        const char *program = p_argc > 0 && p_argv[0] != nullptr ? p_argv[0] : "ratlab_tests";
        std::string filter;
        bool list_only = false;
        bool quiet = false;

        for (int index = 1; index < p_argc; ++index) {
            const char *argument = p_argv[index] == nullptr ? "" : p_argv[index];
            if (std::strcmp(argument, "--list") == 0) {
                list_only = true;
            } else if (std::strcmp(argument, "--quiet") == 0 || std::strcmp(argument, "-q") == 0) {
                quiet = true;
            } else if (std::strcmp(argument, "--filter") == 0 && index + 1 < p_argc) {
                filter = p_argv[++index];
            } else if (std::strncmp(argument, "--filter=", 9) == 0) {
                filter = argument + 9;
            } else if (std::strcmp(argument, "--help") == 0 || std::strcmp(argument, "-h") == 0) {
                print_usage(program);
                return 0;
            } else {
                std::printf("%sUnknown argument: %s%s\n", console.paint(Console::red()), argument,
                            console.paint(Console::reset()));
                print_usage(program);
                return 1;
            }
        }

        if (list_only) {
            list(filter);
            return 0;
        }

        return run(filter, quiet);
    }

    // Returns whether every executed check passed and at least one check was performed.
    bool all_passed() const { return fail_count == 0 && check_total > 0; }

    // Returns whether the test case which is currently being executed has a failing check.
    bool case_passed() const { return case_fail_count == 0; }

    // Returns the number of failing checks of the whole run.
    unsigned long long failures() const { return fail_count; }

    // Returns the number of checks which were performed so far.
    unsigned long long checks() const { return check_total; }

    // Prints the usage text of the test runner.
    static void print_usage(const char *p_program) {
        Console console;
        std::printf("%sUsage:%s %s [options]\n", console.paint(Console::bold()),
                    console.paint(Console::reset()), p_program);
        std::printf("  %-18s Only run the test cases whose name contains <text>\n", "--filter <text>");
        std::printf("  %-18s Only report the failing test cases\n", "--quiet, -q");
        std::printf("  %-18s List the registered test cases without running them\n", "--list");
        std::printf("  %-18s Show this text\n", "--help, -h");
    }
};
