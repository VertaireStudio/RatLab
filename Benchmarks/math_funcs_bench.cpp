/************************************/
/*      math_funcs_bench.cpp        */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "benchmarker.hpp"
#include "../Core/Math/math_funcs.hpp"

static volatile float sink_f32;
static volatile double sink_f64;

static constexpr const f32 in_f32_a = f32(1.57f);
static constexpr const f32 in_f32_b = f32(0.75f);
static constexpr const f32 in_f32_c = f32(2.0f);
static constexpr const f64 in_f64_a = f64(1.57);
static constexpr const f64 in_f64_b = f64(0.75);
static constexpr const f64 in_f64_c = f64(2.0);

void bench_abs_f32() { sink_f32 = abs(in_f32_a).get_value(); }
void bench_abs_f64() { sink_f64 = abs(in_f64_a).get_value(); }

void bench_min_f32() { sink_f32 = min(in_f32_a, in_f32_b).get_value(); }
void bench_min_f64() { sink_f64 = min(in_f64_a, in_f64_b).get_value(); }

void bench_max_f32() { sink_f32 = max(in_f32_a, in_f32_b).get_value(); }
void bench_max_f64() { sink_f64 = max(in_f64_a, in_f64_b).get_value(); }

void bench_clamp_f32() { sink_f32 = clamp(in_f32_a, in_f32_b, in_f32_c).get_value(); }
void bench_clamp_f64() { sink_f64 = clamp(in_f64_a, in_f64_b, in_f64_c).get_value(); }

void bench_trunc_f32() { sink_f32 = trunc(in_f32_a).get_value(); }
void bench_trunc_f64() { sink_f64 = trunc(in_f64_a).get_value(); }

void bench_decimal_f32() { sink_f32 = decimal(in_f32_a).get_value(); }
void bench_decimal_f64() { sink_f64 = decimal(in_f64_a).get_value(); }

void bench_floor_f32() { sink_f32 = floor(in_f32_a).get_value(); }
void bench_floor_f64() { sink_f64 = floor(in_f64_a).get_value(); }

void bench_ceil_f32() { sink_f32 = ceil(in_f32_a).get_value(); }
void bench_ceil_f64() { sink_f64 = ceil(in_f64_a).get_value(); }

void bench_round_f32() { sink_f32 = round(in_f32_a).get_value(); }
void bench_round_f64() { sink_f64 = round(in_f64_a).get_value(); }

void bench_cross_f32() { sink_f32 = cross(in_f32_a, in_f32_b, in_f32_c, in_f32_a).get_value(); }
void bench_cross_f64() { sink_f64 = cross(in_f64_a, in_f64_b, in_f64_c, in_f64_a).get_value(); }

void bench_dot_f32() { sink_f32 = dot(in_f32_a, in_f32_b, in_f32_c, in_f32_a).get_value(); }
void bench_dot_f64() { sink_f64 = dot(in_f64_a, in_f64_b, in_f64_c, in_f64_a).get_value(); }

void bench_sin_series_f32() { sink_f32 = sin_series(in_f32_a).get_value(); }
void bench_sin_series_f64() { sink_f64 = sin_series(in_f64_a).get_value(); }

void bench_cos_series_f32() { sink_f32 = cos_series(in_f32_a).get_value(); }
void bench_cos_series_f64() { sink_f64 = cos_series(in_f64_a).get_value(); }

void bench_reduce_2pi_f32() { sink_f32 = reduce_2pi(in_f32_a).get_value(); }
void bench_reduce_2pi_f64() { sink_f64 = reduce_2pi(in_f64_a).get_value(); }

void bench_sin_half_f32() { sink_f32 = sin_half(in_f32_a).get_value(); }
void bench_sin_half_f64() { sink_f64 = sin_half(in_f64_a).get_value(); }

void bench_cos_half_f32() { sink_f32 = cos_half(in_f32_a).get_value(); }
void bench_cos_half_f64() { sink_f64 = cos_half(in_f64_a).get_value(); }

void bench_sin_f32() { sink_f32 = sin(in_f32_a).get_value(); }
void bench_sin_f64() { sink_f64 = sin(in_f64_a).get_value(); }

