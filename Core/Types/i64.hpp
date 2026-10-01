/************************************/
/*            i64.hpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"

// The i64 type: It represents a 64-bit signed integer with a Variant extension and unique wrapper methods.
// Can be converted to any of the supported types that extend from the Variant type.
class i64 : public Variant {
    private:
    // The minimum value for the i64 type.
    static constexpr const signed long long MIN_VALUE = -9223372036854775807;
    // The maximum value for the i64 type.
    static constexpr const signed long long MAX_VALUE = 9223372036854775807;

    // The original value of the i64 type.
    signed long long value = 0;
    /*-----------------------------------*/

    public:
    // Default constructor.
    func i64() = default;
    // Constructor with a single signed char value.
    func i64(signed long long p_value) : value(p_value) {}
    // Constructor with a signed char value and read-only flag.
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    func i64(signed long long p_value, Accessability p_access) : value(p_value) { access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns the original value (signed long long) of the i64 type.
    // If the i64 type is write-only or neutral, returns 0.
    func signed long long get() const { return can_read() ? value : 0; }
    // Sets the value of the i64 type with the given signed long long value.
    // If the i64 type is read-only or aggressive, the value will not be set.
    func void set(signed long long p_value) { if (can_write()) value = p_value; }
    // Sets the value of the i64 type with the given i64 value.
    // If the i64 type (which is being set from) is read-only or aggressive, the value will not be set.
    // If the i64 type (from the parameter) is write-only or neutral, the value will not be set.
    func void set(const i64 &p_other) { if (can_write() && p_other.can_read()) value = p_other.get(); }
    // Sets the accessibility of the i64 type.
    // If the i64 type is aggressive, neutral, or passive: the access will NOT be overriden, until freed from memory.
    func void set_access(Accessability p_access) { if (can_overwrite_access()) access = p_access; }
    /*--------------------------------------------------------------------------------------------------------------*/

    // Adds two i64 values together.
    func i64 operator+(const i64 &p_other) const { return i64(get() + p_other.get()); }
    // Subtracts one i64 value from another.
    func i64 operator-(const i64 &p_other) const { return i64(get() - p_other.get()); }
    // Multiplies two i64 values together.
    func i64 operator*(const i64 &p_other) const { return i64(get() * p_other.get()); }
    // Divides one i64 value by another. (avoids division by zero)
    func i64 operator/(const i64 &p_other) const { return p_other.get() == 0 ? i64(MAX_VALUE) : i64(get() / p_other.get()); }
    // Modulo operation on two i64 values.
    func i64 operator%(const i64 &p_other) const { return i64(get() % p_other.get()); }

    // Adds a signed char value to an i64.
    func i64 operator+(signed char p_value) const { return i64(get() + p_value); }
    // Subtracts a signed char value from an i64.
    func i64 operator-(signed char p_value) const { return i64(get() - p_value); }
    // Multiplies an i64 value by a signed char.
    func i64 operator*(signed char p_value) const { return i64(get() * p_value); }
    // Divides an i64 value by a signed char. (avoids division by zero)
    func i64 operator/(signed char p_value) const { return p_value == 0 ? i64(MAX_VALUE) : i64(get() / p_value); }
    // Modulo operation on an i64 value by a signed char.
    func i64 operator%(signed char p_value) const { return i64(get() % p_value); }

    // Adds two i64 values together.
    func i64 operator+=(const i64 &p_other) { return i64(get() + p_other.get()); }
    // Subtracts one i64 value from another.
    func i64 operator-=(const i64 &p_other) { return i64(get() - p_other.get()); }
    // Multiplies two i64 values together.
    func i64 operator*=(const i64 &p_other) { return i64(get() * p_other.get()); }
    // Divides one i64 value by another. (avoids division by zero)
    func i64 operator/=(const i64 &p_other) { return p_other.get() == 0 ? i64(MAX_VALUE) : i64(get() / p_other.get()); }
    // Modulo operation on two i64 values.
    func i64 operator%=(const i64 &p_other) { return i64(get() % p_other.get()); }

    // Adds a signed char value to an i64.
    func i64 operator+=(signed char p_value) { return i64(get() + p_value); }
    // Subtracts a signed char value from an i64.
    func i64 operator-=(signed char p_value) { return i64(get() - p_value); }
    // Multiplies an i64 value by a signed char.
    func i64 operator*=(signed char p_value) { return i64(get() * p_value); }
    // Divides an i64 value by a signed char. (avoids division by zero)
    func i64 operator/=(signed char p_value) { return p_value == 0 ? i64(MAX_VALUE) : i64(get() / p_value); }
    // Modulo operation on an i64 value by a signed char.
    func i64 operator%=(signed char p_value) { return i64(get() % p_value); }

    // Negates an i64 value.
    func i64 operator-() const { return i64(-get()); }
    // Bitwise NOT operation on an i64 value.
    func i64 operator~() const { return i64(~get()); }
    // Left shift operation on an i64 value.
    func i64 operator<<(signed char p_value) const { return i64(get() << p_value); }
    // Right shift operation on an i64 value.
    func i64 operator>>(signed char p_value) const { return i64(get() >> p_value); }

    // Pre-increment operator.
    func i64 operator++() { if (can_write()) set(get() + 1); return can_read() ? *this : i64(0); }
    // Post-increment operator.
    func i64 operator++(int) { if (can_write()) set(get() + 1); return can_read() ? *this : i64(0); }
    // Pre-decrement operator.
    func i64 operator--() { if (can_write()) set(get() - 1); return can_read() ? *this : i64(0); }
    // Post-decrement operator.
    func i64 operator--(int) { if (can_write()) set(get() - 1); return can_read() ? *this : i64(0); }

    // Assignment operator with another i64 value.
    func void operator=(const i64 &p_other) { set(p_other); }
    // Assignment operator with a signed long long value.
    func void operator=(signed long long p_value) { set(p_value); }

    // Equality operator with another i64 value.
    func bool operator==(const i64 &p_other) const { return get() == p_other.get(); }
    // Inequality operator with another i64 value.
    func bool operator!=(const i64 &p_other) const { return get() != p_other.get(); }
    // Less than operator with another i64 value.
    func bool operator<(const i64 &p_other) const { return get() < p_other.get(); }
    // Greater than operator with another i64 value.
    func bool operator>(const i64 &p_other) const { return get() > p_other.get(); }
    // Less than or equal operator with another i64 value.
    func bool operator<=(const i64 &p_other) const { return get() <= p_other.get(); }
    // Greater than or equal operator with another i64 value.
    func bool operator>=(const i64 &p_other) const { return get() >= p_other.get(); }

    // Equality operator with a signed long long value.
    func bool operator==(signed long long p_other) const { return get() == p_other; }
    // Inequality operator with a signed long long value.
    func bool operator!=(signed long long p_other) const { return get() != p_other; }
    // Less than operator with a signed long long value.
    func bool operator<(signed long long p_other) const { return get() < p_other; }
    // Greater than operator with a signed long long value.
    func bool operator>(signed long long p_other) const { return get() > p_other; }
    // Less than or equal operator with a signed long long value.
    func bool operator<=(signed long long p_other) const { return get() <= p_other; }
    // Greater than or equal operator with a signed long long value.
    func bool operator>=(signed long long p_other) const { return get() >= p_other; }
    /*-------------------------------------------------------------------------------*/

    // Returns the absolute value of the i64.
    // Returns 0 if the value cannot be read.
    func i64 abs() const { return !can_read() ? i64(0) : (get() < 0 ? i64(-get()) : i64(get())); }

    // Returns the lower value between two i64 values.
    // Returns 0 if the value cannot be read.
    func i64 min(const i64 &p_other) const { return !can_read() ? i64(0) : (get() < p_other.get() ? i64(get()) : i64(p_other.get())); }
    // Returns the lower value between an i64 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i64 min(signed char p_value) const { return !can_read() ? i64(0) : (get() < p_value ? i64(get()) : i64(p_value)); }

    // Returns the higher value between two i64 values.
    // Returns 0 if the value cannot be read.
    func i64 max(const i64 &p_other) const { return !can_read() ? i64(0) : (get() > p_other.get() ? i64(get()) : i64(p_other.get())); }
    // Returns the higher value between an i64 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i64 max(signed char p_value) const { return !can_read() ? i64(0) : (get() > p_value ? i64(get()) : i64(p_value)); }

    // Returns the clamped value between two i64 values.
    // Returns 0 if the value cannot be read.
    func i64 clamp(const i64 &p_min, const i64 &p_max) const { return !can_read() ? i64(0) : (min(p_min).max(p_max)); }
    // Returns the clamped value between an i64 value and a signed char.
    // Returns 0 if the value cannot be read.
    func i64 clamp(signed char p_min, signed char p_max) const { return !can_read() ? i64(0) : (min(p_min).max(p_max)); }
};
