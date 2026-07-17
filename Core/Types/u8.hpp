/************************************/
/*             u8.hpp               */
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
struct u16;
struct u32;
struct u64;
struct i8;
struct i16;
struct i32;
struct i64;
struct f32;
struct f64;


struct u8 {
    static const u8 ZERO;

    // Data
    unsigned char value;

    // Constructor
    func u8() : value(0) {}
    func u8(unsigned char p_value) : value(p_value) {}
    
    template<typename T>
    func u8(T p_value) : value((unsigned char)p_value) {}

    // Getsets
    func unsigned char get() const { return value; }
    func void set(unsigned char p_value) { value = p_value; }

    // Operators
    func u8 operator+(u8 p_value) const { return u8(value + p_value.get()); }
    func void operator+=(u8 p_value) { value += p_value.value; }
    func u8 operator-(u8 p_value) const { return u8(value - p_value.get()); }
    func void operator-=(u8 p_value) { value -= p_value.value; }
    func u8 operator*(u8 p_value) const { return u8(value * p_value.get()); }
    func void operator*=(u8 p_value) { value *= p_value.value; }
    func u8 operator/(u8 p_value) const { return p_value.value == 0 ? u8(U8_MAX) : u8(value / p_value.get()); }
    func void operator/=(u8 p_value) { p_value.value == 0 ? (value = U8_MAX) : (value /= p_value.value); }

    func bool operator==(u8 p_value) { return value == p_value.value; }
    func bool operator!=(u8 p_value) { return value != p_value.value; }
    func bool operator<(u8 p_value) { return value < p_value.value; }
    func bool operator<=(u8 p_value) { return value <= p_value.value; }
    func bool operator>(u8 p_value) { return value > p_value.value; }
    func bool operator>=(u8 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U8_MAX) : value -= 1; }
    func void operator++() { value == U8_MAX ? (value = 0) : value += 1; }
    func u8 operator!() { return u8(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::U8; }

    // Math Functions
    func u8 min(u8 p_value) { return value < p_value.value ? value : p_value.value; }
    func u8 max(u8 p_value) { return value > p_value.value ? value : p_value.value; }
    func u8 clamp(u8 p_left, u8 p_right) { return min(p_right).max(p_left); }

    // Converters

    func operator Bool();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func u8 u8::ZERO = {0};