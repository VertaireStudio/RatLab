/************************************/
/*             u16.hpp              */
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
struct u32;
struct u64;
struct i8;
struct i16;
struct i32;
struct i64;
struct f32;
struct f64;


struct u16 {
    static const u16 ZERO;

    // Data
    unsigned short value;

    // Constructor
    func u16() : value(0) {}
    func u16(unsigned short p_value) : value(p_value) {}
    
    template<typename T>
    func u16(T p_value) : value((unsigned short)p_value) {}

    // Getsets
    func unsigned short get() const { return value; }
    func void set(unsigned short p_value) { value = p_value; }

    // Operators
    func u16 operator+(u16 p_value) const { return u16(value + p_value.get()); }
    func void operator+=(u16 p_value) { value += p_value.value; }
    func u16 operator-(u16 p_value) const { return u16(value - p_value.get()); }
    func void operator-=(u16 p_value) { value -= p_value.value; }
    func u16 operator*(u16 p_value) const { return u16(value * p_value.get()); }
    func void operator*=(u16 p_value) { value *= p_value.value; }
    func u16 operator/(u16 p_value) const { return p_value.value == 0 ? u16(U16_MAX) : u16(value / p_value.get()); }
    func void operator/=(u16 p_value) { p_value.value == 0 ? (value = U16_MAX) : (value /= p_value.value); }

    func void operator=(u16 p_value) { value = p_value.value; }
    func bool operator==(u16 p_value) { return value == p_value.value; }
    func bool operator!=(u16 p_value) { return value != p_value.value; }
    func bool operator<(u16 p_value) { return value < p_value.value; }
    func bool operator<=(u16 p_value) { return value <= p_value.value; }
    func bool operator>(u16 p_value) { return value > p_value.value; }
    func bool operator>=(u16 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U16_MAX) : value -= 1; }
    func void operator++() { value == U16_MAX ? (value = 0) : value += 1; }
    func u16 operator!() { return u16(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::U16; }

    // Math Functions
    func u16 min(u16 p_value) { return value < p_value.value ? value : p_value.value; }
    func u16 max(u16 p_value) { return value > p_value.value ? value : p_value.value; }
    func u16 clamp(u16 p_left, u16 p_right) { return min(p_right).max(p_left); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func u16 u16::ZERO = {0};