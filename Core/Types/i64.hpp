/************************************/
/*             i64.hpp              */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

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
struct f32;
struct f64;


struct i64 {
    static const i64 ZERO;

    // Data
    unsigned long long value;

    // Constructor
    func i64() : value(0) {}
    func i64(unsigned long long p_value) : value(p_value) {}
    
    template<typename T>
    func i64(T p_value) : value((unsigned long long)p_value) {}

    // Getsets
    func unsigned long long get() const { return value; }
    func void set(unsigned long long p_value) { value = p_value; }

    // Operators
    func i64 operator+(i64 p_value) const { return i64(value + p_value.get()); }
    func void operator+=(i64 p_value) { value += p_value.value; }
    func i64 operator-(i64 p_value) const { return i64(value - p_value.get()); }
    func void operator-=(i64 p_value) { value -= p_value.value; }
    func i64 operator*(i64 p_value) const { return i64(value * p_value.get()); }
    func void operator*=(i64 p_value) { value *= p_value.value; }
    func i64 operator/(i64 p_value) const { return p_value.value == 0 ? i64(I64_MAX) : i64(value / p_value.get()); }
    func void operator/=(i64 p_value) { p_value.value == 0 ? (value = I64_MAX) : (value /= p_value.value); }

    func void operator=(i64 p_value) { value = p_value.value; }
    func bool operator==(i64 p_value) { return value == p_value.value; }
    func bool operator!=(i64 p_value) { return value != p_value.value; }
    func bool operator<(i64 p_value) { return value < p_value.value; }
    func bool operator<=(i64 p_value) { return value <= p_value.value; }
    func bool operator>(i64 p_value) { return value > p_value.value; }
    func bool operator>=(i64 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I64_MAX) : value -= 1; }
    func void operator++() { value == I64_MAX ? (value = 0) : value += 1; }
    func i64 operator!() { return i64(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::I64; }

    // Math Functions
    func i64 min(i64 p_value) { return value < p_value.value ? value : p_value.value; }
    func i64 max(i64 p_value) { return value > p_value.value ? value : p_value.value; }
    func i64 clamp(i64 p_left, i64 p_right) { return min(p_right).max(p_left); }
    func i64 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator f32();
    func operator f64();
};

inline const func i64 i64::ZERO = {0};