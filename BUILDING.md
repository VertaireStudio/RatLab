# Building RatLab

## Prerequisites

- **CMake** ≥ 3.21
- **C++20** compatible compiler (GCC, Clang, MSVC, AppleClang)
- **Git** (for fetching test/benchmark dependencies via FetchContent)

## Quick Start

```bash
# Configure the project (creates the build directory)
cmake -B build

# Build everything
cmake --build build

# Run the tests
ctest --test-dir build
```

## CMake Options

All options are booleans (`ON` / `OFF`). Pass them with `-D<OPTION>=<VALUE>` during configuration, for example:

```bash
cmake -B build -DRATLAB_BUILD_BENCHMARKS=ON
```

| Option | Default | Description |
|--------|---------|-------------|
| `RATLAB_BUILD_TESTS` | `ON` | Build the unit test suite (uses doctest). |
| `RATLAB_BUILD_BENCHMARKS` | `OFF` | Build performance benchmarks (uses Google Benchmark). |
| `RATLAB_BUILD_SHARED` | `OFF` | Build `ratlab_core_obj` as part of a shared library. Sets the `RATLAB_BUILD_DLL` preprocessor definition. |
| `RATLAB_DEV_BUILD` | `ON` | Enable development-mode features. Defines the `COMPILE_DEV` preprocessor macro. |
| `RATLAB_CONSTEVAL` | `OFF` | Force `consteval` for the `func` macro. Defines the `COMPILE_CONSTEVAL` preprocessor macro. |

## Build Types

CMake's standard build types are supported:

```bash
# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# RelWithDebInfo
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo

# MinSizeRel
cmake -B build -DCMAKE_BUILD_TYPE=MinSizeRel
```

## Build Targets

| Target | Description |
|--------|-------------|
| `ratlab_core` | The header-only core library (INTERFACE target). Link against this to use RatLab in your own project. |
| `ratlab_core_obj` | Object library containing the single compiled translation unit (`Core/Types/type_conversion.cpp`). Linked automatically by `ratlab_core`. |
| `RatLab::Core` | Alias for `ratlab_core`. |
| `ratlab_tests` | Unit test executable (built only when `RATLAB_BUILD_TESTS=ON`). |
| `ratlab_benchmarks` | Benchmark executable (built only when `RATLAB_BUILD_BENCHMARKS=ON`). |

## Running Tests

```bash
# Configure with tests (enabled by default)
cmake -B build

# Build and run tests
cmake --build build
ctest --test-dir build --output-on-failure

# Or run the test executable directly
./build/Tests/ratlab_tests
```

## Running Benchmarks

```bash
# Configure with benchmarks enabled
cmake -B build -DRATLAB_BUILD_BENCHMARKS=ON

# Build
cmake --build build

# Run the benchmark executable
./build/Benchmarks/ratlab_benchmarks
```

## Installation

```bash
cmake --install build --prefix /path/to/install
```

This installs:
- Header files to `<prefix>/include/RatLab/Core/`
- The object library to `<prefix>/lib/`
- CMake package configuration files to `<prefix>/lib/cmake/RatLab/`

Downstream projects can then find RatLab with:

```cmake
find_package(RatLab REQUIRED)
target_link_libraries(my_app PRIVATE RatLab::Core)
```

## Using RatLab as a Subdirectory

You can also embed RatLab directly in another CMake project:

```cmake
add_subdirectory(path/to/RatLab)
target_link_libraries(my_app PRIVATE RatLab::Core)
```

## Notes

- RatLab is **mostly header-only**; only `Core/Types/type_conversion.cpp` is compiled separately.
- The C++ standard is hardcoded to **C++20** in `CMakeLists.txt`.
- Compiler warning flags are set in `cmake/CompilerConfig.cmake`:
  - GCC/Clang: `-Wall -Wextra -Wpedantic`
  - MSVC: `/W4 /utf-8`
- Dependencies (doctest, Google Benchmark) are fetched automatically via `FetchContent` at configure time.