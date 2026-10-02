# =============================================================================
# RatLab — Unit Tests
# =============================================================================
# The test framework is RatLab's own ('Core/Testing/Tester.hpp'), there is no
# third-party dependency to fetch. Every test file registers its test cases at
# static-initialization time, so adding a file to the list below is all it takes
# to get it executed.
add_executable(ratlab_tests
    main.cpp
    u8_test.cpp
)

target_link_libraries(ratlab_tests PRIVATE
    ratlab_core
    ratlab_warnings
)

# The whole suite is a single test case for CTest; the Tester reports which of
# its own test cases passed or failed, and returns a non-zero exit code on failure.
add_test(NAME ratlab_tests COMMAND ratlab_tests)
