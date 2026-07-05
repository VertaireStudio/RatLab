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


struct u8 {
    // Data
    unsigned char value;

    // Constructor
    func u8() : value(0) {}
    func u8(unsigned char p_value) : value(p_value) {}
    
    template<typename T>
    func u8(T p_value) : value((unsigned char)p_value) {}

    // Getsets
    virtual func unsigned char get() const { return value; }
    virtual func void set(unsigned char p_value) { value = p_value; }

    // Operators
    func u8 operator+(u8 p_value) const { return u8(value + p_value.get()); }
    func void operator+=(u8 p_value) { value += p_value.value; }
    func u8 operator-(u8 p_value) const { return u8(value - p_value.get()); }
    func void operator-=(u8 p_value) { value -= p_value.value; }
    func u8 operator*(u8 p_value) const { return u8(value * p_value.get()); }
    func void operator*=(u8 p_value) { value *= p_value.value; }
    func u8 operator/(u8 p_value) const { return p_value.value == 0 ? u8(U8_MAX) : u8(value / p_value.get()); }
    func void operator/=(u8 p_value) { p_value.value == 0 ? (value = U8_MAX) : (value /= p_value.value); }

    func void operator=(u8 p_value) { value = p_value.value; }
    func bool operator==(u8 p_value) { return value == p_value.value; }
    func bool operator<(u8 p_value) { return value < p_value.value; }
    func bool operator<=(u8 p_value) { return value <= p_value.value; }
    func bool operator>(u8 p_value) { return value > p_value.value; }
    func bool operator>=(u8 p_value) { return value >= p_value.value; }

    func void operator--() { value == 0 ? (value = U8_MAX) : value -= 1; }
    func void operator++() { value == U8_MAX ? (value = 0) : value += 1; }

    // Helper Functions
    func TYPES get_type() { return TYPES::U8; }

    // Math Functions
    func u8 min(u8 p_value) { return value < p_value.value ? value : p_value.value; }
    func u8 max(u8 p_value) { return value > p_value.value ? value : p_value.value; }
    func u8 clamp(u8 p_left, u8 p_right) { return min(p_right).max(p_left); }
};