void bench_cos_f32() { sink_f32 = cos(in_f32_a).get_value(); }
void bench_cos_f64() { sink_f64 = cos(in_f64_a).get_value(); }

void bench_tan_f32() { sink_f32 = tan(in_f32_a).get_value(); }
void bench_tan_f64() { sink_f64 = tan(in_f64_a).get_value(); }

void bench_basic(Benchmarker& r_bench) {
    r_bench.iterate("abs(f32)", bench_abs_f32, 1);
    r_bench.iterate("abs(f64)", bench_abs_f64, 1);
    r_bench.iterate("min(f32)", bench_min_f32, 1);
    r_bench.iterate("min(f64)", bench_min_f64, 1);
    r_bench.iterate("max(f32)", bench_max_f32, 1);
    r_bench.iterate("max(f64)", bench_max_f64, 1);
    r_bench.iterate("clamp(f32)", bench_clamp_f32, 1);
    r_bench.iterate("clamp(f64)", bench_clamp_f64, 1);
}

void bench_rounding(Benchmarker& r_bench) {
    r_bench.iterate("trunc(f32)", bench_trunc_f32, 1);
    r_bench.iterate("trunc(f64)", bench_trunc_f64, 1);
    r_bench.iterate("decimal(f32)", bench_decimal_f32, 1);
    r_bench.iterate("decimal(f64)", bench_decimal_f64, 1);
    r_bench.iterate("floor(f32)", bench_floor_f32, 1);
    r_bench.iterate("floor(f64)", bench_floor_f64, 1);
    r_bench.iterate("ceil(f32)", bench_ceil_f32, 1);
    r_bench.iterate("ceil(f64)", bench_ceil_f64, 1);
    r_bench.iterate("round(f32)", bench_round_f32, 1);
    r_bench.iterate("round(f64)", bench_round_f64, 1);
}

void bench_vector(Benchmarker& r_bench) {
    r_bench.iterate("cross(f32)", bench_cross_f32, 1);
    r_bench.iterate("cross(f64)", bench_cross_f64, 1);
    r_bench.iterate("dot(f32)", bench_dot_f32, 1);
    r_bench.iterate("dot(f64)", bench_dot_f64, 1);
}

void bench_trig_series(Benchmarker& r_bench) {
    r_bench.iterate("sin_series(f32)", bench_sin_series_f32, 1);
    r_bench.iterate("sin_series(f64)", bench_sin_series_f64, 1);
    r_bench.iterate("cos_series(f32)", bench_cos_series_f32, 1);
    r_bench.iterate("cos_series(f64)", bench_cos_series_f64, 1);
    r_bench.iterate("reduce_2pi(f32)", bench_reduce_2pi_f32, 1);
    r_bench.iterate("reduce_2pi(f64)", bench_reduce_2pi_f64, 1);
    r_bench.iterate("sin_half(f32)", bench_sin_half_f32, 1);
    r_bench.iterate("sin_half(f64)", bench_sin_half_f64, 1);
    r_bench.iterate("cos_half(f32)", bench_cos_half_f32, 1);
    r_bench.iterate("cos_half(f64)", bench_cos_half_f64, 1);
}

void bench_trig(Benchmarker& r_bench) {
    r_bench.iterate("sin(f32)", bench_sin_f32, 1);
    r_bench.iterate("sin(f64)", bench_sin_f64, 1);
    r_bench.iterate("cos(f32)", bench_cos_f32, 1);
    r_bench.iterate("cos(f64)", bench_cos_f64, 1);
    r_bench.iterate("tan(f32)", bench_tan_f32, 1);
    r_bench.iterate("tan(f64)", bench_tan_f64, 1);
}

int main() {
    Benchmarker bench;

    bench.run_group("Basic", bench_basic);
    bench.run_group("Rounding", bench_rounding);
    bench.run_group("Vector", bench_vector);
    bench.run_group("Trig Series", bench_trig_series);
    bench.run_group("Trigonometry", bench_trig);

    bench.print_results();
    return 0;
}
