# =============================================================================
# RatLab — Benchmarks
# =============================================================================
# The benchmark framework is RatLab's own ('Core/Testing/Benchmarker.hpp'), there
# is no third-party dependency to fetch. Every benchmark file registers itself at
# static-initialization time, so adding a file to the list below is all it takes
# to get it measured. The iteration count is calibrated at runtime.
add_executable(ratlab_benchmarks
    main.cpp
    u8_bench.cpp
)

target_link_libraries(ratlab_benchmarks PRIVATE
    ratlab_core
    ratlab_warnings
)
