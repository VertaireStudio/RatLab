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


struct i32 {
    // Data
    unsigned int value;

    // Constructor
    func i32() : value(0) {}
    func i32(unsigned int p_value) : value(p_value) {}
    
    template<typename T>
    func i32(T p_value) : value((unsigned int)p_value) {}

    // Getsets
    virtual func unsigned int get() const { return value; }
    virtual func void set(unsigned int p_value) { value = p_value; }

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
    func bool operator<(i32 p_value) { return value < p_value.value; }
    func bool operator<=(i32 p_value) { return value <= p_value.value; }
    func bool operator>(i32 p_value) { return value > p_value.value; }
    func bool operator>=(i32 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I32_MAX) : value -= 1; }
    func void operator++() { value == I32_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::I32; }

    // Math Functions
    func i32 min(i32 p_value) { return value < p_value.value ? value : p_value.value; }
    func i32 max(i32 p_value) { return value > p_value.value ? value : p_value.value; }
    func i32 clamp(i32 p_left, i32 p_right) { return min(p_right).max(p_left); }
    func i32 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }
};