/************************************/
/*       math_funcs_test.cpp        */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "tester.hpp"
#include "../Core/Math/math_funcs.hpp"

#include <cmath>
#include <cstdio>
#include <initializer_list>

static constexpr const f32 EPS_F32 = 0.00001f;
static constexpr const f64 EPS_F64 = 0.000000001;

// abs - both overloads.

void test_abs(Tester& r_tester) {

    r_tester.test_equal(abs(f32(-3.5f)), f32(3.5f));
    r_tester.test_equal(abs(f32(2.25f)), f32(2.25f));
    r_tester.test_equal(abs(f32(0.0f)), f32(0.0f));

    r_tester.test_equal(abs(f64(-3.5)), f64(3.5));
    r_tester.test_equal(abs(f64(2.25)), f64(2.25));
    r_tester.test_equal(abs(f64(0.0)), f64(0.0));
}

// min - both orderings, equal values, negatives.

void test_min(Tester& r_tester) {

    r_tester.test_equal(min(f32(1.0f), f32(2.0f)), f32(1.0f));
    r_tester.test_equal(min(f32(2.0f), f32(1.0f)), f32(1.0f));
    r_tester.test_equal(min(f32(-5.0f), f32(3.0f)), f32(-5.0f));
    r_tester.test_equal(min(f32(4.0f), f32(4.0f)), f32(4.0f));

    r_tester.test_equal(min(f64(1.0), f64(2.0)), f64(1.0));
    r_tester.test_equal(min(f64(2.0), f64(1.0)), f64(1.0));
    r_tester.test_equal(min(f64(-5.0), f64(3.0)), f64(-5.0));
    r_tester.test_equal(min(f64(4.0), f64(4.0)), f64(4.0));
}

// max - both orderings, equal values, negatives.

void test_max(Tester& r_tester) {

    r_tester.test_equal(max(f32(1.0f), f32(2.0f)), f32(2.0f));
    r_tester.test_equal(max(f32(2.0f), f32(1.0f)), f32(2.0f));
    r_tester.test_equal(max(f32(-5.0f), f32(3.0f)), f32(3.0f));
    r_tester.test_equal(max(f32(4.0f), f32(4.0f)), f32(4.0f));

    r_tester.test_equal(max(f64(1.0), f64(2.0)), f64(2.0));
    r_tester.test_equal(max(f64(2.0), f64(1.0)), f64(2.0));
    r_tester.test_equal(max(f64(-5.0), f64(3.0)), f64(3.0));
    r_tester.test_equal(max(f64(4.0), f64(4.0)), f64(4.0));
}

// clamp - below range, within range, above range.

void test_clamp(Tester& r_tester) {

    r_tester.test_equal(clamp(f32(0.0f), f32(1.0f), f32(3.0f)), f32(1.0f));
    r_tester.test_equal(clamp(f32(2.0f), f32(1.0f), f32(3.0f)), f32(2.0f));
    r_tester.test_equal(clamp(f32(9.0f), f32(1.0f), f32(3.0f)), f32(3.0f));
    r_tester.test_equal(clamp(f32(-2.0f), f32(-3.0f), f32(-1.0f)), f32(-2.0f));

    r_tester.test_equal(clamp(f64(0.0), f64(1.0), f64(3.0)), f64(1.0));
    r_tester.test_equal(clamp(f64(2.0), f64(1.0), f64(3.0)), f64(2.0));
    r_tester.test_equal(clamp(f64(9.0), f64(1.0), f64(3.0)), f64(3.0));
    r_tester.test_equal(clamp(f64(-2.0), f64(-3.0), f64(-1.0)), f64(-2.0));
}

// trunc - positive/negative fractions and whole numbers.

void test_trunc(Tester& r_tester) {

    r_tester.test_equal(trunc(f32(3.7f)), f32(3.0f));
    r_tester.test_equal(trunc(f32(-3.7f)), f32(-3.0f));
    r_tester.test_equal(trunc(f32(4.0f)), f32(4.0f));
    r_tester.test_equal(trunc(f32(0.0f)), f32(0.0f));

    r_tester.test_equal(trunc(f64(3.7)), f64(3.0));
    r_tester.test_equal(trunc(f64(-3.7)), f64(-3.0));
    r_tester.test_equal(trunc(f64(4.0)), f64(4.0));
    r_tester.test_equal(trunc(f64(0.0)), f64(0.0));
}

// decimal - fractional part, negative input, whole input.

void test_decimal(Tester& r_tester) {

    r_tester.test_approximately(decimal(f32(3.75f)), f32(0.75f), EPS_F32);
    r_tester.test_approximately(decimal(f32(-3.75f)), f32(0.75f), EPS_F32);
    r_tester.test_approximately(decimal(f32(4.0f)), f32(0.0f), EPS_F32);

    r_tester.test_approximately(decimal(f64(3.75)), f64(0.75), EPS_F64);
    r_tester.test_approximately(decimal(f64(-3.75)), f64(0.75), EPS_F64);
    r_tester.test_approximately(decimal(f64(4.0)), f64(0.0), EPS_F64);
}

