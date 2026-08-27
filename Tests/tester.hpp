/************************************/
/*           tester.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Core/Essentials/essentials.hpp"
#include <cstdio>

#if defined(_WIN32)
#include <io.h>
#define TESTER_ISATTY(stream) (_isatty(_fileno(stream)) != 0)
#else
#include <unistd.h>
#define TESTER_ISATTY(stream) (::isatty(::fileno(stream)) != 0)
#endif

struct Tester {
    private:
    u32 pass_count;
    u32 fail_count;
    u32 check_total;
    u32 groups_run;
    u32 group_checks;
    u32 group_fails;
    const char* group_name;
    bool use_color;
    bool line_open;

    func static const char* green() { return "\x1b[32m"; }
    func static const char* red()   { return "\x1b[31m"; }
    func static const char* yellow(){ return "\x1b[33m"; }
    func static const char* cyan()  { return "\x1b[36m"; }
    func static const char* reset() { return "\x1b[0m"; }

    // Colors are stripped entirely when stdout is not a terminal (CI logs stay plain).
    func const char* paint(const char* p_code) const { return use_color ? p_code : ""; }

    // Writes the underlying value of the scalar wrapper types ('get_value') into a text buffer.
    template <typename V>
    func static void write_underlying(char* r_buffer, const unsigned int p_size, const V p_value) {
        std::snprintf(r_buffer, p_size, "%lld", (long long)p_value);
    }

    func static void write_underlying(char* r_buffer, const unsigned int p_size, const float p_value) {
        std::snprintf(r_buffer, p_size, "%.9g", (double)p_value);
    }

    func static void write_underlying(char* r_buffer, const unsigned int p_size, const double p_value) {
        std::snprintf(r_buffer, p_size, "%.17g", p_value);
    }

    // Preferred overload - matches every type exposing 'get_value()' (the scalar wrappers).
    template <typename T>
    func static auto write_value(char* r_buffer, const unsigned int p_size, const T& p_value, const int) -> decltype((void)write_underlying(r_buffer, p_size, p_value.get_value())) {
        write_underlying(r_buffer, p_size, p_value.get_value());
    }

    // Exact overload - plain booleans read as words instead of numbers.
    func static void write_value(char* r_buffer, const unsigned int p_size, const bool& p_value, const int) {
        std::snprintf(r_buffer, p_size, p_value ? "true" : "false");
    }

    // Fallback overload - types without 'get_value()' cannot be printed portably with only '<cstdio>'.
    template <typename T>
    func static void write_value(char* r_buffer, const unsigned int p_size, const T&, ...) {
        std::snprintf(r_buffer, p_size, "<unprintable>");
    }

    // Counts a passing check and advances the progress dots.
    // NOTE: Not 'func' - stdout flushing cannot be part of constant evaluation.
    void count_pass() {
        check_total += 1;
        group_checks += 1;
        pass_count += 1;
        std::printf("%s.%s", paint(green()), paint(reset()));
        std::fflush(stdout);
        line_open = true;
    }

    // Counts a failing check; closes the dot line so the failure block starts cleanly.
    func void count_fail() {
        check_total += 1;
        group_checks += 1;
        fail_count += 1;
        group_fails += 1;
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
    }

    // Records a comparison result. On failure, both operands are shown alongside the group name.
    template <typename T>
    func void record(const bool p_passed, const char* p_relation, const T& p_a, const T& p_b) {
        if (p_passed) {
            count_pass();
            return;
        }

        count_fail();

        char buffer_a[64];
        char buffer_b[64];
        write_value(buffer_a, (unsigned int)sizeof(buffer_a), p_a, 0);
        write_value(buffer_b, (unsigned int)sizeof(buffer_b), p_b, 0);

        std::printf("  %sx FAIL%s [%s] (#%03u) condition '%s %s %s' did not hold\n",
                    paint(red()), paint(reset()), group_name,
                    (unsigned int)check_total.get_value(), buffer_a, p_relation, buffer_b);
    }

    // Records a bare boolean expectation ('test_true' / 'test_false').
    func void record_flag(const bool p_passed, const char* p_expectation, const bool p_got) {
        if (p_passed) {
            count_pass();
            return;
        }

        count_fail();
        std::printf("  %sx FAIL%s [%s] (#%03u) expected %s, got %s\n",
                    paint(red()), paint(reset()), group_name,
                    (unsigned int)check_total.get_value(), p_expectation, p_got ? "true" : "false");
    }

    public:
    // Constructors - Deconstructors

    // NOTE: Not 'func' - terminal detection is a runtime-only query.
    Tester()
        : pass_count(0), fail_count(0), check_total(0),
          groups_run(0), group_checks(0), group_fails(0), group_name(""),
          use_color(TESTER_ISATTY(stdout)), line_open(false) {}

    // Grouping

    // Starts a named section; subsequent checks belong to it until 'end_group' is called.
    func void begin_group(const char* p_name) {
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
        group_name = p_name;
        group_checks = 0;
        group_fails = 0;
        std::printf("\n%s--- %s ---%s\n", paint(cyan()), p_name, paint(reset()));
    }

    // Ends the current section and prints its tally.
    func void end_group() {
        groups_run += 1;
        if (line_open) {
            std::printf("\n");
            line_open = false;
        }
        if (group_checks.get_value() == 0u) {
            std::printf("  %sno checks performed%s\n", paint(yellow()), paint(reset()));
        } else if (group_fails.get_value() == 0u) {
            std::printf("  %s%u checks - all passed%s\n", paint(green()),
                        (unsigned int)group_checks.get_value(), paint(reset()));
        } else {
            std::printf("  %u checks - %s%u failed%s\n", (unsigned int)group_checks.get_value(),
                        paint(red()), (unsigned int)group_fails.get_value(), paint(reset()));
        }
    }

    // Runs a named section in one call - preferred over manual 'begin_group'/'end_group'.
    func void run_group(const char* p_name, void (*p_tests)(Tester&)) {
        begin_group(p_name);
        p_tests(*this);
        end_group();
    }

    // Comparison Tests

    template <typename T>
    func void test_equal(const T p_a, const T p_b) { record(p_a == p_b, "==", p_a, p_b); }

    // NOTE: Expressed through 'operator==' since the custom types do not define 'operator!='.
    template <typename T>
    func void test_not_equal(const T p_a, const T p_b) { record(!(p_a == p_b), "!=", p_a, p_b); }

    template <typename T>
    func void test_greater(const T p_a, const T p_b) { record(p_a > p_b, ">", p_a, p_b); }

    template <typename T>
    func void test_greater_or_equal(const T p_a, const T p_b) { record(p_a >= p_b, ">=", p_a, p_b); }

    template <typename T>
    func void test_less(const T p_a, const T p_b) { record(p_a < p_b, "<", p_a, p_b); }

    template <typename T>
    func void test_less_or_equal(const T p_a, const T p_b) { record(p_a <= p_b, "<=", p_a, p_b); }

    // Matching Tests - Both conditions must be true for success.

    func void test_match(const bool p_condition_a, const bool p_condition_b) {
        record(p_condition_a && p_condition_b, "&&", p_condition_a, p_condition_b);
    }

    // Boolean Tests - Preferred over 'test_match(condition, true)'.

    func void test_true(const bool p_condition) {
        record_flag(p_condition, "true", p_condition);
    }

    func void test_false(const bool p_condition) {
        record_flag(!p_condition, "false", p_condition);
    }

    // Float Comparisons With Tolerance

    func void test_approximately(const f32 p_a, const f32 p_b, const f32 p_epsilon) {
        f32 difference = p_a - p_b;
        if (difference < f32(0.0f)) {
            difference = f32(0.0f) - difference;
        }
        record(difference <= p_epsilon, "|diff| <=", difference, p_epsilon);
    }

    func void test_approximately(const f64 p_a, const f64 p_b, const f64 p_epsilon) {
        f64 difference = p_a - p_b;
        if (difference < f64(0.0)) {
            difference = f64(0.0) - difference;
        }
        record(difference <= p_epsilon, "|diff| <=", difference, p_epsilon);
    }

    // Reporting

    func bool all_passed() const {
        return (fail_count.get_value() == 0u) && (check_total.get_value() > 0u);
    }

    func void print_results() const {
        const u32 total = check_total;
        f64 pass_rate = f64(0.0);
        if (total.get_value() > 0u) {
            pass_rate = f64(100.0) * f64(pass_count.get_value()) / f64(total.get_value());
        }

        std::printf("\n==============================================\n");
        std::printf("           RATLAB TEST RESULTS\n");
        std::printf("==============================================\n");
        std::printf("  Groups run ......... %u\n", (unsigned int)groups_run.get_value());
        std::printf("  Checks total ....... %u\n", (unsigned int)total.get_value());
        std::printf("  Checks passed ...... %u\n", (unsigned int)pass_count.get_value());
        std::printf("  Checks failed ...... %u\n", (unsigned int)fail_count.get_value());
        std::printf("  Pass rate .......... %.1f%%\n", (double)pass_rate.get_value());
        std::printf("----------------------------------------------\n");
        if (all_passed()) {
            std::printf("  RESULT: %sSUCCESS%s (all checks passed)\n", paint(green()), paint(reset()));
        } else if (total.get_value() == 0u) {
            std::printf("  RESULT: %sNO CHECKS PERFORMED%s\n", paint(yellow()), paint(reset()));
        } else {
            std::printf("  RESULT: %sFAILURE%s (%u failing checks)\n", paint(red()), paint(reset()),
                        (unsigned int)fail_count.get_value());
        }
        std::printf("==============================================\n");
    }
};
