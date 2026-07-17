/************************************/
/*        Benchmarker.hpp           */
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

#include "../../Thirdparty/benchmark/include/benchmark/benchmark.h"

namespace RatLab {

// Benchmarker — A convenience wrapper around Google Benchmark.
//
// Other scripts/modules can register named benchmark lambdas at runtime via
// the static `register_benchmark()` method.  Each lambda receives a
// `benchmark::State&` and should use the standard Google Benchmark loop
// pattern (for (auto _ : state) { ... }).
//
// Calling `run(argc, argv)` executes all registered benchmarks and returns
// the exit code from `benchmark::RunSpecifiedBenchmarks()`.
//
// Usage:
//   Benchmarker::register_benchmark("my_bench", [](benchmark::State& s) {
//       for (auto _ : s) {
//           int x = 42;
//           benchmark::DoNotOptimize(x);
//       }
//   });
//   int exit_code = Benchmarker::run(argc, argv);
class Benchmarker {
public:
    using BenchmarkFunc = std::function<void(benchmark::State&)>;

    // Registration
    static void register_benchmark(std::string name, BenchmarkFunc func) {
        instance().benchmarks_.push_back({std::move(name), std::move(func)});
    }

    // Execution
    static int run(int argc, char** argv) {
        auto& inst = instance();

        // Register all stored benchmarks with Google Benchmark
        for (const auto& entry : inst.benchmarks_) {
            benchmark::RegisterBenchmark(entry.name.c_str(), entry.func);
        }

        // Delegate to Google Benchmark's own runner
        ::benchmark::Initialize(&argc, argv);
        int result = ::benchmark::RunSpecifiedBenchmarks();
        ::benchmark::Shutdown();

        return result;
    }

    // Utility
    static void clear() {
        instance().benchmarks_.clear();
    }

    static std::size_t benchmark_count() {
        return instance().benchmarks_.size();
    }

private:
    struct BenchmarkEntry {
        std::string    name;
        BenchmarkFunc  func;
    };

    std::vector<BenchmarkEntry> benchmarks_;

    // Singleton access
    static Benchmarker& instance() {
        static Benchmarker inst;
        return inst;
    }

    Benchmarker()  = default;
    ~Benchmarker() = default;

    Benchmarker(const Benchmarker&)            = delete;
    Benchmarker& operator=(const Benchmarker&) = delete;
};

} // namespace RatLab