// floor - rounds toward negative infinity.

void test_floor(Tester& r_tester) {

    r_tester.test_approximately(floor(f32(3.2f)), f32(3.0f), EPS_F32);
    r_tester.test_approximately(floor(f32(-3.2f)), f32(-4.0f), EPS_F32);
    r_tester.test_approximately(floor(f32(4.0f)), f32(4.0f), EPS_F32);
    r_tester.test_approximately(floor(f32(0.0f)), f32(0.0f), EPS_F32);

    r_tester.test_approximately(floor(f64(3.2)), f64(3.0), EPS_F64);
    r_tester.test_approximately(floor(f64(-3.2)), f64(-4.0), EPS_F64);
    r_tester.test_approximately(floor(f64(4.0)), f64(4.0), EPS_F64);
    r_tester.test_approximately(floor(f64(0.0)), f64(0.0), EPS_F64);
}

// ceil - rounds toward positive infinity.

void test_ceil(Tester& r_tester) {

    r_tester.test_approximately(ceil(f32(3.2f)), f32(4.0f), EPS_F32);
    r_tester.test_approximately(ceil(f32(-3.2f)), f32(-3.0f), EPS_F32);
    r_tester.test_approximately(ceil(f32(4.0f)), f32(4.0f), EPS_F32);
    r_tester.test_approximately(ceil(f32(0.0f)), f32(0.0f), EPS_F32);

    r_tester.test_approximately(ceil(f64(3.2)), f64(4.0), EPS_F64);
    r_tester.test_approximately(ceil(f64(-3.2)), f64(-3.0), EPS_F64);
    r_tester.test_approximately(ceil(f64(4.0)), f64(4.0), EPS_F64);
    r_tester.test_approximately(ceil(f64(0.0)), f64(0.0), EPS_F64);
}

// round - nearest integer semantics.

void test_round(Tester& r_tester) {

    r_tester.test_approximately(round(f32(3.4f)), f32(3.0f), EPS_F32);
    r_tester.test_approximately(round(f32(3.6f)), f32(4.0f), EPS_F32);
    r_tester.test_approximately(round(f32(-3.4f)), f32(-3.0f), EPS_F32);
    r_tester.test_approximately(round(f32(-3.6f)), f32(-4.0f), EPS_F32);
    r_tester.test_approximately(round(f32(0.0f)), f32(0.0f), EPS_F32);

    r_tester.test_approximately(round(f64(3.4)), f64(3.0), EPS_F64);
    r_tester.test_approximately(round(f64(3.6)), f64(4.0), EPS_F64);
    r_tester.test_approximately(round(f64(-3.4)), f64(-3.0), EPS_F64);
    r_tester.test_approximately(round(f64(-3.6)), f64(-4.0), EPS_F64);
    r_tester.test_approximately(round(f64(0.0)), f64(0.0), EPS_F64);
}

// dot - scalar product x1*x2 + y1*y2.
// NOTE: Current implementation computes 'x1*x2 - y1*y2', so these tests are expected to fail until fixed.

void test_dot(Tester& r_tester) {

    r_tester.test_equal(dot(f32(3.0f), f32(4.0f), f32(5.0f), f32(6.0f)), f32(42.0f));
    r_tester.test_equal(dot(f32(1.0f), f32(0.0f), f32(0.0f), f32(1.0f)), f32(0.0f));

    r_tester.test_equal(dot(f64(3.0), f64(4.0), f64(5.0), f64(6.0)), f64(42.0));
    r_tester.test_equal(dot(f64(1.0), f64(0.0), f64(0.0), f64(1.0)), f64(0.0));
}

// cross - scalar difference x1*x2 - y1*y2.
// NOTE: Current implementation computes 'x1*x2 + y1*y2', so these tests are expected to fail until fixed.

void test_cross(Tester& r_tester) {

    r_tester.test_equal(cross(f32(3.0f), f32(4.0f), f32(5.0f), f32(6.0f)), f32(-18.0f));
    r_tester.test_equal(cross(f32(1.0f), f32(1.0f), f32(1.0f), f32(1.0f)), f32(0.0f));

    r_tester.test_equal(cross(f64(3.0), f64(4.0), f64(5.0), f64(6.0)), f64(-18.0));
    r_tester.test_equal(cross(f64(1.0), f64(1.0), f64(1.0), f64(1.0)), f64(0.0));
}

// sin_series / cos_series - Taylor series on small inputs vs std.

void test_series(Tester& r_tester) {

    for (const float x : { 0.1f, 0.3f, 0.6f }) {
        r_tester.test_approximately(sin_series(f32(x)), f32(std::sin(x)), EPS_F32);
        r_tester.test_approximately(cos_series(f32(x)), f32(std::cos(x)), EPS_F32);
    }

    for (const double x : { 0.1, 0.3, 0.6 }) {
        r_tester.test_approximately(sin_series(f64(x)), f64(std::sin(x)), EPS_F64);
        r_tester.test_approximately(cos_series(f64(x)), f64(std::cos(x)), EPS_F64);
    }
}

// reduce_2pi - result must land in [-PI, PI] while preserving the angle's direction.

