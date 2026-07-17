#define DOCTEST_CONFIG_IMPLEMENT
#include "../Thirdparty/doctest/doctest/doctest.h"

#include "../Core/Testing/Tester.hpp"

// ---- doctest-based tests ----------------------------------------------------
TEST_CASE("smoke test") {
    CHECK(1 + 1 == 2);
}

// ---- Tester-based tests -----------------------------------------------------
// These can be registered from anywhere in the codebase that includes
// Core/Testing/Tester.hpp.  Registration happens before main() because the
// static `register_test` is called at namespace scope.
namespace {
bool registered = []() {
    RatLab::Tester::register_test("tester smoke", []() -> int {
        return (1 + 1 == 2) ? 0 : 1;
    });
    return true;
}();
} // unnamed namespace

// ---- Combined runner --------------------------------------------------------
int main(int argc, char** argv)
{
    // 1) Run doctest
    doctest::Context context(argc, argv);
    int doctest_result = context.run();

    // 2) Run Tester-registered tests
    int tester_result = RatLab::Tester::run();

    // Return non-zero if either reported failures
    return (doctest_result != 0 || tester_result != 0) ? 1 : 0;
}
