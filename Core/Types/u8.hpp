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
    func u8() noexcept = default;
    // Constructor with a single unsigned char value.
    func u8(unsigned char p_value) noexcept : value(p_value) {}
    // Constructor with an unsigned char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func u8(unsigned char p_value, Accessability p_access) noexcept : value(p_value) { access = p_access; }
    /*------------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (unsigned char) of the u8 type.
    // If the u8 type is write-only or neutral, returns 0.
    func unsigned char get() const noexcept { return can_read() ? value : 0; }
    // Returns the type of the value stored in this Variant.
    static func Variant::Types get_type() noexcept { return Variant::Types::U8; }
    // Sets the value of the u8 type with the given unsigned char value.
    // If the u8 type is read-only or Aggressive, the value will not be set.
    func void set(unsigned char p_value) noexcept { if (can_write()) value = p_value; }
    // Sets the value of the u8 type with the given u8 value.
    // If the u8 type (which is being set from) is read-only or Aggressive, the value will not be set.
    // If the u8 type (from the parameter) is write-only or Neutral, the value will not be set.
    func void set(const u8 &p_other) noexcept { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the u8 type.
    // If the u8 type is Aggressive, Neutral, or Passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) noexcept { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the result of adding this value with another u8 value.
    func u8 operator+(const u8 &p_other) const noexcept { return u8(get() + p_other.get()); }
    // Returns the result of subtracting this value with another u8 value.
    func u8 operator-(const u8 &p_other) const noexcept { return u8(get() - p_other.get()); }
    // Returns the result of multiplying this value with another u8 value.
    func u8 operator*(const u8 &p_other) const noexcept { return u8(get() * p_other.get()); }
    // Returns the result of dividing this value with another u8 value.
    func u8 operator/(const u8 &p_other) const noexcept { return u8(get() / p_other.get()); }
    // Returns the result of moduloing this value with another u8 value.
    func u8 operator%(const u8 &p_other) const noexcept { return u8(get() - (get() / p_other.get()) * p_other.get()); }
    // Returns the result of masking (Bitwise AND) this value with another u8 value.
    func u8 operator&(const u8 &p_other) const noexcept { return u8(get() & p_other.get()); }
    // Returns the result of masking (Bitwise OR) this value with another u8 value.
    func u8 operator|(const u8 &p_other) const noexcept { return u8(get() | p_other.get()); }
    // Returns the result of masking (Bitwise XOR) this value with another u8 value.
    func u8 operator^(const u8 &p_other) const noexcept { return u8(get() ^ p_other.get()); }

    // Returns the result of adding this value with an unsigned char value.
    func u8 operator+(unsigned char p_value) const noexcept { return u8(get() + p_value); }
    // Returns the result of subtracting this value with an unsigned char value.
    func u8 operator-(unsigned char p_value) const noexcept { return u8(get() - p_value); }
    // Returns the result of multiplying this value with an unsigned char value.
    func u8 operator*(unsigned char p_value) const noexcept { return u8(get() * p_value); }
    // Returns the result of dividing this value with an unsigned char value.
    func u8 operator/(unsigned char p_value) const noexcept { return u8(get() / p_value); }
    // Returns the result of moduloing this value with an unsigned char value.
    func u8 operator%(unsigned char p_value) const noexcept { return u8(get() - (get() / p_value) * p_value); }

    // Adds two u8 values together.
    func void operator+=(const u8 &p_other) noexcept { set(get() + p_other.get()); }
    // Subtracts one u8 value from another.
    func void operator-=(const u8 &p_other) noexcept { set(get() - p_other.get()); }
    // Multiplies two u8 values together.
    func void operator*=(const u8 &p_other) noexcept { set(get() * p_other.get()); }
    // Divides one u8 value by another.
    func void operator/=(const u8 &p_other) noexcept { set(get() / p_other.get()); }
    // Modulo operation with an u8 value.
    func void operator%=(const u8 &p_other) noexcept { set(get() - (get() / p_other.get()) * p_other.get()); }

    // Adds with an unsigned char value.
    func void operator+=(unsigned char p_value) noexcept { set(get() + p_value); }
    // Subtracts an unsigned char value.
    func void operator-=(unsigned char p_value) noexcept { set(get() - p_value); }
    // Multiplies an u8 value by an unsigned char.
    func void operator*=(unsigned char p_value) noexcept { set(get() * p_value); }
    // Divides an u8 value by an unsigned char.
    func void operator/=(unsigned char p_value) noexcept { set(get() / p_value); }
    // Modulos with an unsigned char value.
    func void operator%=(unsigned char p_value) noexcept { set(get() - (get() / p_value) * p_value); }

    // Negates a u8 value.
    func u8 operator-() const noexcept { return u8(-get()); }
    // Flips the value's bits (from 0 to 1 - and 1 to 0).
    func u8 operator~() const noexcept { return u8(~get()); }

    // Left shift operation on an u8 value.
    func u8 operator<<(const u8 &p_value) const noexcept { return u8(get() << p_value.get()); }
    // Right shift operation on an u8 value.
    func u8 operator>>(const u8 &p_value) const noexcept { return u8(get() >> p_value.get()); }
    // Left shift operation on an u8 value.
    func u8 operator<<(unsigned char p_value) const noexcept { return u8(get() << p_value); }
    // Right shift operation on an u8 value.
    func u8 operator>>(unsigned char p_value) const noexcept { return u8(get() >> p_value); }

    // Increments the value by one, then returns the result.
    func u8 operator++() noexcept { if (can_write()) set(get() + 1); return can_read() ? *this : u8(0); }
    // Increments the value by one.
    func void operator++(int) noexcept { if (can_write()) set(get() + 1); }
    // Decrements the value by one, then returns the result.
    func u8 operator--() noexcept { if (can_write()) set(get() - 1); return can_read() ? *this : u8(0); }
    // Decrements the value by one.
    func void operator--(int) noexcept { if (can_write()) set(get() - 1); }

    // Equality operator with another u8 value.
    func bool operator==(const u8 &p_other) const noexcept { return get() == p_other.get(); }
    // Inequality operator with another u8 value.
    func bool operator!=(const u8 &p_other) const noexcept { return get() != p_other.get(); }
    // Less than operator with another u8 value.
    func bool operator<(const u8 &p_other) const noexcept { return get() < p_other.get(); }
    // Greater than operator with another u8 value.
    func bool operator>(const u8 &p_other) const noexcept { return get() > p_other.get(); }
    // Less than or equal operator with another u8 value.
    func bool operator<=(const u8 &p_other) const noexcept { return get() <= p_other.get(); }
    // Greater than or equal operator with another u8 value.
    func bool operator>=(const u8 &p_other) const noexcept { return get() >= p_other.get(); }

    // Equality operator with an unsigned char value.
    func bool operator==(unsigned char p_other) const noexcept { return get() == p_other; }
    // Inequality operator with an unsigned char value.
    func bool operator!=(unsigned char p_other) const noexcept { return get() != p_other; }
    // Less than operator with an unsigned char value.
    func bool operator<(unsigned char p_other) const noexcept { return get() < p_other; }
    // Greater than operator with an unsigned char value.
    func bool operator>(unsigned char p_other) const noexcept { return get() > p_other; }
    // Less than or equal operator with an unsigned char value.
    func bool operator<=(unsigned char p_other) const noexcept { return get() <= p_other; }
    // Greater than or equal operator with an unsigned char value.
    func bool operator>=(unsigned char p_other) const noexcept { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the lower value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 min(const u8 &p_other) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get(), other = p_other.get();
        return self < other ? u8(self) : u8(other);
    }
    // Returns the lower value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 min(unsigned char p_value) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get();
        return self < p_value ? u8(self) : u8(p_value);
    }

    // Returns the higher value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 max(const u8 &p_other) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get(), other = p_other.get();
        return self > other ? u8(self) : u8(other);
    }
    // Returns the higher value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 max(unsigned char p_value) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get();
        return self > p_value ? u8(self) : u8(p_value);
    }

    // Returns the clamped value between two u8 values.
    // Returns 0 if the value cannot be read.
    func u8 clamp(const u8 &p_min, const u8 &p_max) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get(), lower = p_min.get(), upper = p_max.get();
        const unsigned char clamped = self < lower ? self : lower;
        return u8(clamped > upper ? clamped : upper);
    }
    // Returns the clamped value between an u8 value and an unsigned char.
    // Returns 0 if the value cannot be read.
    func u8 clamp(unsigned char p_min, unsigned char p_max) const noexcept {
        if (!can_read()) return u8(0);
        const unsigned char self = get();
        const unsigned char clamped = self < p_min ? self : p_min;
        return u8(clamped > p_max ? clamped : p_max);
    }
};
