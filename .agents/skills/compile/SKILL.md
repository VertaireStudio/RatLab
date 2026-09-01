---
name: compile
description: Compiles the RatLab workspace.
---

# compile

Before running commands, make sure your terminal is pointed at the RatLab directory.

`//...` All targets inside the terminal's path, can be replaced for specific builds

`bazel build //...` Build targets (default C++20)
`bazel build cpp17 //...` Build targets with C++17
`bazel build cpp23 //...` Build targets with C++23
`bazel build cpp26 //...` Build targets with C++26
`bazel build //:ratlab` Build only the core library
`bazel build --keep_going //...` Build targets, report errors but don't stop on first failure

`bazel build //Tests:math_funcs_test` Compile the test source file
`bazel-bin/Tests/math_funcs_test` After compiling, run the test
`bazel test //...` Run tests without a visual header

`bazel build //Benchmarks:math_funcs_bench` Compile the benchmark source file
`bazel-bin/Benchmarks/math_funcs_bench` After compiling, run the benchmark 

`bazel query //...` List targets in the workspace
`bazel query 'kind("cc_.*", //...)'` List only C++ targets
`bazel query 'deps(//:ratlab)'` Show transitive dependencies of core
`bazel query 'somepath(//Tests:..., //Core:core)'` Find dependency path between targets

`bazel clean` Remove build output symlinks and intermediate files
`bazel clean --expunge` Full clean, removes entire output base

`--config=ci` Echo which rc options were applied
`-k` / `--keep_going` Don't stop on first error
`-j N` Limit parallel jobs to N, where N is the amount of processor threads
`--subcommands` Print the shell commands Bazel runs

## Usage

Run any of the above commands into the terminal to compile the RatLab workspace in any way you prefer.
If you want to compile tests, you can either use the header version (e.g: `bazel build //Tests:math_funcs_test`) for detailed test results or the headless version (`bazel test //Tests:math_funcs_test`) for only summarizing the end result, depending on the task.

## Steps

1. Select one of the above commands which is needed for the type of compilation you need for the task.
2. Run the command in the terminal.
3. Wait for the command to finish. However if the command didn't finish within 5 minutes, then automatically cancel that command.
4. If all of the above steps are finished, then this skill is completed.

## Common Problems

- What if the command doesn't run because Bazel is not installed on the system?
If you are NOT in YOLO mode: Ask the user for permission to install Bazel into the system, with a question tool. If the user declines this request, then this skill is completed- without question.
If you are in YOLO mode: You are automatically granted permission to install Bazel into the system.

In both cases- to install Bazel, execute the following steps:
1. For Windows: Go to this [Installation Page](https://bazel.build/install/windows) and set up Bazelisk.
1. For Linux (Ubuntu): Go to this [Installation Page](https://bazel.build/install/ubuntu) and set up Bazelisk.
1. For MacOS: Go to this [Installation Page](https://bazel.build/install/os-x) and set up Bazelisk.
2. Once Bazelisk is set up successfully, run Bazelisk. This will install Bazel and all the other requirements into the system.
3. Once all of the above steps are finished respectively, then this problem is solved.

- What if the command doesn't run because parts of the command is incorrect?
Then follow these steps in numerical order:
1. Check the skill's context again to verify what parts of the used command is wrong.
2. Check the available text files in the workspace to see how the command should run.
3. If possible: Search online for the exact command and identify solutions based on the findings of the command's usage, problem and potential solution(s).
4. Ask the user for possible solutions for this problem and wait for it's answer.
5. If all of the above steps have been finished without solving the actual problem, then this problem is unsolvable.