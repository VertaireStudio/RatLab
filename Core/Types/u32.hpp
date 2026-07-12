/************************************/
/*             u32.hpp              */
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
struct u64;
struct i8;
struct i16;
struct i32;
struct i64;
struct f32;
struct f64;


struct u32 {
    static const u32 ZERO;

    // Data
    unsigned int value;

    // Constructor
    func u32() : value(0) {}
    func u32(unsigned int p_value) : value(p_value) {}
    
    template<typename T>
    func u32(T p_value) : value((unsigned int)p_value) {}

    // Getsets
    func unsigned int get() const { return value; }
    func void set(unsigned int p_value) { value = p_value; }

    // Operators
    func u32 operator+(u32 p_value) const { return u32(value + p_value.get()); }
    func void operator+=(u32 p_value) { value += p_value.value; }
    func u32 operator-(u32 p_value) const { return u32(value - p_value.get()); }
    func void operator-=(u32 p_value) { value -= p_value.value; }
    func u32 operator*(u32 p_value) const { return u32(value * p_value.get()); }
    func void operator*=(u32 p_value) { value *= p_value.value; }
    func u32 operator/(u32 p_value) const { return p_value.value == 0 ? u32(U32_MAX) : u32(value / p_value.get()); }
    func void operator/=(u32 p_value) { p_value.value == 0 ? (value = U32_MAX) : (value /= p_value.value); }

    func void operator=(u32 p_value) { value = p_value.value; }
    func bool operator==(u32 p_value) { return value == p_value.value; }
    func bool operator!=(u32 p_value) { return value != p_value.value; }
    func bool operator<(u32 p_value) { return value < p_value.value; }
    func bool operator<=(u32 p_value) { return value <= p_value.value; }
    func bool operator>(u32 p_value) { return value > p_value.value; }
    func bool operator>=(u32 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U32_MAX) : value -= 1; }
    func void operator++() { value == U32_MAX ? (value = 0) : value += 1; }
    func u32 operator!() { return u32(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::U32; }

    // Math Functions
    func u32 min(u32 p_value) { return value < p_value.value ? value : p_value.value; }
    func u32 max(u32 p_value) { return value > p_value.value ? value : p_value.value; }
    func u32 clamp(u32 p_left, u32 p_right) { return min(p_right).max(p_left); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func u32 u32::ZERO = {0};