void test_reduce_2pi(Tester& r_tester) {

    for (const float x : { 10.0f, -10.0f, 100.0f, -100.0f, 6.2831855f }) {
        const f32 reduced = reduce_2pi(f32(x));
        r_tester.test_greater_or_equal(reduced, f32(-F32_PI.get_value()));
        r_tester.test_less_or_equal(reduced, f32(F32_PI.get_value()));
        r_tester.test_approximately(reduced, f32(std::remainder(x, F32_PI.get_value() * 2.0f)), EPS_F32);
    }

    for (const double x : { 10.0, -10.0, 100.0, -100.0, 6.283185307179586 }) {
        const f64 reduced = reduce_2pi(f64(x));
        r_tester.test_greater_or_equal(reduced, f64(-F64_PI.get_value()));
        r_tester.test_less_or_equal(reduced, f64(F64_PI.get_value()));
        r_tester.test_approximately(reduced, f64(std::remainder(x, F64_PI.get_value() * 2.0)), EPS_F64);
    }
}

// sin_half / cos_half - accurate on both sides of the PI_QUARTER switch point.

void test_half(Tester& r_tester) {

    for (const float x : { 0.3f, -0.3f, 1.2f, -1.2f }) {
        r_tester.test_approximately(sin_half(f32(x)), f32(std::sin(x)), EPS_F32);
        r_tester.test_approximately(cos_half(f32(x)), f32(std::cos(x)), EPS_F32);
    }

    for (const double x : { 0.3, -0.3, 1.2, -1.2 }) {
        r_tester.test_approximately(sin_half(f64(x)), f64(std::sin(x)), EPS_F64);
        r_tester.test_approximately(cos_half(f64(x)), f64(std::cos(x)), EPS_F64);
    }
}

// sin / cos / tan - full range handling vs std, including large and negative angles.

void test_trigonometry(Tester& r_tester) {

    constexpr float PI_F = 3.1415927f;
    for (const float x : { 0.0f, PI_F / 6.0f, PI_F / 4.0f, PI_F / 2.0f, PI_F,
                           -PI_F / 6.0f, -PI_F / 2.0f, -PI_F, 6.2831855f, 10.0f, -10.0f }) {
        r_tester.test_approximately(sin(f32(x)), f32(std::sin(x)), EPS_F32);
        r_tester.test_approximately(cos(f32(x)), f32(std::cos(x)), EPS_F32);

        // NOTE: tan is undefined at PI/2; only check that the result is finite there.
        if (std::fabs(std::cos(x)) > 0.00001f) {
            r_tester.test_approximately(tan(f32(x)), f32(std::tan(x)), EPS_F32 * f32(10.0f));
        } else {
            const f32 t = tan(f32(x));
            r_tester.test_match(t == t && t > f32(-F32_MAX) && t < f32(F32_MAX), true);
        }
    }

    constexpr double PI_D = 3.141592653589793;
    for (const double x : { 0.0, PI_D / 6.0, PI_D / 4.0, PI_D / 2.0, PI_D,
                            -PI_D / 6.0, -PI_D / 2.0, -PI_D, 6.283185307179586, 10.0, -10.0 }) {
        r_tester.test_approximately(sin(f64(x)), f64(std::sin(x)), EPS_F64);
        r_tester.test_approximately(cos(f64(x)), f64(std::cos(x)), EPS_F64);

        // NOTE: tan is undefined at PI/2; only check that the result is finite there.
        if (std::fabs(std::cos(x)) > 0.000000001) {
            r_tester.test_approximately(tan(f64(x)), f64(std::tan(x)), EPS_F64 * f64(10.0));
        } else {
            const f64 t = tan(f64(x));
            r_tester.test_match(t == t && t > f64(-F64_MAX) && t < f64(F64_MAX), true);
        }
    }
}

int main() {
    Tester tester;

    tester.begin_group("abs");          test_abs(tester);           tester.end_group();
    tester.begin_group("min");          test_min(tester);           tester.end_group();
    tester.begin_group("max");          test_max(tester);           tester.end_group();
    tester.begin_group("clamp");        test_clamp(tester);         tester.end_group();
    tester.begin_group("trunc");        test_trunc(tester);         tester.end_group();
    tester.begin_group("decimal");      test_decimal(tester);       tester.end_group();
    tester.begin_group("floor");        test_floor(tester);         tester.end_group();
    tester.begin_group("ceil");         test_ceil(tester);          tester.end_group();
    tester.begin_group("round");        test_round(tester);         tester.end_group();
    tester.begin_group("dot");          test_dot(tester);           tester.end_group();
    tester.begin_group("cross");        test_cross(tester);         tester.end_group();
    tester.begin_group("series");       test_series(tester);        tester.end_group();
    tester.begin_group("reduce_2pi");   test_reduce_2pi(tester);    tester.end_group();
    tester.begin_group("half");         test_half(tester);          tester.end_group();
    tester.begin_group("trigonometry"); test_trigonometry(tester);  tester.end_group();

    tester.print_results();
    return tester.all_passed() ? 0 : 1;
}
