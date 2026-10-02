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

`Release` is not the whole story for a benchmark build: the runners additionally get their own
optimization flags, which are worth several times the throughput. See
[Optimization](#optimization).

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
| Run quickly | `./build/Benchmarks/ratlab_benchmarks --quick` |

The benchmark framework is RatLab's own (`Core/Testing/Benchmarker.hpp`), modeled after
Criterion, the benchmark tool of Rust. A benchmark file holds one group function, `main.cpp`
names it in a `BENCHMARK_GROUP`, and `BENCHMARK_MAIN` starts the run — so a benchmark is
registered when the run reaches it, after the command line has been read, and a run can be
filtered, shortened or turned into a comparison without any of it being written into the
benchmarks themselves.

Every benchmark is measured in the three phases Criterion uses. The amount of iterations a
single sample performs is calibrated first, until a batch is long enough to be timed at all.
The benchmark is then warmed up, so the caches and the clock of the machine have settled on
the work. Only then are the samples taken, and all of them are kept: every estimate in the
report is built out of them, which is why every one of them comes with an interval and why a
run can be compared against another one instead of only being read.

The u8 suite takes about 13 seconds at the defaults below: 0.8 s per benchmark for the warm-up
and the samples together. Criterion's own 3 s of warm-up and 5 s of measurement are two
arguments away, at roughly ten times the time, and are worth it before a claim is made about a
few percent.

| Argument | Effect |
| -------- | ------ |
| `--bench <text>` | Only run the benchmarks whose id contains `<text>` |
| `--filter=<text>` | Same, without a separate argument |
| `--list` | List the benchmarks without measuring them |
| `--warm-up-time <time>` | Warm-up per benchmark, e.g. `300ms` (default 0.3 s) |
| `--measurement-time <time>` | Measurement per benchmark (default 0.5 s) |
| `--sample-size <count>` | Samples per benchmark (default 100) |
| `--nresamples <count>` | Resamples behind every interval (default 1000) |
| `--confidence-level <level>` | Share of the distribution an interval covers (default 0.95) |
| `--noise-threshold <fraction>` | Share of changes called noise (default 0.01) |
| `--significance-level <level>` | Likelihood a difference is called noise (default 0.05) |
| `--save-baseline <file>` | Write the samples of this run into `<file>` |
| `--load-baseline <file>` | Compare this run against the samples in `<file>` |
| `--comparison-only` | Compare against the baseline, report nothing else |
| `--quick` | Warm up, measure and sample less (10 samples, 0.1 s, 0.2 s) |
| `--verbose` | Print every sample of every benchmark |
| `--quiet`, `-q` | Print only what each benchmark costs |
| `--help`, `-h` | Show the usage text |

```sh
./build/Benchmarks/ratlab_benchmarks --filter u8/bitwise
./build/Benchmarks/ratlab_benchmarks --warm-up-time 3s --measurement-time 5s
./build/Benchmarks/ratlab_benchmarks --list
```

`--quick` is a set of defaults rather than an override: anything spelled out next to it wins,
so `--quick --sample-size 25` is a short run of 25 samples.

### Comparing two runs

A baseline is the raw samples of a run, saved as plain text. A later run is compared against
it, and the comparison is what tells a change from noise: the samples of both runs are
resampled together and the change is reported with the probability of it being noise, so a
0.4% difference on a quiet machine is reported as "no change in performance" and a 5% one is
not.

```sh
# Before a change: keep what the code does now.
./build/Benchmarks/ratlab_benchmarks --save-baseline Misc/u8_base.txt

# After it: measure the same way and compare.
./build/Benchmarks/ratlab_benchmarks --load-baseline Misc/u8_base.txt
```

| Line of a report | What it is |
| ---------------- | ---------- |
| `time` | The cost per iteration, with a 95% interval around it, and the iterations it is per |
| `thrpt` | What that cost works out to per second, for the configured throughput |
| `median` | The middle of the samples with an interval, then how far they sit around the average and around the middle |
| `outliers` | How many samples sat too far from the middle of the rest to belong to it, and the one which reached furthest |
| `sample` | One sample, listed with `--verbose`, marked when it is an outlier |
| `change` | The change against the baseline, and the probability of it being noise |

A benchmark takes three lines without a baseline and five with one. The values line up in one
column under a 9-wide label, every line of every report fits in 74 columns, and the numbers
are the only thing in bold or coloured.

Notes:

- A benchmark whose work the optimizer removed leaves samples shorter than the clock can
  resolve. Those are reported as `faster than the clock resolves` rather than quoted as a
  cost, and they are left out of the fastest benchmark of the summary.
- Compare numbers only against numbers taken the same way: a different compiler, a different
  flag or a different amount of warm-up makes two runs incomparable, however much they look
  alike.

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
- A benchmark report is the whole block of every benchmark, with its statistics, its outliers
  and, against a baseline, its comparison — the text above is the same without the colors.
  The baseline itself is a separate file, written by `--save-baseline`, and holds the raw
  samples rather than the estimates taken from them.
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
| `RATLAB_ARCH_NATIVE` | `ON` | Compile the benchmarks for the host CPU (`-march=native`) |
| `RATLAB_UNROLL_LOOPS` | `ON` | Unroll the benchmark loops (`-funroll-loops`) |
| `RATLAB_LTO` | `ON` | Link-time optimization for both runners |
| `RATLAB_NO_EXCEPTIONS` | `ON` | Build the runners without exceptions and RTTI |
| `RATLAB_NATIVE_TOOLCHAIN` | `ON` | Drop the hardening flags distributions add by default |
| `RATLAB_FAST_MATH` | `OFF` | Relax floating-point semantics (`-ffast-math`) |

```sh
# A workspace with the tests only, warnings as errors, and no report collection.
cmake -S . -B build -DRATLAB_BUILD_BENCHMARKS=OFF \
      -DRATLAB_WARNINGS_AS_ERRORS=ON -DRATLAB_GENERATE_REPORTS=OFF
cmake --build build
```

---

## Optimization

The two runners are compiled with different flags on purpose: the benchmarks are
measured for throughput and the tests are not.

| Target | Optimization level | Extra flags |
| ------ | ------------------ | ----------- |
| `ratlab_benchmarks` | `-O3` | `-funroll-loops -march=native -mtune=native` |
| `ratlab_tests` | `-O2` | none |

Both get link-time optimization, `-fno-exceptions -fno-rtti`, and the drop of the
hardening flags a distribution adds to the compiler defaults
(`-fno-stack-protector -fcf-protection=none -fno-plt`). On Ubuntu, GCC enables
`-fstack-protector-strong` and `-fcf-protection=full` by default; both put work in
front of the loop being measured.

`-funroll-loops` is aimed at the shape these bodies have: a counted loop around a handful of
instructions is what the unroller collapses the most. It is kept because it costs nothing and
because the float and vector benchmarks to come are the shape it does collapse. It is not what
decides whether a measurement means anything: an unrolled loop and a rolled one both hoist the
same work, which is what the [next section](#keeping-the-benchmarks-honest) is about.

Notes:

- Every flag is probed with `check_cxx_compiler_flag` first. An unsupported one is
  left out with a message during configure, instead of breaking the build. `-march=native`
  is additionally skipped when the build targets another machine
  (`CMAKE_CROSSCOMPILING`).
- Turn the host tuning off for a set of numbers meant to be reproducible on other
  hardware: `-DRATLAB_ARCH_NATIVE=OFF`.
- `RATLAB_FAST_MATH` is off by default. It does change the result of floating-point
  arithmetic, which is a decision to make for the benchmarks that measure it rather
  than a default to adopt.
- On MSVC most of this does not apply: `Release` is already `/O2`, there is no
  loop unrolling flag, and `-march=native` has no equivalent. `RATLAB_FAST_MATH` and
  `RATLAB_NO_EXCEPTIONS` are the only levers there.

---

## Keeping the benchmarks honest

The flags above decide how fast the measured code runs. They do not decide whether there is
any measured code left to run, which is a separate problem with its own tools in
`Benchmarker`. Both of them are easy to get wrong in a way that does not show up: a body
whose work the optimizer deleted reports a very small number, and it reports it confidently,
every run, with an interval around it.

### The fold that is easy to see

`Benchmarker::do_not_optimize` keeps a *result* alive. It cannot keep a *computation* from
happening at compile time in the first place. A body written as:

```cpp
const u8 first(37), second(5);
for (...) Benchmarker::do_not_optimize(first + second);
```

has both operands as literals, so `first + second` is a literal too. There is nothing left for
the barrier to protect: the loop ends up timing the barrier and nothing else.

`Benchmarker::black_box` is the fix for that, and it is free. It hands the value to the
compiler through an empty assembly statement and gets it back, so the compiler has to
materialize the value and cannot know where it came from:

```cpp
const u8 first = Benchmarker::black_box<u8>((unsigned char)37);
const u8 second = Benchmarker::black_box<u8>((unsigned char)5);
```

On MSVC, which has no inline assembly to ask, `black_box` falls back to the round trip through
a volatile cell, which is not free. A measurement taken with MSVC therefore carries that
barrier in it, and is not comparable with one taken with GCC or Clang.

### The barrier which was not one

`black_box` is an assembly statement with the value as both its input and its output, and it
says `volatile`. Both halves of that are load-bearing, and the first one is the easy mistake to
make:

```cpp
asm volatile("" : "+rm"(value) : :);   // keeps the work, and keeps it in the loop
asm        ("" : "+rm"(value) : :);   // neither: the whole statement is dead code
```

An assembly statement which is not volatile is deleted as soon as nothing reads its output, and
a benchmark is nothing *but* unused results: the result of the last iteration of a sample is
thrown away, and so is any result no body looks at. What the compiler is then left with is a
body whose only remaining side effect is the counter it carries to make its operands differ —
which is exactly the loop and nothing else.

Measured on GCC 13.3 with the `u8/arithmetic` body, ten operator overloads including four
divides:

| `black_box` | `u8/arithmetic` | The same body without a counter |
| ----------- | --------------- | ------------------------------ |
| plain assembly statement | 1.88 ns/iter | 0.27 ns/iter |
| `volatile` assembly statement | 7.71 ns/iter | 6.39 ns/iter |

The 0.27 ns is not the arithmetic, it is the cost of the counter and the barrier around the
result — measured on its own by a body which does nothing else, and the one number in a report
which says what the framework costs.

A volatile statement is also what keeps the work inside the loop, so the bodies do not depend on
their counter to survive. They carry one anyway: a divide by a value which never changes is a
divide the compiler can reason about in a way it cannot reason about a real one. The counter is
not what keeps a body alive; it is what keeps its operands the ones a caller would pass.

### The predicate which is constant

Neither barrier can help a predicate which answers the same thing every time, because there is
nothing to keep: the compiler proves the answer while the workspace is being compiled and the
predicate is gone. The three `can_*` helpers of `Variant` are written as `!a || !b` over
conditions that cannot both hold, which makes every accessability readable, writable and
overwritable:

```cpp
func bool can_read() const noexcept { return !is_write_only() || !is_neutral(); }   // always true
```

`u8/accessability/queries` varies the accessability of its value every iteration anyway, so
that it measures the queries as soon as there are any to measure, and reports what it measures
today. `Tests/u8_test.cpp` pins the permissive behaviour down in
`u8/accessability/permissive_helpers`, so a benchmark reporting a guard cost means the guard
exists.

### What this does to the numbers

It makes them larger, and that is the point. A benchmark whose work the optimizer deleted was
not reporting a fast operation, it was reporting an empty loop. Compare numbers only against
numbers taken the same way: a report written before this change measures the loop, and one
written after it measures the operations.

A benchmark can also end up measuring the framework rather than the type under it. The
framework's own cost is 0.28 ns per iteration — the counter of the body, the loop, and the
`black_box` around the value it returns — and `u8` is two bytes wide, so every body in the
suite sits within a factor of a few of it. Measured on GCC 13.3:

| Benchmark | Cost per iteration |
| --------- | ------------------ |
| `u8/arithmetic` (ten overloads, four divides) | 7.07 ns |
| `u8/bitwise` (three operators) | 2.89 ns |
| `u8/math/min_max` (two helpers) | 2.41 ns |
| `u8/arithmetic/accumulated` (three chained) | 2.33 ns |
| `u8/set` | 1.96 ns |
| `u8/get` | 1.92 ns |
| the loop and its barriers alone | 0.28 ns |

Only the multi-operation bodies are far enough above the floor to say much about the operations
they are made of. The single-operation ones are honest as differences from each other — a store
costs more than a shift, a guarded divide more than a copy — and nothing more. A number in that
band is a statement about the body, not about `u8`.

### Known limitation

`u8/arithmetic/increment` measures a chain of increments rather than the `++value; --value;`
pair it used to. That pair is the identity for every possible input: it reads and writes twice
to arrive where it started, which is a real pair of operations to measure and a poor description
of what the benchmark is about. The chain is one increment per iteration instead. It is the one
body in the suite that measures a different thing from the one its name used to describe, and
it says so where it is written.

---

## A typical session

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release   # configure
cmake --build build                              # build, run and file the reports
ctest --test-dir build --output-on-failure       # gate on the test results
./build/Benchmarks/ratlab_benchmarks --measurement-time 2s  # measure more carefully
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