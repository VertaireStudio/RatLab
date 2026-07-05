/************************************/
/*             u64.hpp              */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/type_configs.hpp"
#include "../Essentials/def_configs.hpp"


struct u64 {
    // Data
    unsigned long long value;

    // Constructor
    func u64() : value(0) {}
    func u64(unsigned long long p_value) : value(p_value) {}
    
    template<typename T>
    func u64(T p_value) : value((unsigned long long)p_value) {}

    // Getsets
    virtual func unsigned long long get() const { return value; }
    virtual func void set(unsigned long long p_value) { value = p_value; }

    // Operators
    func u64 operator+(u64 p_value) const { return u64(value + p_value.get()); }
    func void operator+=(u64 p_value) { value += p_value.value; }
    func u64 operator-(u64 p_value) const { return u64(value - p_value.get()); }
    func void operator-=(u64 p_value) { value -= p_value.value; }
    func u64 operator*(u64 p_value) const { return u64(value * p_value.get()); }
    func void operator*=(u64 p_value) { value *= p_value.value; }
    func u64 operator/(u64 p_value) const { return p_value.value == 0 ? u64(U64_MAX) : u64(value / p_value.get()); }
    func void operator/=(u64 p_value) { p_value.value == 0 ? (value = U64_MAX) : (value /= p_value.value); }

    func void operator=(u64 p_value) { value = p_value.value; }
    func bool operator==(u64 p_value) { return value == p_value.value; }
    func bool operator<(u64 p_value) { return value < p_value.value; }
    func bool operator<=(u64 p_value) { return value <= p_value.value; }
    func bool operator>(u64 p_value) { return value > p_value.value; }
    func bool operator>=(u64 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U64_MAX) : value -= 1; }
    func void operator++() { value == U64_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::U64; }

    // Math Functions
    func u64 min(u64 p_value) { return value < p_value.value ? value : p_value.value; }
    func u64 max(u64 p_value) { return value > p_value.value ? value : p_value.value; }
    func u64 clamp(u64 p_left, u64 p_right) { return min(p_right).max(p_left); }
};