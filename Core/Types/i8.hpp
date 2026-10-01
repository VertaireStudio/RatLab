/************************************/
/*             i8.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The i8 type: It represents an 8-bit signed integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class i8 : public Variant {
    private:
    // The minimum value for the i8 type.
    static constexpr const signed char MIN_VALUE = -128;
    // The maximum value for the i8 type.
    static constexpr const signed char MAX_VALUE = 127;

    // The original value of the i8 type.
    signed char value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func i8() = default;
    // Constructor with a single signed char value.
    func i8(signed char p_value) : value(p_value) {}
    // Constructor with a signed char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func i8(signed char p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (signed char) of the i8 type.
    // If the i8 type is write-only or neutral, returns 0.
    func signed char get() const { return can_read() ? value : 0; }
    // Sets the value of the i8 type with the given signed char value.
    // If the i8 type is read-only or aggressive, the value will not be set.
    func void set(signed char p_value) { if (can_write()) value = p_value; }
    // Sets the value of the i8 type with the given i8 value.
    // If the i8 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the i8 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const i8 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the i8 type.
    // If the i8 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Adds two i8 values together.
    func i8 operator+(const i8 &p_other) const { return i8(get() + p_other.get()); }
    // Subtracts one i8 value from another.
    func i8 operator-(const i8 &p_other) const { return i8(get() - p_other.get()); }
    // Multiplies two i8 values together.
    func i8 operator*(const i8 &p_other) const { return i8(get() * p_other.get()); }
    // Divides one i8 value by another. (avoids division by zero)
    func i8 operator/(const i8 &p_other) const { return p_other.get() == 0 ? i8(MAX_VALUE) : i8(get() / p_other.get()); }
    // Modulo operation on two i8 values.
    func i8 operator%(const i8 &p_other) const { return i8(get() % p_other.get()); }

    // Adds a signed char value to an i8.
    func i8 operator+(signed char p_value) const { return i8(get() + p_value); }
    // Subtracts a signed char value from an i8.
    func i8 operator-(signed char p_value) const { return i8(get() - p_value); }
    // Multiplies an i8 value by a signed char.
    func i8 operator*(signed char p_value) const { return i8(get() * p_value); }
    // Divides an i8 value by a signed char. (avoids division by zero)
    func i8 operator/(signed char p_value) const { return p_value == 0 ? i8(MAX_VALUE) : i8(get() / p_value); }
    // Modulo operation on an i8 value by a signed char.
    func i8 operator%(signed char p_value) const { return i8(get() % p_value); }

    // Adds two i8 values together.
    func i8 operator+=(const i8 &p_other) { return i8(get() + p_other.get()); }
    // Subtracts one i8 value from another.
    func i8 operator-=(const i8 &p_other) { return i8(get() - p_other.get()); }
    // Multiplies two i8 values together.
    func i8 operator*=(const i8 &p_other) { return i8(get() * p_other.get()); }
    // Divides one i8 value by another. (avoids division by zero)
    func i8 operator/=(const i8 &p_other) { return p_other.get() == 0 ? i8(MAX_VALUE) : i8(get() / p_other.get()); }
    // Modulo operation on two i8 values.
    func i8 operator%=(const i8 &p_other) { return i8(get() % p_other.get()); }

    // Adds a signed char value to an i8.
    func i8 operator+=(signed char p_value) { return i8(get() + p_value); }
    // Subtracts a signed char value from an i8.
    func i8 operator-=(signed char p_value) { return i8(get() - p_value); }
    // Multiplies an i8 value by a signed char.
    func i8 operator*=(signed char p_value) { return i8(get() * p_value); }
    // Divides an i8 value by a signed char. (avoids division by zero)
    func i8 operator/=(signed char p_value) { return p_value == 0 ? i8(MAX_VALUE) : i8(get() / p_value); }
    // Modulo operation on an i8 value by a signed char.
    func i8 operator%=(signed char p_value) { return i8(get() % p_value); }

    // Negates an i8 value.
    func i8 operator-() const { return i8(-get()); }
    // Bitwise NOT operation on an i8 value.
    func i8 operator~() const { return i8(~get()); }
    // Left shift operation on an i8 value.
    func i8 operator<<(signed char p_value) const { return i8(get() << p_value); }
    // Right shift operation on an i8 value.
    func i8 operator>>(signed char p_value) const { return i8(get() >> p_value); }

    // Pre-increment operator.
    func i8 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : i8(0); }
    // Post-increment operator.
    func i8 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : i8(0); }
    // Pre-decrement operator.
    func i8 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : i8(0); }
    // Post-decrement operator.
    func i8 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : i8(0); }

    // Assignment operator with another i8 value.
    func void operator=(const i8 &p_other) { set(p_other); }
    // Assignment operator with a signed char value.
    func void operator=(signed char p_value) { set(p_value); }

    // Equality operator with another i8 value.
    func bool operator==(const i8 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another i8 value.
    func bool operator!=(const i8 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another i8 value.
    func bool operator<(const i8 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another i8 value.
    func bool operator>(const i8 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another i8 value.
    func bool operator<=(const i8 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another i8 value.
    func bool operator>=(const i8 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with a signed char value.
    func bool operator==(signed char p_other) const { return get() == p_other; }
    // Inequality operator with a signed char value.
    func bool operator!=(signed char p_other) const { return get() != p_other; }
    // Less than operator with a signed char value.
    func bool operator<(signed char p_other) const { return get() < p_other; }
    // Greater than operator with a signed char value.
    func bool operator>(signed char p_other) const { return get() > p_other; }
    // Less than or equal operator with a signed char value.
    func bool operator<=(signed char p_other) const { return get() <= p_other; }
    // Greater than or equal operator with a signed char value.
    func bool operator>=(signed char p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the i8.
    // Returns 0 if the value cannot be read.
    func i8 abs() const { return !can_read() ? i8(0) : (get() < 0 ? i8(-get()) : i8(get())); }

    // Returns the lower value between two i8 values.
    // Returns 0 if the value cannot be read.
    func i8 min(const i8 &p_other) const { return !can_read() ? i8(0) : (get() < p_other.get() ? i8(get()) : i8(p_other.get())); }
    // Returns the lower value between an i8 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i8 min(signed char p_value) const { return !can_read() ? i8(0) : (get() < p_value ? i8(get()) : i8(p_value)); }

    // Returns the higher value between two i8 values.
    // Returns 0 if the value cannot be read.
    func i8 max(const i8 &p_other) const { return !can_read() ? i8(0) : (get() > p_other.get() ? i8(get()) : i8(p_other.get())); }
    // Returns the higher value between an i8 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i8 max(signed char p_value) const { return !can_read() ? i8(0) : (get() > p_value ? i8(get()) : i8(p_value)); }

    // Returns the clamped value between two i8 values.
    // Returns 0 if the value cannot be read.
    func i8 clamp(const i8 &p_min, const i8 &p_max) const { return !can_read() ? i8(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an i8 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i8 clamp(signed char p_min, signed char p_max) const { return !can_read() ? i8(0) : (min(p_min).max(p_max)); }
};
