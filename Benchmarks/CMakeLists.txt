# =============================================================================
# RatLab — Benchmarks
# =============================================================================
# The benchmark framework is RatLab's own ('Core/Testing/Benchmarker.hpp'), modeled
# after Criterion, and there is no third-party dependency to fetch. A benchmark file
# holds one group function, which 'main.cpp' names in a 'BENCHMARK_GROUP', so adding
# a file to the list below and naming its group is all it takes to get it measured.
# Every benchmark is calibrated, warmed up and sampled at runtime.
add_executable(ratlab_benchmarks
    main.cpp
    u8_bench.cpp
)

target_link_libraries(ratlab_benchmarks PRIVATE
    ratlab_core
    ratlab_warnings
    ratlab_optimization
)

# Applied here rather than on 'ratlab_optimization', which cannot carry it.
ratlab_enable_lto(ratlab_benchmarks)

# ---- Optimization -----------------------------------------------------------
# This is the binary whose numbers are reported as a cost per iteration, so it is
# the one that gets the flags aimed at throughput rather than at build speed.
#
# '-funroll-loops' is aimed at the shape these bodies have: a counted loop
# around a handful of instructions is what the unroller collapses the most. It
# used to be worth about 3.3x over the u8 suite, and it no longer is: with the
# bodies carrying an operand which changes every iteration (see
# 'Benchmarks/u8_bench.cpp') there is no loop-invariant work left to collapse,
# and two runs of the suite with and without it differ by less than the spread
# between two runs of the same build. It is kept because it costs nothing and the
# float and vector benchmarks to come are the shape it does collapse, not
# because the u8 suite is waiting on it.
#
# '-march=native' schedules for the CPU that is actually running the benchmark
# instead of the generic baseline the compiler assumes. It is neutral on the
# integer u8 suite, which the machine's out-of-order core already saturates, but
# it is what lets the float and vector benchmarks to come use the wider
# registers. Turn it off with '-DRATLAB_ARCH_NATIVE=OFF' for a run whose numbers
# should be reproducible on other hardware.
if(NOT MSVC)
    # 'Release' already brings '-O3'; restated here so the level is decided in one
    # place rather than inherited from the build type. Target options come after
    # the per-configuration flags, so this is the one that applies.
    target_compile_options(ratlab_benchmarks PRIVATE -O3)

    if(RATLAB_UNROLL_LOOPS)
        ratlab_add_supported_option(ratlab_benchmarks "-funroll-loops")
    endif()

    # Guarded rather than assumed: '-march=native' is rejected outright when the
    # build targets a machine other than the one running CMake.
    if(RATLAB_ARCH_NATIVE AND NOT CMAKE_CROSSCOMPILING)
        ratlab_add_supported_option(ratlab_benchmarks "-march=native")
        ratlab_add_supported_option(ratlab_benchmarks "-mtune=native")
    endif()
endif()
