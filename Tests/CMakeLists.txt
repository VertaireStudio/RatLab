# =============================================================================
# RatLab — Unit Tests
# =============================================================================

# Use the local copy of doctest (header-only, single-header test framework)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/doctest
    ${CMAKE_BINARY_DIR}/doctest
)

# Placeholder test runner — will be replaced as tests are added
add_executable(ratlab_tests
    main.cpp
)
target_link_libraries(ratlab_tests PRIVATE
    ratlab_core
    doctest::doctest
)
target_include_directories(ratlab_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(NAME ratlab_tests COMMAND ratlab_tests)
