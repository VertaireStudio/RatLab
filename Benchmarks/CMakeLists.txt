# =============================================================================
# RatLab — Benchmarks
# =============================================================================

# Use the local copy of Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/benchmark
    ${CMAKE_BINARY_DIR}/benchmark
)

# Placeholder benchmark runner — will be replaced as benchmarks are added
add_executable(ratlab_benchmarks
    main.cpp
)
target_link_libraries(ratlab_benchmarks PRIVATE
    ratlab_core
    benchmark::benchmark
)
target_include_directories(ratlab_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
