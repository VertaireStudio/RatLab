/************************************/
/*            i32.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The i32 type: It represents a 32-bit signed integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class i32 : public Variant {
    private:
    // The minimum value for the i32 type.
    static constexpr const signed int MIN_VALUE = -2147483648;
    // The maximum value for the i32 type.
    static constexpr const signed int MAX_VALUE = 2147483647;

    // The original value of the i32 type.
    signed int value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func i32() = default;
    // Constructor with a single signed char value.
    func i32(signed int p_value) : value(p_value) {}
    // Constructor with a signed char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func i32(signed int p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (signed int) of the i32 type.
    // If the i32 type is write-only or neutral, returns 0.
    func signed int get() const { return can_read() ? value : 0; }
    // Sets the value of the i32 type with the given signed int value.
    // If the i32 type is read-only or aggressive, the value will not be set.
    func void set(signed int p_value) { if (can_write()) value = p_value; }
    // Sets the value of the i32 type with the given i32 value.
    // If the i32 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the i32 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const i32 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the i32 type.
    // If the i32 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Adds two i32 values together.
    func i32 operator+(const i32 &p_other) const { return i32(get() + p_other.get()); }
    // Subtracts one i32 value from another.
    func i32 operator-(const i32 &p_other) const { return i32(get() - p_other.get()); }
    // Multiplies two i32 values together.
    func i32 operator*(const i32 &p_other) const { return i32(get() * p_other.get()); }
    // Divides one i32 value by another. (avoids division by zero)
    func i32 operator/(const i32 &p_other) const { return p_other.get() == 0 ? i32(MAX_VALUE) : i32(get() / p_other.get()); }
    // Modulo operation on two i32 values.
    func i32 operator%(const i32 &p_other) const { return i32(get() % p_other.get()); }

    // Adds a signed char value to an i32.
    func i32 operator+(signed char p_value) const { return i32(get() + p_value); }
    // Subtracts a signed char value from an i32.
    func i32 operator-(signed char p_value) const { return i32(get() - p_value); }
    // Multiplies an i32 value by a signed char.
    func i32 operator*(signed char p_value) const { return i32(get() * p_value); }
    // Divides an i32 value by a signed char. (avoids division by zero)
    func i32 operator/(signed char p_value) const { return p_value == 0 ? i32(MAX_VALUE) : i32(get() / p_value); }
    // Modulo operation on an i32 value by a signed char.
    func i32 operator%(signed char p_value) const { return i32(get() % p_value); }

    // Adds two i32 values together.
    func i32 operator+=(const i32 &p_other) { return i32(get() + p_other.get()); }
    // Subtracts one i32 value from another.
    func i32 operator-=(const i32 &p_other) { return i32(get() - p_other.get()); }
    // Multiplies two i32 values together.
    func i32 operator*=(const i32 &p_other) { return i32(get() * p_other.get()); }
    // Divides one i32 value by another. (avoids division by zero)
    func i32 operator/=(const i32 &p_other) { return p_other.get() == 0 ? i32(MAX_VALUE) : i32(get() / p_other.get()); }
    // Modulo operation on two i32 values.
    func i32 operator%=(const i32 &p_other) { return i32(get() % p_other.get()); }

    // Adds a signed char value to an i32.
    func i32 operator+=(signed char p_value) { return i32(get() + p_value); }
    // Subtracts a signed char value from an i32.
    func i32 operator-=(signed char p_value) { return i32(get() - p_value); }
    // Multiplies an i32 value by a signed char.
    func i32 operator*=(signed char p_value) { return i32(get() * p_value); }
    // Divides an i32 value by a signed char. (avoids division by zero)
    func i32 operator/=(signed char p_value) { return p_value == 0 ? i32(MAX_VALUE) : i32(get() / p_value); }
    // Modulo operation on an i32 value by a signed char.
    func i32 operator%=(signed char p_value) { return i32(get() % p_value); }

    // Negates an i32 value.
    func i32 operator-() const { return i32(-get()); }
    // Bitwise NOT operation on an i32 value.
    func i32 operator~() const { return i32(~get()); }
    // Left shift operation on an i32 value.
    func i32 operator<<(signed char p_value) const { return i32(get() << p_value); }
    // Right shift operation on an i32 value.
    func i32 operator>>(signed char p_value) const { return i32(get() >> p_value); }

    // Pre-increment operator.
    func i32 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : i32(0); }
    // Post-increment operator.
    func i32 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : i32(0); }
    // Pre-decrement operator.
    func i32 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : i32(0); }
    // Post-decrement operator.
    func i32 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : i32(0); }

    // Assignment operator with another i32 value.
    func void operator=(const i32 &p_other) { set(p_other); }
    // Assignment operator with a signed int value.
    func void operator=(signed int p_value) { set(p_value); }

    // Equality operator with another i32 value.
    func bool operator==(const i32 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another i32 value.
    func bool operator!=(const i32 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another i32 value.
    func bool operator<(const i32 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another i32 value.
    func bool operator>(const i32 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another i32 value.
    func bool operator<=(const i32 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another i32 value.
    func bool operator>=(const i32 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with a signed int value.
    func bool operator==(signed int p_other) const { return get() == p_other; }
    // Inequality operator with a signed int value.
    func bool operator!=(signed int p_other) const { return get() != p_other; }
    // Less than operator with a signed int value.
    func bool operator<(signed int p_other) const { return get() < p_other; }
    // Greater than operator with a signed int value.
    func bool operator>(signed int p_other) const { return get() > p_other; }
    // Less than or equal operator with a signed int value.
    func bool operator<=(signed int p_other) const { return get() <= p_other; }
    // Greater than or equal operator with a signed int value.
    func bool operator>=(signed int p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the i32.
    // Returns 0 if the value cannot be read.
    func i32 abs() const { return !can_read() ? i32(0) : (get() < 0 ? i32(-get()) : i32(get())); }

    // Returns the lower value between two i32 values.
    // Returns 0 if the value cannot be read.
    func i32 min(const i32 &p_other) const { return !can_read() ? i32(0) : (get() < p_other.get() ? i32(get()) : i32(p_other.get())); }
    // Returns the lower value between an i32 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i32 min(signed char p_value) const { return !can_read() ? i32(0) : (get() < p_value ? i32(get()) : i32(p_value)); }

    // Returns the higher value between two i32 values.
    // Returns 0 if the value cannot be read.
    func i32 max(const i32 &p_other) const { return !can_read() ? i32(0) : (get() > p_other.get() ? i32(get()) : i32(p_other.get())); }
    // Returns the higher value between an i32 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i32 max(signed char p_value) const { return !can_read() ? i32(0) : (get() > p_value ? i32(get()) : i32(p_value)); }

    // Returns the clamped value between two i32 values.
    // Returns 0 if the value cannot be read.
    func i32 clamp(const i32 &p_min, const i32 &p_max) const { return !can_read() ? i32(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an i32 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i32 clamp(signed char p_min, signed char p_max) const { return !can_read() ? i32(0) : (min(p_min).max(p_max)); }
};
