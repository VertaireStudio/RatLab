/************************************/
/*            i16.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The i16 type: It represents a 16-bit signed integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class i16 : public Variant {
    private:
    // The minimum value for the i16 type.
    static constexpr const signed short MIN_VALUE = -32768;
    // The maximum value for the i16 type.
    static constexpr const signed short MAX_VALUE = 32767;

    // The original value of the i16 type.
    signed short value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func i16() = default;
    // Constructor with a single signed char value.
    func i16(signed short p_value) : value(p_value) {}
    // Constructor with a signed char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func i16(signed short p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (signed short) of the i16 type.
    // If the i16 type is write-only or neutral, returns 0.
    func signed short get() const { return can_read() ? value : 0; }
    // Sets the value of the i16 type with the given signed short value.
    // If the i16 type is read-only or aggressive, the value will not be set.
    func void set(signed short p_value) { if (can_write()) value = p_value; }
    // Sets the value of the i16 type with the given i16 value.
    // If the i16 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the i16 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const i16 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the i16 type.
    // If the i16 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Adds two i16 values together.
    func i16 operator+(const i16 &p_other) const { return i16(get() + p_other.get()); }
    // Subtracts one i16 value from another.
    func i16 operator-(const i16 &p_other) const { return i16(get() - p_other.get()); }
    // Multiplies two i16 values together.
    func i16 operator*(const i16 &p_other) const { return i16(get() * p_other.get()); }
    // Divides one i16 value by another. (avoids division by zero)
    func i16 operator/(const i16 &p_other) const { return p_other.get() == 0 ? i16(MAX_VALUE) : i16(get() / p_other.get()); }
    // Modulo operation on two i16 values.
    func i16 operator%(const i16 &p_other) const { return i16(get() % p_other.get()); }

    // Adds a signed char value to an i16.
    func i16 operator+(signed char p_value) const { return i16(get() + p_value); }
    // Subtracts a signed char value from an i16.
    func i16 operator-(signed char p_value) const { return i16(get() - p_value); }
    // Multiplies an i16 value by a signed char.
    func i16 operator*(signed char p_value) const { return i16(get() * p_value); }
    // Divides an i16 value by a signed char. (avoids division by zero)
    func i16 operator/(signed char p_value) const { return p_value == 0 ? i16(MAX_VALUE) : i16(get() / p_value); }
    // Modulo operation on an i16 value by a signed char.
    func i16 operator%(signed char p_value) const { return i16(get() % p_value); }

    // Adds two i16 values together.
    func i16 operator+=(const i16 &p_other) { return i16(get() + p_other.get()); }
    // Subtracts one i16 value from another.
    func i16 operator-=(const i16 &p_other) { return i16(get() - p_other.get()); }
    // Multiplies two i16 values together.
    func i16 operator*=(const i16 &p_other) { return i16(get() * p_other.get()); }
    // Divides one i16 value by another. (avoids division by zero)
    func i16 operator/=(const i16 &p_other) { return p_other.get() == 0 ? i16(MAX_VALUE) : i16(get() / p_other.get()); }
    // Modulo operation on two i16 values.
    func i16 operator%=(const i16 &p_other) { return i16(get() % p_other.get()); }

    // Adds a signed char value to an i16.
    func i16 operator+=(signed char p_value) { return i16(get() + p_value); }
    // Subtracts a signed char value from an i16.
    func i16 operator-=(signed char p_value) { return i16(get() - p_value); }
    // Multiplies an i16 value by a signed char.
    func i16 operator*=(signed char p_value) { return i16(get() * p_value); }
    // Divides an i16 value by a signed char. (avoids division by zero)
    func i16 operator/=(signed char p_value) { return p_value == 0 ? i16(MAX_VALUE) : i16(get() / p_value); }
    // Modulo operation on an i16 value by a signed char.
    func i16 operator%=(signed char p_value) { return i16(get() % p_value); }

    // Negates an i16 value.
    func i16 operator-() const { return i16(-get()); }
    // Bitwise NOT operation on an i16 value.
    func i16 operator~() const { return i16(~get()); }
    // Left shift operation on an i16 value.
    func i16 operator<<(signed char p_value) const { return i16(get() << p_value); }
    // Right shift operation on an i16 value.
    func i16 operator>>(signed char p_value) const { return i16(get() >> p_value); }

    // Pre-increment operator.
    func i16 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : i16(0); }
    // Post-increment operator.
    func i16 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : i16(0); }
    // Pre-decrement operator.
    func i16 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : i16(0); }
    // Post-decrement operator.
    func i16 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : i16(0); }

    // Assignment operator with another i16 value.
    func void operator=(const i16 &p_other) { set(p_other); }
    // Assignment operator with a signed short value.
    func void operator=(signed short p_value) { set(p_value); }

    // Equality operator with another i16 value.
    func bool operator==(const i16 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another i16 value.
    func bool operator!=(const i16 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another i16 value.
    func bool operator<(const i16 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another i16 value.
    func bool operator>(const i16 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another i16 value.
    func bool operator<=(const i16 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another i16 value.
    func bool operator>=(const i16 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with a signed short value.
    func bool operator==(signed short p_other) const { return get() == p_other; }
    // Inequality operator with a signed short value.
    func bool operator!=(signed short p_other) const { return get() != p_other; }
    // Less than operator with a signed short value.
    func bool operator<(signed short p_other) const { return get() < p_other; }
    // Greater than operator with a signed short value.
    func bool operator>(signed short p_other) const { return get() > p_other; }
    // Less than or equal operator with a signed short value.
    func bool operator<=(signed short p_other) const { return get() <= p_other; }
    // Greater than or equal operator with a signed short value.
    func bool operator>=(signed short p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the i16.
    // Returns 0 if the value cannot be read.
    func i16 abs() const { return !can_read() ? i16(0) : (get() < 0 ? i16(-get()) : i16(get())); }

    // Returns the lower value between two i16 values.
    // Returns 0 if the value cannot be read.
    func i16 min(const i16 &p_other) const { return !can_read() ? i16(0) : (get() < p_other.get() ? i16(get()) : i16(p_other.get())); }
    // Returns the lower value between an i16 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i16 min(signed char p_value) const { return !can_read() ? i16(0) : (get() < p_value ? i16(get()) : i16(p_value)); }

    // Returns the higher value between two i16 values.
    // Returns 0 if the value cannot be read.
    func i16 max(const i16 &p_other) const { return !can_read() ? i16(0) : (get() > p_other.get() ? i16(get()) : i16(p_other.get())); }
    // Returns the higher value between an i16 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i16 max(signed char p_value) const { return !can_read() ? i16(0) : (get() > p_value ? i16(get()) : i16(p_value)); }

    // Returns the clamped value between two i16 values.
    // Returns 0 if the value cannot be read.
    func i16 clamp(const i16 &p_min, const i16 &p_max) const { return !can_read() ? i16(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an i16 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i16 clamp(signed char p_min, signed char p_max) const { return !can_read() ? i16(0) : (min(p_min).max(p_max)); }
};
