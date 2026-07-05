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


struct i16 {
    // Data
    unsigned short value;

    // Constructor
    func i16() : value(0) {}
    func i16(unsigned short p_value) : value(p_value) {}
    
    template<typename T>
    func i16(T p_value) : value((unsigned short)p_value) {}

    // Getsets
    virtual func unsigned short get() const { return value; }
    virtual func void set(unsigned short p_value) { value = p_value; }

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
    func bool operator<(i16 p_value) { return value < p_value.value; }
    func bool operator<=(i16 p_value) { return value <= p_value.value; }
    func bool operator>(i16 p_value) { return value > p_value.value; }
    func bool operator>=(i16 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I16_MAX) : value -= 1; }
    func void operator++() { value == I16_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::I16; }

    // Math Functions
    func i16 min(i16 p_value) { return value < p_value.value ? value : p_value.value; }
    func i16 max(i16 p_value) { return value > p_value.value ? value : p_value.value; }
    func i16 clamp(i16 p_left, i16 p_right) { return min(p_right).max(p_left); }
    func i16 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }
};