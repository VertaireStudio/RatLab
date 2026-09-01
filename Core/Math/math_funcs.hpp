/************************************/
/*         math_funcss.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"
#include "math_defs.hpp"


always_inline static func f32 abs(const f32 p_x) noexcept {
    return p_x < 0 ? -p_x : p_x;
}
always_inline static func f64 abs(const f64 p_x) noexcept {
    return p_x < 0 ? -p_x : p_x;
}

always_inline static func f32 min(const f32 p_x, const f32 p_y) noexcept {
    return p_x < p_y ? p_x : p_y;
}
always_inline static func f64 min(const f64 p_x, const f64 p_y) noexcept {
    return p_x < p_y ? p_x : p_y;
}

always_inline static func f32 max(const f32 p_x, const f32 p_y) noexcept {
    return p_x > p_y ? p_x : p_y;
}
always_inline static func f64 max(const f64 p_x, const f64 p_y) noexcept {
    return p_x > p_y ? p_x : p_y;
}

always_inline static func f32 clamp(const f32 p_value, const f32 p_min, const f32 p_max) noexcept {
    return p_value < p_min ? p_min : (p_value > p_max ? p_max : p_value);
}
always_inline static func f64 clamp(const f64 p_value, const f64 p_min, const f64 p_max) noexcept {
    return p_value < p_min ? p_min : (p_value > p_max ? p_max : p_value);
}

always_inline static func f32 trunc(const f32 p_x) noexcept {
    return to_f32(to_i32(p_x));
}
always_inline static func f64 trunc(const f64 p_x) noexcept {
    return to_f64(to_i64(p_x));
}

always_inline static func f32 decimal(const f32 p_x) noexcept {
    return abs(p_x - trunc(p_x));
}
always_inline static func f64 decimal(const f64 p_x) noexcept {
    return abs(p_x - trunc(p_x));
}

always_inline static func f32 floor(const f32 p_x) noexcept {
    const f32 t = trunc(p_x);
    return t > p_x ? t - 1.0f : t;
}
always_inline static func f64 floor(const f64 p_x) noexcept {
    const f64 t = trunc(p_x);
    return t > p_x ? t - 1.0 : t;
}

always_inline static func f32 ceil(const f32 p_x) noexcept {
    const f32 t = trunc(p_x);
    return t < p_x ? t + 1.0f : t;
}
always_inline static func f64 ceil(const f64 p_x) noexcept {
    const f64 t = trunc(p_x);
    return t < p_x ? t + 1.0 : t;
}

always_inline static func f32 round(const f32 p_x) noexcept {
    return p_x >= 0.0f ? floor(p_x + 0.5f) : ceil(p_x - 0.5f);
}
always_inline static func f64 round(const f64 p_x) noexcept {
    return p_x >= 0.0 ? floor(p_x + 0.5) : ceil(p_x - 0.5);
}

always_inline static func f32 cross(const f32 p_x1, const f32 p_x2, const f32 p_y1, const f32 p_y2) noexcept {
     return p_x1 * p_x2 - p_y1 * p_y2;
}
always_inline static func f64 cross(const f64 p_x1, const f64 p_x2, const f64 p_y1, const f64 p_y2) noexcept {
     return p_x1 * p_x2 - p_y1 * p_y2;
}

always_inline static func f32 dot(const f32 p_x1, const f32 p_x2, const f32 p_y1, const f32 p_y2) noexcept {
     return p_x1 * p_x2 + p_y1 * p_y2;
}
always_inline static func f64 dot(const f64 p_x1, const f64 p_x2, const f64 p_y1, const f64 p_y2) noexcept {
     return p_x1 * p_x2 + p_y1 * p_y2;
}

always_inline static func f32 sin_series(const f32 p_x) noexcept {
    const f32 x2 = p_x * p_x;
    constexpr const f32 y1 = 1.0f;
    constexpr const f32 y2 = -0.16666667f;
    constexpr const f32 y3 = 0.0083333333f;
    constexpr const f32 y4 = -0.00019841269f;
    constexpr const f32 y5 = 0.0000027557319f;

    return p_x * (y1 + x2 * (y2 + x2 * (y3 + x2 * (y4 + x2 * y5))));
}
always_inline static func f64 sin_series(const f64 p_x) noexcept {
    const f64 x2 = p_x * p_x;
    constexpr const f64 y1 = 1.0;
    constexpr const f64 y2 = -0.16666666666666666;
    constexpr const f64 y3 = 0.008333333333333333;
    constexpr const f64 y4 = -0.0001984126984126984;
    constexpr const f64 y5 = 0.0000027557319223985893;
    constexpr const f64 y6 = -0.00000002505210838544171877;
    constexpr const f64 y7 = 0.000000000160590438368216146;
    return p_x * (y1 + x2 * (y2 + x2 * (y3 + x2 * (y4 + x2 * (y5 + x2 * (y6 + x2 * y7))))));
}

always_inline static func f32 cos_series(const f32 p_x) noexcept {
    const f32 x2 = p_x * p_x;
    constexpr const f32 y1 = 1.0f;
    constexpr const f32 y2 = -0.5f;
    constexpr const f32 y3 = 0.041666667f;
    constexpr const f32 y4 = -0.0013888889f;
    constexpr const f32 y5 = 0.000024801587f;
    constexpr const f32 y6 = -0.00000027557319f;
    return y1 + x2 * (y2 + x2 * (y3 + x2 * (y4 + x2 * (y5 + x2 * y6))));
}
always_inline static func f64 cos_series(const f64 p_x) noexcept {
    const f64 x2 = p_x * p_x;
    constexpr const f64 y1 = 1.0;
    constexpr const f64 y2 = -0.5;
    constexpr const f64 y3 = 0.041666666666666664;
    constexpr const f64 y4 = -0.001388888888888889;
    constexpr const f64 y5 = 0.0000248015873015873;
    constexpr const f64 y6 = -0.0000002755731922398589;
    constexpr const f64 y7 = 0.0000000020876756987868099;
    return y1 + x2 * (y2 + x2 * (y3 + x2 * (y4 + x2 * (y5 + x2 * (y6 + x2 * y7)))));
}

always_inline static func f32 reduce_2pi(const f32 p_x) noexcept {
    return p_x - trunc(p_x * (f32(1.0f) / F32_TAU) + (p_x < 0 ? -0.5f : 0.5f)) * F32_TAU;
}
always_inline static func f64 reduce_2pi(const f64 p_x) noexcept {
    return p_x - trunc(p_x * (f64(1.0) / F64_TAU) + (p_x < 0 ? -0.5 : 0.5)) * F64_TAU;
}

always_inline static func f32 sin_half(const f32 p_x) noexcept {
    const f32 ax = abs(p_x);
    if (ax > F32_PI_QUARTER) {
        const f32 c = cos_series((F32_PI_HALF - ax) + F32_PI_HALF_OFFSET);
        return p_x < 0 ? -c : c;
    }
    return sin_series(p_x);
}
always_inline static func f64 sin_half(const f64 p_x) noexcept {
    const f64 ax = abs(p_x);
    if (ax > F64_PI_QUARTER) {
        const f64 c = cos_series((F64_PI_HALF - ax) + F64_PI_HALF_OFFSET);
        return p_x < 0 ? -c : c;
    }
    return sin_series(p_x);
}

always_inline static func f32 cos_half(const f32 p_x) noexcept {
    const f32 ax = abs(p_x);
    return ax > F32_PI_QUARTER ? sin_series((F32_PI_HALF - ax) + F32_PI_HALF_OFFSET) : cos_series(p_x);
}
always_inline static func f64 cos_half(const f64 p_x) noexcept {
    const f64 ax = abs(p_x);
    return ax > F64_PI_QUARTER ? sin_series((F64_PI_HALF - ax) + F64_PI_HALF_OFFSET) : cos_series(p_x);
}

always_inline static func f32 sin(const f32 p_x) noexcept {
    const f32 x = reduce_2pi(p_x);
    return sin_half(x > F32_PI_HALF ? F32_PI - x : (x < -F32_PI_HALF ? -F32_PI - x : x));
}
always_inline static func f64 sin(const f64 p_x) noexcept {
    const f64 x = reduce_2pi(p_x);
    return sin_half(x > F64_PI_HALF ? F64_PI - x : (x < -F64_PI_HALF ? -F64_PI - x : x));
}

always_inline static func f32 cos(const f32 p_x) noexcept {
    const f32 x = reduce_2pi(p_x);
    const f32 result = x > F32_PI_HALF ? F32_PI - x : (x < -F32_PI_HALF ? -F32_PI - x : x);
    return x > F32_PI_HALF || x < -F32_PI_HALF ? -cos_half(result) : cos_half(result);
}
always_inline static func f64 cos(const f64 p_x) noexcept {
    const f64 x = reduce_2pi(p_x);
    const f64 result = x > F64_PI_HALF ? F64_PI - x : (x < -F64_PI_HALF ? -F64_PI - x : x);
    return x > F64_PI_HALF || x < -F64_PI_HALF ? -cos_half(result) : cos_half(result);
}

always_inline static func f32 tan(const f32 p_x) noexcept {
    const f32 x = reduce_2pi(p_x);
    const f32 folded = x > F32_PI_HALF ? F32_PI - x : (x < -F32_PI_HALF ? -F32_PI - x : x);
    const f32 s = sin_half(folded);
    const f32 c = cos_half(folded);
    return x > F32_PI_HALF || x < -F32_PI_HALF ? -s / c : s / c;
}
always_inline static func f64 tan(const f64 p_x) noexcept {
    const f64 x = reduce_2pi(p_x);
    const f64 folded = x > F64_PI_HALF ? F64_PI - x : (x < -F64_PI_HALF ? -F64_PI - x : x);
    const f64 s = sin_half(folded);
    const f64 c = cos_half(folded);
    return x > F64_PI_HALF || x < -F64_PI_HALF ? -s / c : s / c;
}
