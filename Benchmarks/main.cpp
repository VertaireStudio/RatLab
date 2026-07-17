#include "../Core/Testing/Benchmarker.hpp"

// ---- Benchmarker-based benchmarks -------------------------------------------
// These can be registered from anywhere in the codebase that includes
// Core/Testing/Benchmarker.hpp.  Registration happens before main() because
// the static `register_benchmark` is called at namespace scope.
namespace {
bool registered = []() {
    RatLab::Benchmarker::register_benchmark("BM_Placeholder", [](benchmark::State& state) {
        for (auto _ : state) {
            int x = 42;
            benchmark::DoNotOptimize(x);
        }
    });
    return true;
}();
} // unnamed namespace

// ---- Entry point ------------------------------------------------------------
int main(int argc, char** argv)
{
    return RatLab::Benchmarker::run(argc, argv);
}
