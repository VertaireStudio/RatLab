# RatLab — Build Commands

Every command below is run from the repository root. RatLab is header-only with no third-party
dependency, so configuring the workspace is all that is needed before building.

## Requirements

| Requirement | Version |
| ----------- | ------- |
| CMake       | 3.21 or newer |
| Compiler    | C++23, verified with GCC 13.3 |
| Shell       | POSIX `sh`, for the report collector only (see [Reports](#reports)) |

---

## Configure

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

The build directory defaults to `build`. When `CMAKE_BUILD_TYPE` is left out it falls back to
`Release`, since benchmark numbers from a debug build are meaningless. Add `-G <generator>` to
pick a generator other than the default one.

Configuring once is enough: `cmake --build` reconfigures by itself afterwards.

---

## Build everything

```sh
cmake --build build
```

This builds both runners and, as its last step, collects the reports. Add `-j` to build in
parallel.

---

## Tests

| Goal | Command |
| ---- | ------- |
| Build | `cmake --build build --target ratlab_tests` |
| Run | `./build/Tests/ratlab_tests` |
| Run through CTest | `ctest --test-dir build --output-on-failure` |

The test runner registers every test case at startup and returns a non-zero exit code as soon as
one check fails.

| Argument | Effect |
| -------- | ------ |
| `--filter <text>` | Only run the test cases whose name contains `<text>` |
| `--filter=<text>` | Same, without a separate argument |
| `--quiet`, `-q` | Only report the failing test cases |
| `--list` | List the registered test cases without running them |
| `--help`, `-h` | Show the usage text |

```sh
./build/Tests/ratlab_tests --filter u8/arithmetic
./build/Tests/ratlab_tests --quiet
```

---

## Benchmarks

| Goal | Command |
| ---- | ------- |
| Build | `cmake --build build --target ratlab_benchmarks` |
| Run | `./build/Benchmarks/ratlab_benchmarks` |

A full run takes a few seconds: every benchmark is calibrated at runtime until one sample lasts
at least 25 ms, and the fastest of three samples is the one reported. That default is set by the
benchmark framework itself, not by CMake, and is only overridden with `--min-time`.

| Argument | Effect |
| -------- | ------ |
| `--filter <text>` | Only run the benchmarks whose name contains `<text>` |
| `--filter=<text>` | Same, without a separate argument |
| `--min-time <ms>` | Minimum duration of a single sample (default 25 ms) |
| `--list` | List the registered benchmarks without running them |
| `--help`, `-h` | Show the usage text |

```sh
./build/Benchmarks/ratlab_benchmarks --min-time 5
./build/Benchmarks/ratlab_benchmarks --filter u8/bitwise
```

---

## Reports

Both runners write a plain text report of every run, and `Tools/collect_reports.sh` runs them and
files those reports under a name carrying the time of the run. Reports therefore never overwrite
each other and can be compared between two runs.

| Goal | Command |
| ---- | ------- |
| Collect after a build | `cmake --build build` |
| Collect on its own | `cmake --build build --target ratlab_reports` |
| Collect without CMake | `./Tools/collect_reports.sh build` |

`ratlab_reports` depends on both runners, so it builds whatever it still needs before running.
A runner which was not built is skipped and reported as skipped.

| Report | Written to |
| ------ | ---------- |
| Tests | `Misc/ratlab_tests_<date>_<time>.txt` |
| Benchmarks | `Misc/ratlab_benchmarks_<date>_<time>.txt` |

Notes:

- Two runs of the same runner within the same second get a `_2`, `_3`, … suffix rather than
  overwriting each other. If no time is available to name a report after, it keeps its plain
  name and is overwritten by the next run.
- A failing test suite is reported but does **not** fail the build; `ctest` is what gates on
  test results.
- The collector is a POSIX shell tool, so it is only wired into the build on non-Windows hosts.
  On Windows it stays available for a manual run from Git Bash, and
  `-DRATLAB_GENERATE_REPORTS=OFF` keeps it out of the build everywhere.
- Paths are relative to the working directory of the process. The runners are started from the
  repository root by the collector, so the reports always end up in `Misc/`. Running a runner by
  hand from somewhere else writes its report next to you instead.

---

## Options

| Option | Default | Effect |
| ------ | ------- | ------ |
| `RATLAB_BUILD_TESTS` | `ON` | Build the unit tests |
| `RATLAB_BUILD_BENCHMARKS` | `ON` | Build the benchmarks |
| `RATLAB_GENERATE_REPORTS` | `ON` | Collect the run reports into `Misc/` after a build |
| `RATLAB_WARNINGS_AS_ERRORS` | `OFF` | Treat compiler warnings as errors |

```sh
# A workspace with the tests only, warnings as errors, and no report collection.
cmake -S . -B build -DRATLAB_BUILD_BENCHMARKS=OFF \
      -DRATLAB_WARNINGS_AS_ERRORS=ON -DRATLAB_GENERATE_REPORTS=OFF
cmake --build build
```

---

## A typical session

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release   # configure
cmake --build build                              # build, run and file the reports
ctest --test-dir build --output-on-failure       # gate on the test results
./build/Benchmarks/ratlab_benchmarks --min-time 50  # measure more carefully
./Tools/collect_reports.sh build                 # file the reports without a rebuild
```

## What ends up where

| Path | Contents |
| ---- | -------- |
| `build/Tests/ratlab_tests` | The test runner |
| `build/Benchmarks/ratlab_benchmarks` | The benchmark runner |
| `Misc/ratlab_tests_*.txt` | A test report per run |
| `Misc/ratlab_benchmarks_*.txt` | A benchmark report per run |
| `Misc/filelist.txt` | Every file of the workspace, written by `Tools/list_files.sh` |