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

# ---- Optimization options ----------------------------------------------------
# The two runners are compiled with deliberately different flags, because they are
# measured for different things. See 'ratlab_optimization' below and the per-target
# sections in 'Tests/' and 'Benchmarks/' for what each one turns on.
option(RATLAB_ARCH_NATIVE      "Compile for the host CPU (-march=native)"        ON)
option(RATLAB_UNROLL_LOOPS     "Unroll hot loops (-funroll-loops)"               ON)
option(RATLAB_LTO              "Link-time optimization for the runners"           ON)
option(RATLAB_NO_EXCEPTIONS    "Build the runners without exceptions and RTTI"   ON)
option(RATLAB_NATIVE_TOOLCHAIN "Drop the hardening and indirection flags that
                                 distributions add to the compiler defaults"      ON)
option(RATLAB_FAST_MATH        "Relax floating-point semantics (-ffast-math)"    OFF)

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

# ---- Optimization -----------------------------------------------------------
#
# 'Release' already hands the runners '-O3 -DNDEBUG' on GCC/Clang, but that is
# everything it does: it leaves the loop unrolling, the host instruction set and
# the distribution's own overhead flags exactly as the compiler ships them. Those
# leftovers are what decide the reported cost per iteration, so they are set here.
#
# The flags are split in two. This library carries what both runners agree on,
# and each runner adds what is specific to what it is measured for: the tests
# optimize for a fast and honest build, the benchmarks optimize for throughput.
#
# Everything is probed with 'check_cxx_compiler_flag' first. An unsupported flag
# is dropped with a warning instead of failing the configure, so an unknown or
# exotic toolchain still builds the workspace.
include(CheckCXXCompilerFlag)

# Adds a flag to 'target' only when the compiler in use actually understands it.
# A rejected flag is reported once, never silently ignored.
#
# The scope is derived from the target rather than passed in: an INTERFACE
# library has to carry its flags as 'INTERFACE', while a real target that is
# handed 'INTERFACE' would silently apply them only to whatever links against it,
# which is not what a caller of this function ever means.
function(ratlab_add_supported_option target flag)
    string(MAKE_C_IDENTIFIER "HAVE${flag}" flag_variable)
    check_cxx_compiler_flag("${flag}" ${flag_variable})
    if(${flag_variable})
        get_target_property(target_type ${target} TYPE)
        if(target_type STREQUAL "INTERFACE_LIBRARY")
            target_compile_options(${target} INTERFACE "${flag}")
        else()
            target_compile_options(${target} PRIVATE "${flag}")
        endif()
    else()
        message(STATUS "Compiler does not support '${flag}', leaving it out of ${target}")
    endif()
endfunction()

# Turns on link-time optimization for a real target. It is set as a target property
# rather than as a raw flag so that CMake also switches the archiver and the linker
# to their LTO variants, which a compile flag alone would not do.
#
# This cannot live on 'ratlab_optimization': 'INTERPROCEDURAL_OPTIMIZATION' is not
# a transitive usage requirement, so it would never reach the runners. Hence one
# call per runner instead of one flag above.
function(ratlab_enable_lto target)
    if(RATLAB_LTO)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endfunction()

add_library(ratlab_optimization INTERFACE)
add_library(RatLab::Optimization ALIAS ratlab_optimization)

if(MSVC)
    # 'Release' is already '/O2' on MSVC and there is no '/funroll-loops'; the
    # floating-point relaxation is the one lever that exists there.
    if(RATLAB_FAST_MATH)
        ratlab_add_supported_option(ratlab_optimization "/fp:fast")
    endif()
    if(RATLAB_NO_EXCEPTIONS)
        ratlab_add_supported_option(ratlab_optimization "/GR-")
        ratlab_add_supported_option(ratlab_optimization "/EHs-c-")
    endif()
else()
    if(RATLAB_FAST_MATH)
        # Off by default: it changes the result of floating-point arithmetic, which
        # is a decision for the benchmarks that measure it, not a default.
        ratlab_add_supported_option(ratlab_optimization "-ffast-math")
    endif()

    if(RATLAB_NO_EXCEPTIONS)
        # Nothing in RatLab throws, catches or downcasts, so the unwinder and the
        # type information are pure cost. Dropping them shrinks every frame the
        # optimizer has to reason about.
        ratlab_add_supported_option(ratlab_optimization "-fno-exceptions")
        ratlab_add_supported_option(ratlab_optimization "-fno-rtti")
    endif()

    if(RATLAB_NATIVE_TOOLCHAIN)
        # Distributions and toolchain vendors add these on their own: Ubuntu's GCC
        # defaults to '-fstack-protector-strong' and '-fcf-protection=full', and
        # every call into a shared library goes through the PLT by default. None of
        # them buy anything for a private benchmark runner, and each one puts work
        # in front of the loop being measured.
        ratlab_add_supported_option(ratlab_optimization "-fno-stack-protector")
        ratlab_add_supported_option(ratlab_optimization "-fcf-protection=none")
        ratlab_add_supported_option(ratlab_optimization "-fno-plt")
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
