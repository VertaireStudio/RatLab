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
    ratlab_optimization
)

ratlab_enable_lto(ratlab_tests)

# ---- Optimization -----------------------------------------------------------
# The tests are optimized too, otherwise a failing check would be reporting the
# cost of unoptimized code rather than a defect. What they are *not* given is
# what only pays off inside a measured loop:
#
#   * '-funroll-loops' inflates and duplicates the bodies of every test case to
#     chase a throughput number nobody reads from this binary, and it makes the
#     suite noticeably slower to build.
#   * '-march=native' ties the binary to one CPU, which is worth it for a runner
#     whose numbers are compared against themselves and worthless for a test suite
#     that has to run on whatever machine CI hands it.
#
# '-O2' is asked for explicitly rather than left to 'Release': target options are
# appended after the per-configuration flags, so this one wins over the '-O3'
# that 'Release' would otherwise set. It is the level where the harness and the
# code under test are still fully inlined, without the extra passes that make a
# build slower to produce for no gain here.
if(NOT MSVC)
    target_compile_options(ratlab_tests PRIVATE -O2)
endif()

# The whole suite is a single test case for CTest; the Tester reports which of
# its own test cases passed or failed, and returns a non-zero exit code on failure.
add_test(NAME ratlab_tests COMMAND ratlab_tests)
