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


struct i64 {
    // Data
    unsigned long long value;

    // Constructor
    func i64() : value(0) {}
    func i64(unsigned long long p_value) : value(p_value) {}
    
    template<typename T>
    func i64(T p_value) : value((unsigned long long)p_value) {}

    // Getsets
    virtual func unsigned long long get() const { return value; }
    virtual func void set(unsigned long long p_value) { value = p_value; }

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
    func bool operator<(i64 p_value) { return value < p_value.value; }
    func bool operator<=(i64 p_value) { return value <= p_value.value; }
    func bool operator>(i64 p_value) { return value > p_value.value; }
    func bool operator>=(i64 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = I64_MAX) : value -= 1; }
    func void operator++() { value == I64_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::I64; }

    // Math Functions
    func i64 min(i64 p_value) { return value < p_value.value ? value : p_value.value; }
    func i64 max(i64 p_value) { return value > p_value.value ? value : p_value.value; }
    func i64 clamp(i64 p_left, i64 p_right) { return min(p_right).max(p_left); }
    func i64 sign() { return value < 0 ? -1 : (value > 0 ? 1 : 0); }
};