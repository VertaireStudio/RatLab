/************************************/
/*             f32.hpp              */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/type_configs.hpp"

#include "../Essentials/type_configs.hpp"
#include "../Essentials/def_configs.hpp"

// Empty structs, used for conversion in 'type_conversion.cpp'

struct Bool;
struct u8;
struct u16;
struct u32;
struct u64;
struct i8;
struct i16;
struct i32;
struct i64;
struct f64;


struct f32 {
    static const f32 ZERO;

    // Data
    float value;

    // Constructor

    func f32() : value(0.0f) {}
    func f32(float p_value) : value(p_value) {}
    
    template<typename T>
    func f32(T p_value) : value((float)p_value) {}

    // Getsets
    func float get() const { return value; }
    func void set(double p_value) { value = p_value; }

    // Operators
    func f32 operator+(f32 p_value) const { return f32(value + p_value.get()); }
    func void operator+=(f32 p_value) { value += p_value.value; }
    func f32 operator-(f32 p_value) const { return f32(value - p_value.get()); }
    func void operator-=(f32 p_value) { value -= p_value.value; }
    func f32 operator*(f32 p_value) const { return f32(value * p_value.get()); }
    func void operator*=(f32 p_value) { value *= p_value.value; }
    func f32 operator/(f32 p_value) const { return p_value.value == 0.0f ? f32(FLOAT_MAX) : f32(value / p_value.get()); }
    func void operator/=(f32 p_value) { p_value.value == 0.0f ? (value = FLOAT_MAX) : (value /= p_value.value); }

    func void operator=(f32 p_value) { value = p_value.value; }
    func bool operator==(f32 p_value) { return value == p_value.value; }
    func bool operator!=(f32 p_value) { return value != p_value.value; }
    func bool operator<(f32 p_value) { return value < p_value.value; }
    func bool operator<=(f32 p_value) { return value <= p_value.value; }
    func bool operator>(f32 p_value) { return value > p_value.value; }
    func bool operator>=(f32 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0.0f ? (value = FLOAT_MAX) : value -= 1.0f; }
    func void operator++() { value == FLOAT_MAX ? (value = 0.0f) : value += 1.0f; }
    func f32 operator!() { return f32(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::F32; }

    // Math Functions
    func f32 min(f32 p_value) { return value < p_value.value ? value : p_value.value; }
    func f32 max(f32 p_value) { return value > p_value.value ? value : p_value.value; }
    func f32 clamp(f32 p_left, f32 p_right) { return min(p_right).max(p_left); }
    func f32 sign() { return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f64();
};

inline const func f32 f32::ZERO = {0.0f};