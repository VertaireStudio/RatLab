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


struct u32 {
    // Data
    unsigned int value;

    // Constructor
    func u32() : value(0) {}
    func u32(unsigned int p_value) : value(p_value) {}
    
    template<typename T>
    func u32(T p_value) : value((unsigned int)p_value) {}

    // Getsets
    virtual func unsigned int get() const { return value; }
    virtual func void set(unsigned int p_value) { value = p_value; }

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
    func bool operator<(u32 p_value) { return value < p_value.value; }
    func bool operator<=(u32 p_value) { return value <= p_value.value; }
    func bool operator>(u32 p_value) { return value > p_value.value; }
    func bool operator>=(u32 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U32_MAX) : value -= 1; }
    func void operator++() { value == U32_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::U32; }

    // Math Functions
    func u32 min(u32 p_value) { return value < p_value.value ? value : p_value.value; }
    func u32 max(u32 p_value) { return value > p_value.value ? value : p_value.value; }
    func u32 clamp(u32 p_left, u32 p_right) { return min(p_right).max(p_left); }
};