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


struct u16 {
    // Data
    unsigned short value;

    // Constructor
    func u16() : value(0) {}
    func u16(unsigned short p_value) : value(p_value) {}
    
    template<typename T>
    func u16(T p_value) : value((unsigned short)p_value) {}

    // Getsets
    virtual func unsigned short get() const { return value; }
    virtual func void set(unsigned short p_value) { value = p_value; }

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
    func bool operator<(u16 p_value) { return value < p_value.value; }
    func bool operator<=(u16 p_value) { return value <= p_value.value; }
    func bool operator>(u16 p_value) { return value > p_value.value; }
    func bool operator>=(u16 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U16_MAX) : value -= 1; }
    func void operator++() { value == U16_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::U16; }

    // Math Functions
    func u16 min(u16 p_value) { return value < p_value.value ? value : p_value.value; }
    func u16 max(u16 p_value) { return value > p_value.value ? value : p_value.value; }
    func u16 clamp(u16 p_left, u16 p_right) { return min(p_right).max(p_left); }
};