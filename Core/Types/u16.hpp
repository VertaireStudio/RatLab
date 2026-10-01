/************************************/
/*            u16.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The u16 type: It represents a 16-bit unsigned integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class u16 : public Variant {
    private:
    // The minimum value for the u16 type.
    static constexpr const unsigned short MIN_VALUE = 0;
    // The maximum value for the u16 type.
    static constexpr const unsigned short MAX_VALUE = 65535;

    // The original value of the u16 type.
    unsigned short value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func u16() = default;
    // Constructor with a single unsigned short value.
    func u16(unsigned short p_value) : value(p_value) {}
    // Constructor with an unsigned short value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func u16(unsigned short p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (unsigned short) of the u16 type.
    // If the u16 type is write-only or neutral, returns 0.
    func unsigned short get() const { return can_read() ? value : 0; }
    // Sets the value of the u16 type with the given unsigned short value.
    // If the u16 type is read-only or aggressive, the value will not be set.
    func void set(unsigned short p_value) { if (can_write()) value = p_value; }
    // Sets the value of the u16 type with the given u16 value.
    // If the u16 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the u16 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const u16 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the u16 type.
    // If the u16 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the result of adding two u16 values together.
    func u16 operator+(const u16 &p_other) const { return u16(get() + p_other.get()); }
    // Returns the result of subtracting one u16 value from another.
    func u16 operator-(const u16 &p_other) const { return u16(get() - p_other.get()); }
    // Returns the result of multiplying two u16 values together.
    func u16 operator*(const u16 &p_other) const { return u16(get() * p_other.get()); }
    // Returns the result of dividing one u16 value by another. (avoids division by zero)
    func u16 operator/(const u16 &p_other) const { return p_other.get() == 0 ? u16(MAX_VALUE) : u16(get() / p_other.get()); }
    // Returns the result of modulo operation on two u16 values.
    func u16 operator%(const u16 &p_other) const { return u16(get() % p_other.get()); }
    // Returns the result of bitwise AND operation on two u16 values.
    func u16 operator&(const u16 &p_other) const { return u16(get() & p_other.get()); }
    // Returns the result of bitwise OR operation on two u16 values.
    func u16 operator|(const u16 &p_other) const { return u16(get() | p_other.get()); }
    // Returns the result of bitwise XOR operation on two u16 values.
    func u16 operator^(const u16 &p_other) const { return u16(get() ^ p_other.get()); }

    // Returns the result of adding with an unsigned short value.
    func u16 operator+(unsigned short p_value) const { return u16(get() + p_value); }
    // Returns the result of subtracting with an unsigned short value.
    func u16 operator-(unsigned short p_value) const { return u16(get() - p_value); }
    // Returns the result of multiplying with an unsigned short value.
    func u16 operator*(unsigned short p_value) const { return u16(get() * p_value); }
    // Returns the result of dividing with an unsigned short value. (avoids division by zero)
    func u16 operator/(unsigned short p_value) const { return p_value == 0 ? u16(MAX_VALUE) : u16(get() / p_value); }
    // Returns the result of modulo operation with an unsigned short value.
    func u16 operator%(unsigned short p_value) const { return u16(get() % p_value); }

    // Adds two u16 values together.
    func u16 operator+=(const u16 &p_other) { return u16(get() + p_other.get()); }
    // Subtracts one u16 value from another.
    func u16 operator-=(const u16 &p_other) { return u16(get() - p_other.get()); }
    // Multiplies two u16 values together.
    func u16 operator*=(const u16 &p_other) { return u16(get() * p_other.get()); }
    // Divides one u16 value by another. (avoids division by zero)
    func u16 operator/=(const u16 &p_other) { return p_other.get() == 0 ? u16(MAX_VALUE) : u16(get() / p_other.get()); }
    // Modulo operation with an u16 value.
    func u16 operator%=(const u16 &p_other) { return u16(get() % p_other.get()); }

    // Adds with an unsigned short value.
    func u16 operator+=(unsigned short p_value) { return u16(get() + p_value); }
    // Subtracts an unsigned short value.
    func u16 operator-=(unsigned short p_value) { return u16(get() - p_value); }
    // Multiplies an u16 value by an unsigned short.
    func u16 operator*=(unsigned short p_value) { return u16(get() * p_value); }
    // Divides an u16 value by an unsigned short. (avoids division by zero)
    func u16 operator/=(unsigned short p_value) { return p_value == 0 ? u16(MAX_VALUE) : u16(get() / p_value); }
    // Modulo operation with an unsigned short value.
    func u16 operator%=(unsigned short p_value) { return u16(get() % p_value); }

    // Negates a u16 value.
    func u16 operator-() const { return u16(-get()); }
    // Bitwise NOT operation on a u16 value.
    func u16 operator~() const { return u16(~get()); }
    // Left shift operation on an u16 value.
    func u16 operator<<(unsigned short p_value) const { return u16(get() << p_value); }
    // Right shift operation on an u16 value.
    func u16 operator>>(unsigned short p_value) const { return u16(get() >> p_value); }

    // Pre-increment operator.
    func u16 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : u16(0); }
    // Post-increment operator.
    func u16 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : u16(0); }
    // Pre-decrement operator.
    func u16 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : u16(0); }
    // Post-decrement operator.
    func u16 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : u16(0); }

    // Assignment operator with another u16 value.
    func void operator=(const u16 &p_other) { set(p_other); }
    // Assignment operator with an unsigned short value.
    func void operator=(unsigned short p_value) { set(p_value); }

    // Equality operator with another u16 value.
    func bool operator==(const u16 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another u16 value.
    func bool operator!=(const u16 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another u16 value.
    func bool operator<(const u16 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another u16 value.
    func bool operator>(const u16 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another u16 value.
    func bool operator<=(const u16 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another u16 value.
    func bool operator>=(const u16 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with an unsigned short value.
    func bool operator==(unsigned short p_other) const { return get() == p_other; }
    // Inequality operator with an unsigned short value.
    func bool operator!=(unsigned short p_other) const { return get() != p_other; }
    // Less than operator with an unsigned short value.
    func bool operator<(unsigned short p_other) const { return get() < p_other; }
    // Greater than operator with an unsigned short value.
    func bool operator>(unsigned short p_other) const { return get() > p_other; }
    // Less than or equal operator with an unsigned short value.
    func bool operator<=(unsigned short p_other) const { return get() <= p_other; }
    // Greater than or equal operator with an unsigned short value.
    func bool operator>=(unsigned short p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the u16 value.
    // Returns 0 if the value cannot be read.
    func u16 abs() const { return !can_read() ? u16(0) : (get() < 0 ? u16(-get()) : u16(get())); }

    // Returns the lower value between two u16 values.
    // Returns 0 if the value cannot be read.
    func u16 min(const u16 &p_other) const { return !can_read() ? u16(0) : (get() < p_other.get() ? u16(get()) : u16(p_other.get())); }
    // Returns the lower value between an u16 value and an unsigned short.
    // Returns 0 if the value cannot be read.
    func u16 min(unsigned short p_value) const { return !can_read() ? u16(0) : (get() < p_value ? u16(get()) : u16(p_value)); }

    // Returns the higher value between two u16 values.
    // Returns 0 if the value cannot be read.
    func u16 max(const u16 &p_other) const { return !can_read() ? u16(0) : (get() > p_other.get() ? u16(get()) : u16(p_other.get())); }
    // Returns the higher value between an u16 value and an unsigned short.
    // Returns 0 if the value cannot be read.
    func u16 max(unsigned short p_value) const { return !can_read() ? u16(0) : (get() > p_value ? u16(get()) : u16(p_value)); }

    // Returns the clamped value between two u16 values.
    // Returns 0 if the value cannot be read.
    func u16 clamp(const u16 &p_min, const u16 &p_max) const { return !can_read() ? u16(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an u16 value and an unsigned short.
    // Returns 0 if the value cannot be read.
    func u16 clamp(unsigned short p_min, unsigned short p_max) const { return !can_read() ? u16(0) : (min(p_min).max(p_max)); }
};
