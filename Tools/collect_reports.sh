# Report Collector
# Runs the unit tests and the benchmarks of a build tree, then files their reports into the
# Misc directory. Every report is named after the time of its run, so that two runs never
# overwrite each other and can be compared afterwards.
# Usage: Tools/collect_reports.sh [build directory]
# Note: The build directory defaults to 'build', relative to the repository root.
# Note: The script is plain POSIX shell, since the build runs it through '/bin/sh'.

#!/bin/sh
set -u

root="$(cd "$(dirname "$0")/.." && pwd)"
build="${1:-$root/build}"
misc="$root/Misc"

# Reports whether something went wrong and stops the script.
fail() {
    echo "  error: $1" >&2
    exit 1
}

# Locates an executable inside the given directory of the build tree. A single configuration
# generator keeps it directly in there, a multi configuration generator adds one more level
# for the configuration ('Debug', 'Release', ...).
# Arguments: <directory> <executable>
locate() {
    if [ -x "$build/$1/$2" ]; then
        printf '%s\n' "$build/$1/$2"
        return 0
    fi

    found="$(find "$build/$1" -mindepth 2 -maxdepth 2 -type f -name "$2" 2>/dev/null |
        sort | head -n 1)"
    if [ -n "$found" ] && [ -x "$found" ]; then
        printf '%s\n' "$found"
        return 0
    fi

    return 1
}

# Runs the given runner from the repository root and files the report it writes.
# Arguments: <executable> <report file name> <label>
collect() {
    local executable="$1"
    local report="$2"
    local label="$3"

    # The runners write their report relative to the working directory, so they are started
    # from the repository root: the report always lands inside Misc/. A failing suite still
    # writes its report, so a non-zero exit code is kept instead of stopping here.
    local status=0
    (cd "$root" && "$executable") || status=$?

    local source="$misc/$report"
    [ -f "$source" ] || fail "'$executable' did not write Misc/$report"

    # The time of the run keeps two reports of the same runner apart. Without it the report
    # keeps its own name and is overwritten by the next run.
    local stamp
    stamp="$(date +%Y-%m-%d_%H-%M-%S 2>/dev/null || true)"
    if [ -n "$stamp" ]; then
        local name="${report%.txt}_$stamp"
        local target="$misc/$name.txt"
        # Two runs of the same runner within the same second would land on the same name, so
        # the first free one is taken instead of overwriting the report of the earlier run.
        local attempt=2
        while [ -e "$target" ]; do
            target="$misc/${name}_$attempt.txt"
            attempt=$((attempt + 1))
        done

        mv -- "$source" "$target" || fail "could not file '$source'"
        echo "  $label  Misc/$(basename "$target")"
    else
        echo "  $label  Misc/$report (kept, no time available to name it after)"
    fi

    if [ "$status" -ne 0 ]; then
        echo "  $label exited with $status, see the report for the failing checks"
    fi
}

# Collects the runner of the given directory, or reports that it was not built. A workspace
# with only one of the two runners enabled is therefore still collected.
# Arguments: <label> <directory> <executable> <report file name>
collect_runner() {
    local path
    path="$(locate "$2" "$3")" || {
        echo "  $1 skipped, '$build/$2/$3' was not built"
        return 1
    }

    collect "$path" "$4" "$1"
}

[ -d "$build" ] || fail "'$build' is not a directory"
mkdir -p -- "$misc" || fail "could not create '$misc'"

collected=0
if collect_runner "tests     " "Tests" "ratlab_tests" "ratlab_tests.txt"; then
    collected=$((collected + 1))
fi
if collect_runner "benchmarks" "Benchmarks" "ratlab_benchmarks" "ratlab_benchmarks.txt"; then
    collected=$((collected + 1))
fi

[ "$collected" -gt 0 ] || fail "no runner was found under '$build'"