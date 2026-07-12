/************************************/
/*             i32.hpp              */
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
struct i64;
struct f32;
struct f64;


struct i32 {
    static const i32 ZERO;

    // Data
    unsigned int value;

    // Constructor
    func i32() : value(0) {}
    func i32(unsigned int p_value) : value(p_value) {}
    
    template<typename T>
    func i32(T p_value) : value((unsigned int)p_value) {}

    // Getsets
    func unsigned int get() const { return value; }
    func void set(unsigned int p_value) { value = p_value; }

    // Operators
    func i32 operator+(i32 p_value) const { return i32(value + p_value.get()); }
    func void operator+=(i32 p_value) { value += p_value.value; }
    func i32 operator-(i32 p_value) const { return i32(value - p_value.get()); }
    func void operator-=(i32 p_value) { value -= p_value.value; }
    func i32 operator*(i32 p_value) const { return i32(value * p_value.get()); }
    func void operator*=(i32 p_value) { value *= p_value.value; }
    func i32 operator/(i32 p_value) const { return p_value.value == 0 ? i32(I32_MAX) : i32(value / p_value.get()); }
    func void operator/=(i32 p_value) { p_value.value == 0 ? (value = I32_MAX) : (value /= p_value.value); }

    func void operator=(i32 p_value) { value = p_value.value; }
    func bool operator==(i32 p_value) { return value == p_value.value; }
    func bool operator!=(i32 p_value) { return value != p_value.value; }
    func bool operator<(i32 p_value) { return value < p_value.value; }
    func bool operator<=(i32 p_value) { return value <= p_value.value; }
    func bool operator>(i32 p_value) { return value > p_value.value; }
    func bool operator>=(i32 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I32_MAX) : value -= 1; }
    func void operator++() { value == I32_MAX ? (value = 0) : value += 1; }
    func i32 operator!() { return i32(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::I32; }

    // Math Functions
    func i32 min(i32 p_value) { return value < p_value.value ? value : p_value.value; }
    func i32 max(i32 p_value) { return value > p_value.value ? value : p_value.value; }
    func i32 clamp(i32 p_left, i32 p_right) { return min(p_right).max(p_left); }
    func i32 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func i32 i32::ZERO = {0};