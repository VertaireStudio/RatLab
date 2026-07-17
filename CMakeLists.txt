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
option(RATLAB_BUILD_TESTS       "Build unit tests"                    ON)
option(RATLAB_BUILD_BENCHMARKS  "Build benchmarks"                    OFF)
option(RATLAB_BUILD_SHARED      "Build as shared library"             OFF)
option(RATLAB_DEV_BUILD         "Enable dev-mode (test_inline, etc.)" ON)
option(RATLAB_CONSTEVAL         "Force consteval for func macro"      OFF)

# ---- C++ Standard -----------------------------------------------------------
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# ---- Visibility -------------------------------------------------------------
set(CMAKE_CXX_VISIBILITY_PRESET      "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN  ON)

# Honor visibility properties for all target types (including FetchContent)
cmake_policy(SET CMP0063 NEW)

# ---- Compiler configuration -------------------------------------------------
include(cmake/CompilerConfig.cmake)

# ---- Core library target ----------------------------------------------------
#
# The majority of RatLab is header-only.  We model it as an INTERFACE library
# so consuming projects can simply link against `ratlab_core` and get all
# include directories and compile definitions automatically.
#
# The single translation unit (type_conversion.cpp) is added separately below
# and merged into the same logical target via a helper OBJECT library.

# 1) OBJECT library for the single .cpp file
add_library(ratlab_core_obj OBJECT
    Core/Types/type_conversion.cpp
)
target_include_directories(ratlab_core_obj PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(ratlab_core_obj PRIVATE
    $<$<BOOL:${RATLAB_DEV_BUILD}>:COMPILE_DEV>
    $<$<BOOL:${RATLAB_CONSTEVAL}>:COMPILE_CONSTEVAL>
)
# Pass the C++ version as the CXX macro (#define CXX 20 etc.)
target_compile_definitions(ratlab_core_obj PRIVATE
    CXX=${CMAKE_CXX_STANDARD}
)

# 2) INTERFACE library carrying all header-only content
add_library(ratlab_core INTERFACE)
target_include_directories(ratlab_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(ratlab_core INTERFACE
    $<$<BOOL:${RATLAB_DEV_BUILD}>:COMPILE_DEV>
    $<$<BOOL:${RATLAB_CONSTEVAL}>:COMPILE_CONSTEVAL>
    CXX=${CMAKE_CXX_STANDARD}
)
target_link_libraries(ratlab_core INTERFACE
    ratlab_core_obj
)

# 3) Alias for convenient use from subdirectories
add_library(RatLab::Core ALIAS ratlab_core)

# ---- Tests ------------------------------------------------------------------
if(RATLAB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

# ---- Benchmarks -------------------------------------------------------------
if(RATLAB_BUILD_BENCHMARKS)
    add_subdirectory(Benchmarks)
endif()

# ---- Install ----------------------------------------------------------------
include(GNUInstallDirs)

# Headers
install(DIRECTORY Core/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RatLab/Core
    FILES_MATCHING PATTERN "*.hpp"
)

# Library (only when we later add a compiled library; for now the OBJECT
# library is consumed directly by the test/benchmark targets.)
install(TARGETS ratlab_core_obj
    EXPORT  RatLabTargets
    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# CMake package config
install(EXPORT RatLabTargets
    FILE            RatLabTargets.cmake
    NAMESPACE       RatLab::
    DESTINATION     ${CMAKE_INSTALL_LIBDIR}/cmake/RatLab
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RatLabConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/RatLabConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RatLab
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/RatLabConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RatLab
)