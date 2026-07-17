# =============================================================================
# RatLab — Compiler Configuration
# =============================================================================
#
# This module sets compiler-specific flags for all targets in the project.
# It is included from the root CMakeLists.txt before any targets are defined.

# ---- GCC / Clang ------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        # Warnings to ignore
        -Wno-infinite-recursion
    )
endif()

# ---- MSVC -------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /utf-8)
endif()

# ---- Shared-library specific ------------------------------------------------
if(RATLAB_BUILD_SHARED)
    add_compile_definitions(RATLAB_BUILD_DLL)
endif()