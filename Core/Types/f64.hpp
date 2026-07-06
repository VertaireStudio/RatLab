/************************************/
/*             f64.hpp              */
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
struct f32;


struct f64 {
    // Data
    double value;

    // Constructor

    func f64() : value(0.0) {}
    func f64(double p_value) : value(p_value) {}
    
    template<typename T>
    func f64(T p_value) : value((double)p_value) {}

    // Getsets
    func float get() const { return value; }
    func void set(double p_value) { value = p_value; }

    // Operators
    func f64 operator+(f64 p_value) const { return f64(value + p_value.get()); }
    func void operator+=(f64 p_value) { value += p_value.value; }
    func f64 operator-(f64 p_value) const { return f64(value - p_value.get()); }
    func void operator-=(f64 p_value) { value -= p_value.value; }
    func f64 operator*(f64 p_value) const { return f64(value * p_value.get()); }
    func void operator*=(f64 p_value) { value *= p_value.value; }
    func f64 operator/(f64 p_value) const { return p_value.value == 0.0 ? f64(DOUBLE_MAX) : f64(value / p_value.get()); }
    func void operator/=(f64 p_value) { p_value.value == 0.0 ? (value = DOUBLE_MAX) : (value /= p_value.value); }

    func void operator=(f64 p_value) { value = p_value.value; }
    func bool operator==(f64 p_value) { return value == p_value.value; }
    func bool operator<(f64 p_value) { return value < p_value.value; }
    func bool operator<=(f64 p_value) { return value <= p_value.value; }
    func bool operator>(f64 p_value) { return value > p_value.value; }
    func bool operator>=(f64 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0.0 ? (value = DOUBLE_MAX) : value -= 1.0; }
    func void operator++() { value == DOUBLE_MAX ? (value = 0.0) : value += 1.0; }

    // Helper Functions
    func TYPES get_type() { return TYPES::F64; }

    // Math Functions
    func f64 min(f64 p_value) { return value < p_value.value ? value : p_value.value; }
    func f64 max(f64 p_value) { return value > p_value.value ? value : p_value.value; }
    func f64 clamp(f64 p_left, f64 p_right) { return min(p_right).max(p_left); }
    func f64 sign() { return value < 0.0 ? -1.0 : (value > 0.0 ? 1.0 : 0.0); }

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
    func operator f32();
};