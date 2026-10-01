/************************************/
/*             u8.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The u8 type: It represents an 8-bit unsigned integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class u8 : public Variant {
    private:
    // The minimum value for the u8 type.
    static constexpr const unsigned char MIN_VALUE = 0;
    // The maximum value for the u8 type.
    static constexpr const unsigned char MAX_VALUE = 255;

    // The original value of the u8 type.
    unsigned char value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func u8() = default;
    // Constructor with a single unsigned char value.
    func u8(unsigned char p_value) : value(p_value) {}
    // Constructor with an unsigned char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func u8(unsigned char p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (unsigned char) of the u8 type.
    // If the u8 type is write-only or neutral, returns 0.
    func unsigned char get() const { return can_read() ? value : 0; }
    // Returns the type of the value stored in this Variant.
    static func Variant::Types get_type() { return Variant::Types::U8; }
    // Sets the value of the u8 type with the given unsigned char value.
    // If the u8 type is read-only or aggressive, the value will not be set.
    func void set(unsigned char p_value) { if (can_write()) value = p_value; }
    // Sets the value of the u8 type with the given u8 value.
    // If the u8 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the u8 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const u8 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the u8 type.
    // If the u8 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the result of adding two u8 values together.
    func u8 operator+(const u8 &p_other) const { return u8(get() + p_other.get()); }
    // Returns the result of subtracting one u8 value from another.
    func u8 operator-(const u8 &p_other) const { return u8(get() - p_other.get()); }
    // Returns the result of multiplying two u8 values together.
    func u8 operator*(const u8 &p_other) const { return u8(get() * p_other.get()); }
    // Returns the result of dividing one u8 value by another. (avoids division by zero)
    func u8 operator/(const u8 &p_other) const { return p_other.get() == 0 ? u8(MAX_VALUE) : u8(get() / p_other.get()); }
    // Returns the result of modulo operation on two u8 values.
    func u8 operator%(const u8 &p_other) const { return u8(get() % p_other.get()); }
    // Returns the result of bitwise AND operation on two u8 values.
    func u8 operator&(const u8 &p_other) const { return u8(get() & p_other.get()); }
    // Returns the result of bitwise OR operation on two u8 values.
    func u8 operator|(const u8 &p_other) const { return u8(get() | p_other.get()); }
    // Returns the result of bitwise XOR operation on two u8 values.
    func u8 operator^(const u8 &p_other) const { return u8(get() ^ p_other.get()); }

    // Returns the result of adding with an unsigned char value.
    func u8 operator+(unsigned char p_value) const { return u8(get() + p_value); }
    // Returns the result of subtracting with an unsigned char value.
    func u8 operator-(unsigned char p_value) const { return u8(get() - p_value); }
    // Returns the result of multiplying with an unsigned char value.
    func u8 operator*(unsigned char p_value) const { return u8(get() * p_value); }
    // Returns the result of dividing with an unsigned char value. (avoids division by zero)
    func u8 operator/(unsigned char p_value) const { return p_value == 0 ? u8(MAX_VALUE) : u8(get() / p_value); }
    // Returns the result of modulo operation with an unsigned char value.
    func u8 operator%(unsigned char p_value) const { return u8(get() % p_value); }

    // Adds two u8 values together.
    func u8 operator+=(const u8 &p_other) { return u8(get() + p_other.get()); }
    // Subtracts one u8 value from another.
    func u8 operator-=(const u8 &p_other) { return u8(get() - p_other.get()); }
    // Multiplies two u8 values together.
    func u8 operator*=(const u8 &p_other) { return u8(get() * p_other.get()); }
    // Divides one u8 value by another. (avoids division by zero)
    func u8 operator/=(const u8 &p_other) { return p_other.get() == 0 ? u8(MAX_VALUE) : u8(get() / p_other.get()); }
    // Modulo operation with an u8 value.
    func u8 operator%=(const u8 &p_other) { return u8(get() % p_other.get()); }

    // Adds with an unsigned char value.
    func u8 operator+=(unsigned char p_value) { return u8(get() + p_value); }
    // Subtracts an unsigned char value.
    func u8 operator-=(unsigned char p_value) { return u8(get() - p_value); }
    // Multiplies an u8 value by an unsigned char.
    func u8 operator*=(unsigned char p_value) { return u8(get() * p_value); }
    // Divides an u8 value by an unsigned char. (avoids division by zero)
    func u8 operator/=(unsigned char p_value) { return p_value == 0 ? u8(MAX_VALUE) : u8(get() / p_value); }
    // Modulo operation with an unsigned char value.
    func u8 operator%=(unsigned char p_value) { return u8(get() % p_value); }

    // Negates a u8 value.
    func u8 operator-() const { return u8(-get()); }
    // Bitwise NOT operation on a u8 value.
    func u8 operator~() const { return u8(~get()); }

    // Left shift operation on an u8 value.
    func u8 operator<<(const u8 &p_value) const { return u8(get() << p_value.get()); }
    // Right shift operation on an u8 value.
    func u8 operator>>(const u8 &p_value) const { return u8(get() >> p_value.get()); }
    // Left shift operation on an u8 value.
    func u8 operator<<(unsigned char p_value) const { return u8(get() << p_value); }
    // Right shift operation on an u8 value.
    func u8 operator>>(unsigned char p_value) const { return u8(get() >> p_value); }

    // Pre-increment operator.
    func u8 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : u8(0); }
    // Post-increment operator.
    func u8 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : u8(0); }
    // Pre-decrement operator.
    func u8 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : u8(0); }
    // Post-decrement operator.
    func u8 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : u8(0); }

    // Assignment operator with another u8 value.
    func void operator=(const u8 &p_other) { set(p_other); }
    // Assignment operator with an unsigned char value.
    func void operator=(unsigned char p_value) { set(p_value); }

    // Equality operator with another u8 value.
    func bool operator==(const u8 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another u8 value.
    func bool operator!=(const u8 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another u8 value.
    func bool operator<(const u8 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another u8 value.
    func bool operator>(const u8 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another u8 value.
    func bool operator<=(const u8 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another u8 value.
    func bool operator>=(const u8 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with an unsigned char value.
    func bool operator==(unsigned char p_other) const { return get() == p_other; }
    // Inequality operator with an unsigned char value.
    func bool operator!=(unsigned char p_other) const { return get() != p_other; }
    // Less than operator with an unsigned char value.
    func bool operator<(unsigned char p_other) const { return get() < p_other; }
    // Greater than operator with an unsigned char value.
    func bool operator>(unsigned char p_other) const { return get() > p_other; }
    // Less than or equal operator with an unsigned char value.
    func bool operator<=(unsigned char p_other) const { return get() <= p_other; }
    // Greater than or equal operator with an unsigned char value.
    func bool operator>=(unsigned char p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the u8 value.
    // Returns 0 if the value cannot be read.
    func u8 abs() const { return !can_read() ? u8(0) : (get() < 0 ? u8(-get()) : u8(get())); }

    // Returns the lower value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 min(const u8 &p_other) const { return !can_read() ? u8(0) : (get() < p_other.get() ? u8(get()) : u8(p_other.get())); }
    // Returns the lower value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 min(unsigned char p_value) const { return !can_read() ? u8(0) : (get() < p_value ? u8(get()) : u8(p_value)); }

    // Returns the higher value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 max(const u8 &p_other) const { return !can_read() ? u8(0) : (get() > p_other.get() ? u8(get()) : u8(p_other.get())); }
    // Returns the higher value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 max(unsigned char p_value) const { return !can_read() ? u8(0) : (get() > p_value ? u8(get()) : u8(p_value)); }

    // Returns the clamped value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 clamp(const u8 &p_min, const u8 &p_max) const { return !can_read() ? u8(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 clamp(unsigned char p_min, unsigned char p_max) const { return !can_read() ? u8(0) : (min(p_min).max(p_max)); }
};
