/************************************/
/*         benchmarker.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Core/Essentials/essentials.hpp"
#include <chrono>
#include <cstdio>
#include <vector>

#if WINDOWS_ENABLED
#include <io.h>
#define BENCHMARKER_ISATTY(stream) (_isatty(_fileno(stream)) != 0)
#else
#include <unistd.h>
#define BENCHMARKER_ISATTY(stream) (::isatty(::fileno(stream)) != 0)
#endif

struct Benchmarker {
    private:
    struct BenchResult {
        const char* name;
        u32 iteration_count;
        f64 total_ns;
    };

    struct GroupData {
        const char* name;
        u32 start_index;
        u32 end_index;
    };

    std::vector<BenchResult> results;
    std::vector<GroupData> groups;
    u32 benchmarks_run;
    bool use_color;
    f64 duration_ns;

    static constexpr const u32 WARMUP_ITERATIONS = 100u;
    static constexpr const u32 SAMPLE_INTERVAL = 256u;
    static constexpr const i32 COL_NAME = 20;
    static constexpr const i32 COL_ITERS = 14;
    static constexpr const i32 COL_TIME = 12;
    static constexpr const i32 COL_NSOP = 12;

    // ── ANSI colors ────────────────────────────────────────────────

    func static const char* bold()    { return "\x1b[1m"; }
    func static const char* dim()     { return "\x1b[2m"; }
    func static const char* red()     { return "\x1b[31m"; }
    func static const char* green()   { return "\x1b[32m"; }
    func static const char* yellow()  { return "\x1b[33m"; }
    func static const char* cyan()    { return "\x1b[36m"; }
    func static const char* magenta() { return "\x1b[35m"; }
    func static const char* white()   { return "\x1b[37m"; }
    func static const char* reset()   { return "\x1b[0m"; }

    func const char* paint(const char* p_code) const { return use_color ? p_code : ""; }

    // ── Compiler barrier ───────────────────────────────────────────

    static void compiler_barrier() {
        __asm__ __volatile__("" ::: "memory");
    }

    // ── Number formatting ──────────────────────────────────────────

    // Writes unsigned integer with comma separators. Returns pointer past last char.
    func static char* format_uint(char* r_buf, u64 p_value) {
        char temp[32];
        i32 idx = 0;
        u64 v = p_value;
        if (v == 0) {
            temp[(++idx).get_value()] = '0';
        } else {
            while (v > 0) {
                temp[(++idx).get_value()] = '0' + to_u8(v % 10).get_value();
                v /= 10;
            }
        }
        char* out = r_buf;
        for (i32 i = idx - 1; i >= 0; --i) {
            i32 pos_from_right = idx - 1 - i;
            if (pos_from_right > 0 && pos_from_right % 3 == 0) {
                *out++ = ',';
            }
            *out++ = temp[i.get_value()];
        }
        *out = '\0';
        return out;
    }

    // Formats iteration count with K/M/B suffix.
    char* format_count(char* r_buf, u64 p_count) const {
        if (p_count >= 1000000000ull) {
            std::snprintf(r_buf, 32, "%.2fB", (double)(p_count / 1000000000.0).get_value());
        } else if (p_count >= 1000000ull) {
            std::snprintf(r_buf, 32, "%.1fM", (double)(p_count / 1000000.0).get_value());
        } else if (p_count >= 1000ull) {
            std::snprintf(r_buf, 32, "%.1fK", (double)(p_count / 1000.0).get_value());
        } else {
            std::snprintf(r_buf, 32, "%llu", p_count.get_value());
        }
        return r_buf;
    }

    // Formats time in nanoseconds with auto-scaled unit.
    char* format_time(char* r_buf, f64 p_ns) const {
        if (p_ns >= 1000000000.0) {
            std::snprintf(r_buf, 32, "%.2f s", (p_ns / 1000000000.0).get_value());
        } else if (p_ns >= 1000000.0) {
            std::snprintf(r_buf, 32, "%.2f ms", (p_ns / 1000000.0).get_value());
        } else if (p_ns >= 1000.0) {
            std::snprintf(r_buf, 32, "%.2f \u03bcs", (p_ns / 1000.0).get_value());
        } else {
            std::snprintf(r_buf, 32, "%.2f ns", p_ns.get_value());
        }
        return r_buf;
    }

    // Formats ns/op with auto-scaled unit.
    char* format_ns_per_op(char* r_buf, f64 p_ns) const {
        if (p_ns >= 1000000.0) {
            std::snprintf(r_buf, 32, "%.2f ms", (p_ns / 1000000.0).get_value());
        } else if (p_ns >= 1000.0) {
            std::snprintf(r_buf, 32, "%.2f \u03bcs", (p_ns / 1000.0).get_value());
        } else {
            std::snprintf(r_buf, 32, "%.2f ns", p_ns.get_value());
        }
        return r_buf;
    }

    // ── Warmup ─────────────────────────────────────────────────────

    // Stabilizes CPU caches and branch predictors before measurement.
    no_inline void warmup(void (*p_fn)()) {
        for (u32 i = 0; i < WARMUP_ITERATIONS; i += 1) {
            p_fn();
        }
    }

    // ── Measurement ────────────────────────────────────────────────

    // Runs function until deadline, sampling clock every SAMPLE_INTERVAL iterations.
    always_inline BenchResult measure_timed(const char* p_name, void (*p_fn)(), u64 p_duration_ns) {
        auto deadline = std::chrono::high_resolution_clock::now() +
                        std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                            std::chrono::duration<double, std::nano>(p_duration_ns.get_value()));

        u64 count = 0;
        compiler_barrier();
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();

        while (true) {
            for (u32 i = 0; i < SAMPLE_INTERVAL; i += 1) {
                p_fn();
                count += 1;
            }
            if (std::chrono::high_resolution_clock::now() >= deadline) {
                break;
            }
        }

        compiler_barrier();
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;

        BenchResult result;
        result.name = p_name;
        result.iteration_count = (count > 0xFFFFFFFFull) ? 0xFFFFFFFFu : to_u32(count);
        result.total_ns = elapsed.count();

        return result;
    }

    // ── Table formatting ───────────────────────────────────────────

    void print_separator() const {
        std::printf("  %s%s%s", paint(dim()),
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
                    "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500",
                    paint(reset()));
    }

    void print_header() const {
        std::printf("  %s%-*s%s %s%-*s%s %s%-*s%s %s%-*s%s\n",
                    paint(dim()), COL_NAME.get_value(), "Name", paint(reset()),
                    paint(dim()), COL_ITERS.get_value(), "Iterations", paint(reset()),
                    paint(dim()), COL_TIME.get_value(), "Time", paint(reset()),
                    paint(dim()), COL_NSOP.get_value(), "ns/op", paint(reset()));
    }

    void print_result_row(const BenchResult& r, f64 p_best_nsop, f64 p_worst_nsop) const {
        f64 ns_per_op = r.total_ns / (double)r.iteration_count.get_value();

        char buf_count[32];
        char buf_time[32];
        char buf_nsop[32];

        format_count(buf_count, r.iteration_count.get_value());
        format_time(buf_time, r.total_ns);
        format_ns_per_op(buf_nsop, ns_per_op);

        const char* marker = "";
        const char* marker_color = "";
        if (r.iteration_count.get_value() > 0) {
            if (ns_per_op <= p_best_nsop * 1.001) {
                marker = " \u25c4 best";
                marker_color = paint(green());
            } else if (ns_per_op >= p_worst_nsop * 0.999) {
                marker = " \u25ba slowest";
                marker_color = paint(red());
            }
        }

        std::printf("  %s%-*s%s %s%-*s%s %s%-*s%s %s%-*s%s%s%s%s\n",
                    paint(yellow()), COL_NAME.get_value(), r.name, paint(reset()),
                    paint(white()), COL_ITERS.get_value(), buf_count, paint(reset()),
                    paint(white()), COL_TIME.get_value(), buf_time, paint(reset()),
                    paint(white()), COL_NSOP.get_value(), buf_nsop, paint(reset()),
                    marker_color, marker, paint(reset()));
    }

    void print_group_table(u32 p_start, u32 p_end) const {
        f64 best_nsop = 1e30;
        f64 worst_nsop = 0.0;
        for (u32 i = p_start.get_value(); i < p_end.get_value(); i += 1) {
            const BenchResult& r = results[i.get_value()];
            if (r.iteration_count.get_value() > 0) {
                f64 ns_per_op = r.total_ns / to_f64(r.iteration_count);
                if (ns_per_op < best_nsop) best_nsop = ns_per_op;
                if (ns_per_op > worst_nsop) worst_nsop = ns_per_op;
            }
        }

        print_header();
        for (u32 i = p_start.get_value(); i < p_end.get_value(); i += 1) {
            print_result_row(results[i.get_value()], best_nsop.get_value(), worst_nsop.get_value());
        }
        print_separator();
    }

    public:
    // Constructors - Deconstructors

    Benchmarker() : benchmarks_run(0), use_color(BENCHMARKER_ISATTY(stdout)), duration_ns(3000000000.0) {}

    void set_duration(double p_seconds) { duration_ns = p_seconds * 1000000000.0; }

    // Benchmarking

    void iterate(const char* p_name, void (*p_fn)(), u64 p_seconds = 3.0) {
        warmup(p_fn);

        BenchResult result = measure_timed(p_name, p_fn, (p_seconds * 1000000000.0).get_value());

        results.push_back(result);
        benchmarks_run += 1;
    }

    // Grouping

    void begin_group(const char* p_name) {
        std::printf("\n  %s\u2500\u2500\u2500 %s \u2500\u2500\u2500%s\n", paint(cyan()), p_name, paint(reset()));
    }

    void end_group() {
        std::printf("\n");
    }

    void run_group(const char* p_name, void (*p_bench)(Benchmarker&)) {
        u32 start_idx = benchmarks_run;
        begin_group(p_name);

        p_bench(*this);

        u32 end_idx = benchmarks_run;
        groups.push_back({p_name, start_idx, end_idx});

        print_group_table(start_idx, end_idx);
        end_group();
    }

    // Reporting

    func bool all_passed() const {
        return benchmarks_run.get_value() > 0u;
    }

    void print_results() const {
        f64 global_best_nsop = 1e30;
        f64 global_worst_nsop = 0.0;
        const char* global_best_name = "";
        f64 total_time_s = 0.0;

        for (u32 i = 0; i < u32(results.size()); i += 1) {
            const BenchResult& r = results[i.get_value()];
            total_time_s += r.total_ns / 1000000000.0;
            if (r.iteration_count.get_value() > 0) {
                f64 ns_per_op = r.total_ns / to_f64(r.iteration_count);
                if (ns_per_op < global_best_nsop) {
                    global_best_nsop = ns_per_op;
                    global_best_name = r.name;
                }
                if (ns_per_op > global_worst_nsop) {
                    global_worst_nsop = ns_per_op;
                }
            }
        }

        std::printf("  %s%s\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550%s\n",
                    paint(magenta()), paint(bold()), paint(reset()));
        std::printf("  %s%s                    RATLAB BENCHMARK RESULTS%s\n",
                    paint(white()), paint(bold()), paint(reset()));

        char buf_footer[128];
        std::snprintf(buf_footer, sizeof(buf_footer),
                      "  Benchmarks: %s%u%s  \u2502  Total: %s%.2fs%s  \u2502  Best: %s%s%s @ %s%.2f ns%s",
                      paint(bold()), benchmarks_run.get_value(), paint(reset()),
                      paint(bold()), total_time_s.get_value(), paint(reset()),
                      paint(green()), global_best_name, paint(reset()),
                      paint(green()), global_best_nsop.get_value(), paint(reset()));

        std::printf("  %s%s\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550%s\n",
                    paint(magenta()), paint(bold()), paint(reset()));
        std::printf("%s\n", buf_footer);
        std::printf("  %s%s\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550%s\n",
                    paint(magenta()), paint(bold()), paint(reset()));
    }
};
