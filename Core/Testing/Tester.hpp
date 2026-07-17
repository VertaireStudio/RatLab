/************************************/
/*           Tester.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace RatLab {

// Tester — A lightweight, standalone test runner.
//
// Other scripts/modules can register named test lambdas at runtime via the
// static `register_test()` method.  Each lambda returns int (0 = pass,
// non-zero = fail).  Calling `run()` executes all registered tests in the
// order they were registered and prints a summary.
//
// Usage:
//   Tester::register_test("addition", []() -> int {
//       return (1 + 1 == 2) ? 0 : 1;
//   });
//   int failures = Tester::run();
class Tester {
    public:
    using TestFunc = std::function<int()>;

    // Registration
    static void register_test(std::string name, TestFunc func) {
        instance().tests_.push_back({std::move(name), std::move(func)});
    }

    // Execution
    static int run() {
        auto& inst      = instance();
        int   passed    = 0;
        int   failed    = 0;

        for (const auto& entry : inst.tests_) {
            std::cout << "[          ] " << entry.name << "\n";
            try {
                int result = entry.func();
                if (result == 0) {
                    std::cout << "[   PASS   ] " << entry.name << "\n";
                    ++passed;
                } else {
                    std::cout << "[   FAIL   ] " << entry.name
                              << " (exit code " << result << ")\n";
                    ++failed;
                }
            } catch (const std::exception& ex) {
                std::cout << "[   FAIL   ] " << entry.name
                          << " (exception: " << ex.what() << ")\n";
                ++failed;
            } catch (...) {
                std::cout << "[   FAIL   ] " << entry.name
                          << " (unknown exception)\n";
                ++failed;
            }
        }

        // Summary
        std::cout << "\n";
        std::cout << "=============== Test Summary ===============\n";
        std::cout << "  Total:  " << (passed + failed) << "\n";
        std::cout << "  Passed: " << passed << "\n";
        std::cout << "  Failed: " << failed << "\n";
        std::cout << "============================================\n";

        return failed;
    }

    // Utility
    static void clear() {
        instance().tests_.clear();
    }

    static std::size_t test_count() {
        return instance().tests_.size();
    }

    private:
    struct TestEntry {
        std::string name;
        TestFunc    func;
    };

    std::vector<TestEntry> tests_;

    // Singleton access
    static Tester& instance() {
        static Tester inst;
        return inst;
    }

    Tester()  = default;
    ~Tester() = default;

    Tester(const Tester&)            = delete;
    Tester& operator=(const Tester&) = delete;
};

} // namespace RatLab