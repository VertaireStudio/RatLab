cmake_minimum_required(VERSION 3.21)

# =============================================================================
# RatLab Game Engine — CMake Build System
# =============================================================================
project(RatLab
    VERSION     0.1.0
    DESCRIPTION "RatLab Game Engine"
    HOMEPAGE_URL "https://github.com/VertaireStudio/RatLab"
    LANGUAGES   CXX
)

# ---- Options ----------------------------------------------------------------
option(RATLAB_BUILD_TESTS       "Build the unit tests"                  ON)
option(RATLAB_BUILD_BENCHMARKS  "Build the benchmarks"                  ON)
option(RATLAB_GENERATE_REPORTS  "Collect the run reports into Misc"     ON)
option(RATLAB_WARNINGS_AS_ERRORS "Treat compiler warnings as errors"   OFF)

# ---- C++ Standard -----------------------------------------------------------
# The engine is built as C++23. 'platform_configs.hpp' detects this from
# '__cplusplus' at compile time, so the two have to stay in sync.
set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# Default to an optimized build, benchmarks are meaningless in a debug build.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# ---- Warnings ---------------------------------------------------------------
# Applied to every target of the workspace, so that a warning introduced anywhere
# is reported while building the tests or the benchmarks.
add_library(ratlab_warnings INTERFACE)
if(MSVC)
    target_compile_options(ratlab_warnings INTERFACE /W4)
    if(RATLAB_WARNINGS_AS_ERRORS)
        target_compile_options(ratlab_warnings INTERFACE /WX)
    endif()
else()
    target_compile_options(ratlab_warnings INTERFACE -Wall -Wextra -Wpedantic)
    if(RATLAB_WARNINGS_AS_ERRORS)
        target_compile_options(ratlab_warnings INTERFACE -Werror)
    endif()
endif()

# ---- Core library target ----------------------------------------------------
#
# RatLab is header-only: every type lives in a header and the test/benchmark
# frameworks are headers as well. The core is therefore modeled as an INTERFACE
# library, which carries the include directory and the compile settings to
# everything linking against it.
add_library(ratlab_core INTERFACE)
add_library(RatLab::Core ALIAS ratlab_core)

target_include_directories(ratlab_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(ratlab_core INTERFACE cxx_std_23)

# ---- Tests ------------------------------------------------------------------
if(RATLAB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

# ---- Benchmarks -------------------------------------------------------------
if(RATLAB_BUILD_BENCHMARKS)
    add_subdirectory(Benchmarks)
endif()

# ---- Reports ---------------------------------------------------------------
# 'Tools/collect_reports.sh' runs the runners above and files their reports into 'Misc'.
#
# It hangs off a custom target of its own rather than a POST_BUILD step on each executable:
# a POST_BUILD step would fire once per executable, so the collectors would race each other
# and each of them could start before the other executable had even been linked. Depending on
# the executables instead makes the collector run exactly once, and only after everything it
# needs is in place.
#
# The collector is a POSIX shell tool, so it is only wired into the build where the shell is
# there to run it. On a Windows host it stays available for a manual run from Git Bash, and
# the new option keeps it out of the build everywhere.
set(RATLAB_REPORT_RUNNERS)
if(RATLAB_BUILD_TESTS)
    list(APPEND RATLAB_REPORT_RUNNERS ratlab_tests)
endif()
if(RATLAB_BUILD_BENCHMARKS)
    list(APPEND RATLAB_REPORT_RUNNERS ratlab_benchmarks)
endif()

if(RATLAB_GENERATE_REPORTS AND RATLAB_REPORT_RUNNERS AND NOT CMAKE_HOST_WIN32)
    add_custom_target(ratlab_reports ALL
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/Tools/collect_reports.sh" "${CMAKE_BINARY_DIR}"
        DEPENDS ${RATLAB_REPORT_RUNNERS}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        USES_TERMINAL
        COMMENT "Collecting the test and benchmark reports into Misc"
        VERBATIM
    )
endif()
