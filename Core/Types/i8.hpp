/************************************/
/*             i8.hpp               */
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
struct i16;
struct i32;
struct i64;
struct f32;
struct f64;


struct i8 {
    // Data
    unsigned char value;

    // Constructor
    func i8() : value(0) {}
    func i8(unsigned char p_value) : value(p_value) {}
    
    template<typename T>
    func i8(T p_value) : value((unsigned char)p_value) {}

    // Getsets
    func unsigned char get() const { return value; }
    func void set(unsigned char p_value) { value = p_value; }

    // Operators
    func i8 operator+(i8 p_value) const { return i8(value + p_value.get()); }
    func void operator+=(i8 p_value) { value += p_value.value; }
    func i8 operator-(i8 p_value) const { return i8(value - p_value.get()); }
    func void operator-=(i8 p_value) { value -= p_value.value; }
    func i8 operator*(i8 p_value) const { return i8(value * p_value.get()); }
    func void operator*=(i8 p_value) { value *= p_value.value; }
    func i8 operator/(i8 p_value) const { return p_value.value == 0 ? i8(I8_MAX) : i8(value / p_value.get()); }
    func void operator/=(i8 p_value) { p_value.value == 0 ? (value = I8_MAX) : (value /= p_value.value); }

    func void operator=(i8 p_value) { value = p_value.value; }
    func bool operator==(i8 p_value) { return value == p_value.value; }
    func bool operator!=(i8 p_value) { return value != p_value.value; }
    func bool operator<(i8 p_value) { return value < p_value.value; }
    func bool operator<=(i8 p_value) { return value <= p_value.value; }
    func bool operator>(i8 p_value) { return value > p_value.value; }
    func bool operator>=(i8 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I8_MAX) : value -= 1; }
    func void operator++() { value == I8_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::I8; }

    // Math Functions
    func i8 min(i8 p_value) { return value < p_value.value ? value : p_value.value; }
    func i8 max(i8 p_value) { return value > p_value.value ? value : p_value.value; }
    func i8 clamp(i8 p_left, i8 p_right) { return min(p_right).max(p_left); }
    func i8 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

    // Converters

    func operator Bool();
    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};