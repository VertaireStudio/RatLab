/************************************/
/*             i16.hpp              */
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
struct i32;
struct i64;
struct f32;
struct f64;


struct i16 {
    static const i16 ZERO;

    // Data
    unsigned short value;

    // Constructor
    func i16() : value(0) {}
    func i16(unsigned short p_value) : value(p_value) {}
    
    template<typename T>
    func i16(T p_value) : value((unsigned short)p_value) {}

    // Getsets
    func unsigned short get() const { return value; }
    func void set(unsigned short p_value) { value = p_value; }

    // Operators
    func i16 operator+(i16 p_value) const { return i16(value + p_value.get()); }
    func void operator+=(i16 p_value) { value += p_value.value; }
    func i16 operator-(i16 p_value) const { return i16(value - p_value.get()); }
    func void operator-=(i16 p_value) { value -= p_value.value; }
    func i16 operator*(i16 p_value) const { return i16(value * p_value.get()); }
    func void operator*=(i16 p_value) { value *= p_value.value; }
    func i16 operator/(i16 p_value) const { return p_value.value == 0 ? i16(I16_MAX) : i16(value / p_value.get()); }
    func void operator/=(i16 p_value) { p_value.value == 0 ? (value = I16_MAX) : (value /= p_value.value); }

    func void operator=(i16 p_value) { value = p_value.value; }
    func bool operator==(i16 p_value) { return value == p_value.value; }
    func bool operator!=(i16 p_value) { return value != p_value.value; }
    func bool operator<(i16 p_value) { return value < p_value.value; }
    func bool operator<=(i16 p_value) { return value <= p_value.value; }
    func bool operator>(i16 p_value) { return value > p_value.value; }
    func bool operator>=(i16 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? value = I16_MAX : value -= 1; }
    func void operator++() { value == I16_MAX ? value = 0 : value += 1; }
    func i16 operator!() { return i16(!value); }

    // Helper Functions
    func TYPES get_type() { return TYPES::I16; }

    // Math Functions
    func i16 min(i16 p_value) { return value < p_value.value ? value : p_value.value; }
    func i16 max(i16 p_value) { return value > p_value.value ? value : p_value.value; }
    func i16 clamp(i16 p_left, i16 p_right) { return min(p_right).max(p_left); }
    func i16 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func i16 i16::ZERO = {0};