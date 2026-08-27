/************************************/
/*        type_configs.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "def_configs.hpp"

// unsigned char - u8
struct u8 {
    private:
    unsigned char data;
    bool read_only;

    public:
    // Constructors - Deconstructors

    func u8() : data(0), read_only(false) {}
    func u8(const unsigned char p_value) : data(p_value), read_only(false) {}
    func u8(const unsigned char p_value, const bool p_read_only) : data(p_value), read_only(p_read_only) {}

    // Operators

    func u8 operator+(const u8 p_value) const { return u8(data + p_value.data); }
    func u8 operator-(const u8 p_value) const { return u8(data - p_value.data); }
    func u8 operator*(const u8 p_value) const { return u8(data * p_value.data); }
    func u8 operator/(const u8 p_value) const { return u8(data / p_value.data); }
    func u8 operator%(const u8 p_value) const { return u8(data % p_value.data); }
    func u8 operator&(const u8 p_value) const { return u8(data & p_value.data); }
    func u8 operator|(const u8 p_value) const { return u8(data | p_value.data); }
    func u8 operator^(const u8 p_value) const { return u8(data ^ p_value.data); }
    func u8 operator<<(const u8 p_value) const { return u8(data << p_value.data); }
    func u8 operator>>(const u8 p_value) const { return u8(data >> p_value.data); }

    func u8 operator+(const unsigned char p_value) const { return u8(data + p_value); }
    func u8 operator-(const unsigned char p_value) const { return u8(data - p_value); }
    func u8 operator*(const unsigned char p_value) const { return u8(data * p_value); }
    func u8 operator/(const unsigned char p_value) const { return u8(data / p_value); }
    func u8 operator%(const unsigned char p_value) const { return u8(data % p_value); }
    func u8 operator&(const unsigned char p_value) const { return u8(data & p_value); }
    func u8 operator|(const unsigned char p_value) const { return u8(data | p_value); }
    func u8 operator^(const unsigned char p_value) const { return u8(data ^ p_value); }
    func u8 operator<<(const unsigned char p_value) const { return u8(data << p_value); }
    func u8 operator>>(const unsigned char p_value) const { return u8(data >> p_value); }

    func u8 operator-() const { return u8(-data); }
    func u8 operator~() const { return u8(~data); }

    func u8 operator--() { data -= 1; return *this; }
    func u8 operator++() { data += 1; return *this; }
    func u8 operator--(int) { ++*this; return *this; }
    func u8 operator++(int) { ++*this; return *this; }

    func void operator+=(const u8 p_value) { if (!read_only) data += p_value.data; }
    func void operator-=(const u8 p_value) { data -= p_value.data; }
    func void operator*=(const u8 p_value) { data *= p_value.data; }
    func void operator/=(const u8 p_value) { data /= p_value.data; }
    func void operator%=(const u8 p_value) { data %= p_value.data; }
    func void operator&=(const u8 p_value) { data &= p_value.data; }
    func void operator|=(const u8 p_value) { data |= p_value.data; }
    func void operator^=(const u8 p_value) { data ^= p_value.data; }
    func void operator<<=(const u8 p_value) { data <<= p_value.data; }
    func void operator>>=(const u8 p_value) { data >>= p_value.data; }

    func void operator+=(const unsigned char p_value) { data += p_value; }
    func void operator-=(const unsigned char p_value) { data -= p_value; }
    func void operator*=(const unsigned char p_value) { data *= p_value; }
    func void operator/=(const unsigned char p_value) { data /= p_value; }
    func void operator%=(const unsigned char p_value) { data %= p_value; }
    func void operator&=(const unsigned char p_value) { data &= p_value; }
    func void operator|=(const unsigned char p_value) { data |= p_value; }
    func void operator^=(const unsigned char p_value) { data ^= p_value; }
    func void operator<<=(const unsigned char p_value) { data <<= p_value; }
    func void operator>>=(const unsigned char p_value) { data >>= p_value; }

    func void operator=(const u8 p_value) { data = p_value.data; }
    func void operator=(const unsigned char p_value) { data = p_value; }

    func bool operator==(const u8 p_value) const { return data == p_value.data; }
    func bool operator!=(const u8 p_value) const { return data != p_value.data; }
    func bool operator>(const u8 p_value) const { return data > p_value.data; }
    func bool operator>=(const u8 p_value) const { return data >= p_value.data; }
    func bool operator<(const u8 p_value) const { return data < p_value.data; }
    func bool operator<=(const u8 p_value) const { return data <= p_value.data; }

    func bool operator==(const unsigned char p_value) const { return data == p_value; }
    func bool operator!=(const unsigned char p_value) const { return data != p_value; }
    func bool operator>(const unsigned char p_value) const { return data > p_value; }
    func bool operator>=(const unsigned char p_value) const { return data >= p_value; }
    func bool operator<(const unsigned char p_value) const { return data < p_value; }
    func bool operator<=(const unsigned char p_value) const { return data <= p_value; }

    func unsigned char get_value() const { return data; }
    func void set_value(const unsigned char p_value) { data = p_value; }
};

// unsigned short - u16
struct u16 {
    private:
    unsigned short data;

    public:
    // Constructors - Deconstructors

    func u16() : data(0) {}
    func u16(const unsigned short p_value) : data(p_value) {}

    // Operators

    func u16 operator+(const u16 p_value) const { return u16(data + p_value.data); }
    func u16 operator-(const u16 p_value) const { return u16(data - p_value.data); }
    func u16 operator*(const u16 p_value) const { return u16(data * p_value.data); }
    func u16 operator/(const u16 p_value) const { return u16(data / p_value.data); }
    func u16 operator%(const u16 p_value) const { return u16(data % p_value.data); }
    func u16 operator&(const u16 p_value) const { return u16(data & p_value.data); }
    func u16 operator|(const u16 p_value) const { return u16(data | p_value.data); }
    func u16 operator^(const u16 p_value) const { return u16(data ^ p_value.data); }
    func u16 operator<<(const u16 p_value) const { return u16(data << p_value.data); }
    func u16 operator>>(const u16 p_value) const { return u16(data >> p_value.data); }

    func u16 operator+(const unsigned short p_value) const { return u16(data + p_value); }
    func u16 operator-(const unsigned short p_value) const { return u16(data - p_value); }
    func u16 operator*(const unsigned short p_value) const { return u16(data * p_value); }
    func u16 operator/(const unsigned short p_value) const { return u16(data / p_value); }
    func u16 operator%(const unsigned short p_value) const { return u16(data % p_value); }
    func u16 operator&(const unsigned short p_value) const { return u16(data & p_value); }
    func u16 operator|(const unsigned short p_value) const { return u16(data | p_value); }
    func u16 operator^(const unsigned short p_value) const { return u16(data ^ p_value); }
    func u16 operator<<(const unsigned short p_value) const { return u16(data << p_value); }
    func u16 operator>>(const unsigned short p_value) const { return u16(data >> p_value); }

    func u16 operator-() const { return u16(-data); }
    func u16 operator~() const { return u16(~data); }

    func u16 operator--() { data -= 1; return *this; }
    func u16 operator++() { data += 1; return *this; }
    func u16 operator--(int) { ++*this; return *this; }
    func u16 operator++(int) { ++*this; return *this; }

    func void operator+=(const u16 p_value) { data += p_value.data; }
    func void operator-=(const u16 p_value) { data -= p_value.data; }
    func void operator*=(const u16 p_value) { data *= p_value.data; }
    func void operator/=(const u16 p_value) { data /= p_value.data; }
    func void operator%=(const u16 p_value) { data %= p_value.data; }
    func void operator&=(const u16 p_value) { data &= p_value.data; }
    func void operator|=(const u16 p_value) { data |= p_value.data; }
    func void operator^=(const u16 p_value) { data ^= p_value.data; }
    func void operator<<=(const u16 p_value) { data <<= p_value.data; }
    func void operator>>=(const u16 p_value) { data >>= p_value.data; }

    func void operator+=(const unsigned short p_value) { data += p_value; }
    func void operator-=(const unsigned short p_value) { data -= p_value; }
    func void operator*=(const unsigned short p_value) { data *= p_value; }
    func void operator/=(const unsigned short p_value) { data /= p_value; }
    func void operator%=(const unsigned short p_value) { data %= p_value; }
    func void operator&=(const unsigned short p_value) { data &= p_value; }
    func void operator|=(const unsigned short p_value) { data |= p_value; }
    func void operator^=(const unsigned short p_value) { data ^= p_value; }
    func void operator<<=(const unsigned short p_value) { data <<= p_value; }
    func void operator>>=(const unsigned short p_value) { data >>= p_value; }

    func void operator=(const u16 p_value) { data = p_value.data; }
    func void operator=(const unsigned short p_value) { data = p_value; }

    func bool operator==(const u16 p_value) const { return data == p_value.data; }
    func bool operator!=(const u16 p_value) const { return data != p_value.data; }
    func bool operator>(const u16 p_value) const { return data > p_value.data; }
    func bool operator>=(const u16 p_value) const { return data >= p_value.data; }
    func bool operator<(const u16 p_value) const { return data < p_value.data; }
    func bool operator<=(const u16 p_value) const { return data <= p_value.data; }

    func bool operator==(const unsigned short p_value) const { return data == p_value; }
    func bool operator!=(const unsigned short p_value) const { return data != p_value; }
    func bool operator>(const unsigned short p_value) const { return data > p_value; }
    func bool operator>=(const unsigned short p_value) const { return data >= p_value; }
    func bool operator<(const unsigned short p_value) const { return data < p_value; }
    func bool operator<=(const unsigned short p_value) const { return data <= p_value; }

    func unsigned short get_value() const { return data; }
    func void set_value(const unsigned short p_value) { data = p_value; }
};

// unsigned int - u32
struct u32 {
    private:
    unsigned int data;

    public:
    // Constructors - Deconstructors

    func u32() : data(0) {}
    func u32(const unsigned int p_value) : data(p_value) {}

    // Operators

    func u32 operator+(const u32 p_value) const { return u32(data + p_value.data); }
    func u32 operator-(const u32 p_value) const { return u32(data - p_value.data); }
    func u32 operator*(const u32 p_value) const { return u32(data * p_value.data); }
    func u32 operator/(const u32 p_value) const { return u32(data / p_value.data); }
    func u32 operator%(const u32 p_value) const { return u32(data % p_value.data); }
    func u32 operator&(const u32 p_value) const { return u32(data & p_value.data); }
    func u32 operator|(const u32 p_value) const { return u32(data | p_value.data); }
    func u32 operator^(const u32 p_value) const { return u32(data ^ p_value.data); }
    func u32 operator<<(const u32 p_value) const { return u32(data << p_value.data); }
    func u32 operator>>(const u32 p_value) const { return u32(data >> p_value.data); }

    func u32 operator+(const unsigned int p_value) const { return u32(data + p_value); }
    func u32 operator-(const unsigned int p_value) const { return u32(data - p_value); }
    func u32 operator*(const unsigned int p_value) const { return u32(data * p_value); }
    func u32 operator/(const unsigned int p_value) const { return u32(data / p_value); }
    func u32 operator%(const unsigned int p_value) const { return u32(data % p_value); }
    func u32 operator&(const unsigned int p_value) const { return u32(data & p_value); }
    func u32 operator|(const unsigned int p_value) const { return u32(data | p_value); }
    func u32 operator^(const unsigned int p_value) const { return u32(data ^ p_value); }
    func u32 operator<<(const unsigned int p_value) const { return u32(data << p_value); }
    func u32 operator>>(const unsigned int p_value) const { return u32(data >> p_value); }

    func u32 operator-() const { return u32(-data); }
    func u32 operator~() const { return u32(~data); }

    func u32 operator--() { data -= 1; return *this; }
    func u32 operator++() { data += 1; return *this; }
    func u32 operator--(int) { ++*this; return *this; }
    func u32 operator++(int) { ++*this; return *this; }

    func void operator+=(const u32 p_value) { data += p_value.data; }
    func void operator-=(const u32 p_value) { data -= p_value.data; }
    func void operator*=(const u32 p_value) { data *= p_value.data; }
    func void operator/=(const u32 p_value) { data /= p_value.data; }
    func void operator%=(const u32 p_value) { data %= p_value.data; }
    func void operator&=(const u32 p_value) { data &= p_value.data; }
    func void operator|=(const u32 p_value) { data |= p_value.data; }
    func void operator^=(const u32 p_value) { data ^= p_value.data; }
    func void operator<<=(const u32 p_value) { data <<= p_value.data; }
    func void operator>>=(const u32 p_value) { data >>= p_value.data; }

    func void operator+=(const unsigned int p_value) { data += p_value; }
    func void operator-=(const unsigned int p_value) { data -= p_value; }
    func void operator*=(const unsigned int p_value) { data *= p_value; }
    func void operator/=(const unsigned int p_value) { data /= p_value; }
    func void operator%=(const unsigned int p_value) { data %= p_value; }
    func void operator&=(const unsigned int p_value) { data &= p_value; }
    func void operator|=(const unsigned int p_value) { data |= p_value; }
    func void operator^=(const unsigned int p_value) { data ^= p_value; }
    func void operator<<=(const unsigned int p_value) { data <<= p_value; }
    func void operator>>=(const unsigned int p_value) { data >>= p_value; }

    func void operator=(const u32 p_value) { data = p_value.data; }
    func void operator=(const unsigned int p_value) { data = p_value; }

    func bool operator==(const u32 p_value) const { return data == p_value.data; }
    func bool operator!=(const u32 p_value) const { return data != p_value.data; }
    func bool operator>(const u32 p_value) const { return data > p_value.data; }
    func bool operator>=(const u32 p_value) const { return data >= p_value.data; }
    func bool operator<(const u32 p_value) const { return data < p_value.data; }
    func bool operator<=(const u32 p_value) const { return data <= p_value.data; }

    func bool operator==(const unsigned int p_value) const { return data == p_value; }
    func bool operator!=(const unsigned int p_value) const { return data != p_value; }
    func bool operator>(const unsigned int p_value) const { return data > p_value; }
    func bool operator>=(const unsigned int p_value) const { return data >= p_value; }
    func bool operator<(const unsigned int p_value) const { return data < p_value; }
    func bool operator<=(const unsigned int p_value) const { return data <= p_value; }

    func unsigned int get_value() const { return data; }
    func void set_value(const unsigned int p_value) { data = p_value; }
};

// unsigned long long - u64
struct u64 {
    private:
    unsigned long long data;

    public:
    // Constructors - Deconstructors

    func u64() : data(0) {}
    func u64(const unsigned long long p_value) : data(p_value) {}

    // Operators

    func u64 operator+(const u64 p_value) const { return u64(data + p_value.data); }
    func u64 operator-(const u64 p_value) const { return u64(data - p_value.data); }
    func u64 operator*(const u64 p_value) const { return u64(data * p_value.data); }
    func u64 operator/(const u64 p_value) const { return u64(data / p_value.data); }
    func u64 operator%(const u64 p_value) const { return u64(data % p_value.data); }
    func u64 operator&(const u64 p_value) const { return u64(data & p_value.data); }
    func u64 operator|(const u64 p_value) const { return u64(data | p_value.data); }
    func u64 operator^(const u64 p_value) const { return u64(data ^ p_value.data); }
    func u64 operator<<(const u64 p_value) const { return u64(data << p_value.data); }
    func u64 operator>>(const u64 p_value) const { return u64(data >> p_value.data); }

    func u64 operator+(const unsigned long long p_value) const { return u64(data + p_value); }
    func u64 operator-(const unsigned long long p_value) const { return u64(data - p_value); }
    func u64 operator*(const unsigned long long p_value) const { return u64(data * p_value); }
    func u64 operator/(const unsigned long long p_value) const { return u64(data / p_value); }
    func u64 operator%(const unsigned long long p_value) const { return u64(data % p_value); }
    func u64 operator&(const unsigned long long p_value) const { return u64(data & p_value); }
    func u64 operator|(const unsigned long long p_value) const { return u64(data | p_value); }
    func u64 operator^(const unsigned long long p_value) const { return u64(data ^ p_value); }
    func u64 operator<<(const unsigned long long p_value) const { return u64(data << p_value); }
    func u64 operator>>(const unsigned long long p_value) const { return u64(data >> p_value); }

    func u64 operator-() const { return u64(-data); }
    func u64 operator~() const { return u64(~data); }

    func u64 operator--() { data -= 1; return *this; }
    func u64 operator++() { data += 1; return *this; }
    func u64 operator--(int) { ++*this; return *this; }
    func u64 operator++(int) { ++*this; return *this; }

    func void operator+=(const u64 p_value) { data += p_value.data; }
    func void operator-=(const u64 p_value) { data -= p_value.data; }
    func void operator*=(const u64 p_value) { data *= p_value.data; }
    func void operator/=(const u64 p_value) { data /= p_value.data; }
    func void operator%=(const u64 p_value) { data %= p_value.data; }
    func void operator&=(const u64 p_value) { data &= p_value.data; }
    func void operator|=(const u64 p_value) { data |= p_value.data; }
    func void operator^=(const u64 p_value) { data ^= p_value.data; }
    func void operator<<=(const u64 p_value) { data <<= p_value.data; }
    func void operator>>=(const u64 p_value) { data >>= p_value.data; }

    func void operator+=(const unsigned long long p_value) { data += p_value; }
    func void operator-=(const unsigned long long p_value) { data -= p_value; }
    func void operator*=(const unsigned long long p_value) { data *= p_value; }
    func void operator/=(const unsigned long long p_value) { data /= p_value; }
    func void operator%=(const unsigned long long p_value) { data %= p_value; }
    func void operator&=(const unsigned long long p_value) { data &= p_value; }
    func void operator|=(const unsigned long long p_value) { data |= p_value; }
    func void operator^=(const unsigned long long p_value) { data ^= p_value; }
    func void operator<<=(const unsigned long long p_value) { data <<= p_value; }
    func void operator>>=(const unsigned long long p_value) { data >>= p_value; }

    func void operator=(const u64 p_value) { data = p_value.data; }
    func void operator=(const unsigned long long p_value) { data = p_value; }

    func bool operator==(const u64 p_value) const { return data == p_value.data; }
    func bool operator!=(const u64 p_value) const { return data != p_value.data; }
    func bool operator>(const u64 p_value) const { return data > p_value.data; }
    func bool operator>=(const u64 p_value) const { return data >= p_value.data; }
    func bool operator<(const u64 p_value) const { return data < p_value.data; }
    func bool operator<=(const u64 p_value) const { return data <= p_value.data; }

    func bool operator==(const unsigned long long p_value) const { return data == p_value; }
    func bool operator!=(const unsigned long long p_value) const { return data != p_value; }
    func bool operator>(const unsigned long long p_value) const { return data > p_value; }
    func bool operator>=(const unsigned long long p_value) const { return data >= p_value; }
    func bool operator<(const unsigned long long p_value) const { return data < p_value; }
    func bool operator<=(const unsigned long long p_value) const { return data <= p_value; }

    func unsigned long long get_value() const { return data; }
    func void set_value(const unsigned long long p_value) { data = p_value; }
};

// signed char - i8
struct i8 {
    private:
    signed char data;

    public:
    // Constructors - Deconstructors

    func i8() : data(0) {}
    func i8(const signed char p_value) : data(p_value) {}

    // Operators

    func i8 operator+(const i8 p_value) const { return i8(data + p_value.data); }
    func i8 operator-(const i8 p_value) const { return i8(data - p_value.data); }
    func i8 operator*(const i8 p_value) const { return i8(data * p_value.data); }
    func i8 operator/(const i8 p_value) const { return i8(data / p_value.data); }
    func i8 operator%(const i8 p_value) const { return i8(data % p_value.data); }
    func i8 operator&(const i8 p_value) const { return i8(data & p_value.data); }
    func i8 operator|(const i8 p_value) const { return i8(data | p_value.data); }
    func i8 operator^(const i8 p_value) const { return i8(data ^ p_value.data); }
    func i8 operator<<(const i8 p_value) const { return i8(data << p_value.data); }
    func i8 operator>>(const i8 p_value) const { return i8(data >> p_value.data); }

    func i8 operator+(const signed char p_value) const { return i8(data + p_value); }
    func i8 operator-(const signed char p_value) const { return i8(data - p_value); }
    func i8 operator*(const signed char p_value) const { return i8(data * p_value); }
    func i8 operator/(const signed char p_value) const { return i8(data / p_value); }
    func i8 operator%(const signed char p_value) const { return i8(data % p_value); }
    func i8 operator&(const signed char p_value) const { return i8(data & p_value); }
    func i8 operator|(const signed char p_value) const { return i8(data | p_value); }
    func i8 operator^(const signed char p_value) const { return i8(data ^ p_value); }
    func i8 operator<<(const signed char p_value) const { return i8(data << p_value); }
    func i8 operator>>(const signed char p_value) const { return i8(data >> p_value); }

    func i8 operator-() const { return i8(-data); }
    func i8 operator~() const { return i8(~data); }

    func i8 operator--() { data -= 1; return *this; }
    func i8 operator++() { data += 1; return *this; }
    func i8 operator--(int) { ++*this; return *this; }
    func i8 operator++(int) { ++*this; return *this; }

    func void operator+=(const i8 p_value) { data += p_value.data; }
    func void operator-=(const i8 p_value) { data -= p_value.data; }
    func void operator*=(const i8 p_value) { data *= p_value.data; }
    func void operator/=(const i8 p_value) { data /= p_value.data; }
    func void operator%=(const i8 p_value) { data %= p_value.data; }
    func void operator&=(const i8 p_value) { data &= p_value.data; }
    func void operator|=(const i8 p_value) { data |= p_value.data; }
    func void operator^=(const i8 p_value) { data ^= p_value.data; }
    func void operator<<=(const i8 p_value) { data <<= p_value.data; }
    func void operator>>=(const i8 p_value) { data >>= p_value.data; }

    func void operator+=(const signed char p_value) { data += p_value; }
    func void operator-=(const signed char p_value) { data -= p_value; }
    func void operator*=(const signed char p_value) { data *= p_value; }
    func void operator/=(const signed char p_value) { data /= p_value; }
    func void operator%=(const signed char p_value) { data %= p_value; }
    func void operator&=(const signed char p_value) { data &= p_value; }
    func void operator|=(const signed char p_value) { data |= p_value; }
    func void operator^=(const signed char p_value) { data ^= p_value; }
    func void operator<<=(const signed char p_value) { data <<= p_value; }
    func void operator>>=(const signed char p_value) { data >>= p_value; }

    func void operator=(const i8 p_value) { data = p_value.data; }
    func void operator=(const signed char p_value) { data = p_value; }

    func bool operator==(const i8 p_value) const { return data == p_value.data; }
    func bool operator!=(const i8 p_value) const { return data != p_value.data; }
    func bool operator>(const i8 p_value) const { return data > p_value.data; }
    func bool operator>=(const i8 p_value) const { return data >= p_value.data; }
    func bool operator<(const i8 p_value) const { return data < p_value.data; }
    func bool operator<=(const i8 p_value) const { return data <= p_value.data; }

    func bool operator==(const signed char p_value) const { return data == p_value; }
    func bool operator!=(const signed char p_value) const { return data != p_value; }
    func bool operator>(const signed char p_value) const { return data > p_value; }
    func bool operator>=(const signed char p_value) const { return data >= p_value; }
    func bool operator<(const signed char p_value) const { return data < p_value; }
    func bool operator<=(const signed char p_value) const { return data <= p_value; }

    func signed char get_value() const { return data; }
    func void set_value(const signed char p_value) { data = p_value; }
};

// signed short - i16
struct i16 {
    private:
    signed short data;

    public:
    // Constructors - Deconstructors

    func i16() : data(0) {}
    func i16(const signed short p_value) : data(p_value) {}

    // Operators

    func i16 operator+(const i16 p_value) const { return i16(data + p_value.data); }
    func i16 operator-(const i16 p_value) const { return i16(data - p_value.data); }
    func i16 operator*(const i16 p_value) const { return i16(data * p_value.data); }
    func i16 operator/(const i16 p_value) const { return i16(data / p_value.data); }
    func i16 operator%(const i16 p_value) const { return i16(data % p_value.data); }
    func i16 operator&(const i16 p_value) const { return i16(data & p_value.data); }
    func i16 operator|(const i16 p_value) const { return i16(data | p_value.data); }
    func i16 operator^(const i16 p_value) const { return i16(data ^ p_value.data); }
    func i16 operator<<(const i16 p_value) const { return i16(data << p_value.data); }
    func i16 operator>>(const i16 p_value) const { return i16(data >> p_value.data); }

    func i16 operator+(const signed short p_value) const { return i16(data + p_value); }
    func i16 operator-(const signed short p_value) const { return i16(data - p_value); }
    func i16 operator*(const signed short p_value) const { return i16(data * p_value); }
    func i16 operator/(const signed short p_value) const { return i16(data / p_value); }
    func i16 operator%(const signed short p_value) const { return i16(data % p_value); }
    func i16 operator&(const signed short p_value) const { return i16(data & p_value); }
    func i16 operator|(const signed short p_value) const { return i16(data | p_value); }
    func i16 operator^(const signed short p_value) const { return i16(data ^ p_value); }
    func i16 operator<<(const signed short p_value) const { return i16(data << p_value); }
    func i16 operator>>(const signed short p_value) const { return i16(data >> p_value); }

    func i16 operator-() const { return i16(-data); }
    func i16 operator~() const { return i16(~data); }

    func i16 operator--() { data -= 1; return *this; }
    func i16 operator++() { data += 1; return *this; }
    func i16 operator--(int) { ++*this; return *this; }
    func i16 operator++(int) { ++*this; return *this; }

    func void operator+=(const i16 p_value) { data += p_value.data; }
    func void operator-=(const i16 p_value) { data -= p_value.data; }
    func void operator*=(const i16 p_value) { data *= p_value.data; }
    func void operator/=(const i16 p_value) { data /= p_value.data; }
    func void operator%=(const i16 p_value) { data %= p_value.data; }
    func void operator&=(const i16 p_value) { data &= p_value.data; }
    func void operator|=(const i16 p_value) { data |= p_value.data; }
    func void operator^=(const i16 p_value) { data ^= p_value.data; }
    func void operator<<=(const i16 p_value) { data <<= p_value.data; }
    func void operator>>=(const i16 p_value) { data >>= p_value.data; }

    func void operator+=(const signed short p_value) { data += p_value; }
    func void operator-=(const signed short p_value) { data -= p_value; }
    func void operator*=(const signed short p_value) { data *= p_value; }
    func void operator/=(const signed short p_value) { data /= p_value; }
    func void operator%=(const signed short p_value) { data %= p_value; }
    func void operator&=(const signed short p_value) { data &= p_value; }
    func void operator|=(const signed short p_value) { data |= p_value; }
    func void operator^=(const signed short p_value) { data ^= p_value; }
    func void operator<<=(const signed short p_value) { data <<= p_value; }
    func void operator>>=(const signed short p_value) { data >>= p_value; }

    func void operator=(const i16 p_value) { data = p_value.data; }
    func void operator=(const signed short p_value) { data = p_value; }

    func bool operator==(const i16 p_value) const { return data == p_value.data; }
    func bool operator!=(const i16 p_value) const { return data != p_value.data; }
    func bool operator>(const i16 p_value) const { return data > p_value.data; }
    func bool operator>=(const i16 p_value) const { return data >= p_value.data; }
    func bool operator<(const i16 p_value) const { return data < p_value.data; }
    func bool operator<=(const i16 p_value) const { return data <= p_value.data; }

    func bool operator==(const signed short p_value) const { return data == p_value; }
    func bool operator!=(const signed short p_value) const { return data != p_value; }
    func bool operator>(const signed short p_value) const { return data > p_value; }
    func bool operator>=(const signed short p_value) const { return data >= p_value; }
    func bool operator<(const signed short p_value) const { return data < p_value; }
    func bool operator<=(const signed short p_value) const { return data <= p_value; }

    func signed short get_value() const { return data; }
    func void set_value(const signed short p_value) { data = p_value; }
};

// signed int - i32
struct i32 {
    private:
    signed int data;

    public:
    // Constructors - Deconstructors

    func i32() : data(0) {}
    func i32(const signed int p_value) : data(p_value) {}

    // Operators

    func i32 operator+(const i32 p_value) const { return i32(data + p_value.data); }
    func i32 operator-(const i32 p_value) const { return i32(data - p_value.data); }
    func i32 operator*(const i32 p_value) const { return i32(data * p_value.data); }
    func i32 operator/(const i32 p_value) const { return i32(data / p_value.data); }
    func i32 operator%(const i32 p_value) const { return i32(data % p_value.data); }
    func i32 operator&(const i32 p_value) const { return i32(data & p_value.data); }
    func i32 operator|(const i32 p_value) const { return i32(data | p_value.data); }
    func i32 operator^(const i32 p_value) const { return i32(data ^ p_value.data); }
    func i32 operator<<(const i32 p_value) const { return i32(data << p_value.data); }
    func i32 operator>>(const i32 p_value) const { return i32(data >> p_value.data); }

    func i32 operator+(const signed int p_value) const { return i32(data + p_value); }
    func i32 operator-(const signed int p_value) const { return i32(data - p_value); }
    func i32 operator*(const signed int p_value) const { return i32(data * p_value); }
    func i32 operator/(const signed int p_value) const { return i32(data / p_value); }
    func i32 operator%(const signed int p_value) const { return i32(data % p_value); }
    func i32 operator&(const signed int p_value) const { return i32(data & p_value); }
    func i32 operator|(const signed int p_value) const { return i32(data | p_value); }
    func i32 operator^(const signed int p_value) const { return i32(data ^ p_value); }
    func i32 operator<<(const signed int p_value) const { return i32(data << p_value); }
    func i32 operator>>(const signed int p_value) const { return i32(data >> p_value); }

    func i32 operator-() const { return i32(-data); }
    func i32 operator~() const { return i32(~data); }

    func i32 operator--() { data -= 1; return *this; }
    func i32 operator++() { data += 1; return *this; }
    func i32 operator--(int) { ++*this; return *this; }
    func i32 operator++(int) { ++*this; return *this; }

    func void operator+=(const i32 p_value) { data += p_value.data; }
    func void operator-=(const i32 p_value) { data -= p_value.data; }
    func void operator*=(const i32 p_value) { data *= p_value.data; }
    func void operator/=(const i32 p_value) { data /= p_value.data; }
    func void operator%=(const i32 p_value) { data %= p_value.data; }
    func void operator&=(const i32 p_value) { data &= p_value.data; }
    func void operator|=(const i32 p_value) { data |= p_value.data; }
    func void operator^=(const i32 p_value) { data ^= p_value.data; }
    func void operator<<=(const i32 p_value) { data <<= p_value.data; }
    func void operator>>=(const i32 p_value) { data >>= p_value.data; }

    func void operator+=(const signed int p_value) { data += p_value; }
    func void operator-=(const signed int p_value) { data -= p_value; }
    func void operator*=(const signed int p_value) { data *= p_value; }
    func void operator/=(const signed int p_value) { data /= p_value; }
    func void operator%=(const signed int p_value) { data %= p_value; }
    func void operator&=(const signed int p_value) { data &= p_value; }
    func void operator|=(const signed int p_value) { data |= p_value; }
    func void operator^=(const signed int p_value) { data ^= p_value; }
    func void operator<<=(const signed int p_value) { data <<= p_value; }
    func void operator>>=(const signed int p_value) { data >>= p_value; }

    func void operator=(const i32 p_value) { data = p_value.data; }
    func void operator=(const signed int p_value) { data = p_value; }

    func bool operator==(const i32 p_value) const { return data == p_value.data; }
    func bool operator!=(const i32 p_value) const { return data != p_value.data; }
    func bool operator>(const i32 p_value) const { return data > p_value.data; }
    func bool operator>=(const i32 p_value) const { return data >= p_value.data; }
    func bool operator<(const i32 p_value) const { return data < p_value.data; }
    func bool operator<=(const i32 p_value) const { return data <= p_value.data; }

    func bool operator==(const signed int p_value) const { return data == p_value; }
    func bool operator!=(const signed int p_value) const { return data != p_value; }
    func bool operator>(const signed int p_value) const { return data > p_value; }
    func bool operator>=(const signed int p_value) const { return data >= p_value; }
    func bool operator<(const signed int p_value) const { return data < p_value; }
    func bool operator<=(const signed int p_value) const { return data <= p_value; }

    func signed int get_value() const { return data; }
    func void set_value(const signed int p_value) { data = p_value; }
};

// signed long long - i64
struct i64 {
    private:
    signed long long data;

    public:
    // Constructors - Deconstructors

    func i64() : data(0) {}
    func i64(const signed long long p_value) : data(p_value) {}

    // Operators

    func i64 operator+(const i64 p_value) const { return i64(data + p_value.data); }
    func i64 operator-(const i64 p_value) const { return i64(data - p_value.data); }
    func i64 operator*(const i64 p_value) const { return i64(data * p_value.data); }
    func i64 operator/(const i64 p_value) const { return i64(data / p_value.data); }
    func i64 operator%(const i64 p_value) const { return i64(data % p_value.data); }
    func i64 operator&(const i64 p_value) const { return i64(data & p_value.data); }
    func i64 operator|(const i64 p_value) const { return i64(data | p_value.data); }
    func i64 operator^(const i64 p_value) const { return i64(data ^ p_value.data); }
    func i64 operator<<(const i64 p_value) const { return i64(data << p_value.data); }
    func i64 operator>>(const i64 p_value) const { return i64(data >> p_value.data); }

    func i64 operator+(const signed long long p_value) const { return i64(data + p_value); }
    func i64 operator-(const signed long long p_value) const { return i64(data - p_value); }
    func i64 operator*(const signed long long p_value) const { return i64(data * p_value); }
    func i64 operator/(const signed long long p_value) const { return i64(data / p_value); }
    func i64 operator%(const signed long long p_value) const { return i64(data % p_value); }
    func i64 operator&(const signed long long p_value) const { return i64(data & p_value); }
    func i64 operator|(const signed long long p_value) const { return i64(data | p_value); }
    func i64 operator^(const signed long long p_value) const { return i64(data ^ p_value); }
    func i64 operator<<(const signed long long p_value) const { return i64(data << p_value); }
    func i64 operator>>(const signed long long p_value) const { return i64(data >> p_value); }

    func i64 operator-() const { return i64(-data); }
    func i64 operator~() const { return i64(~data); }

    func i64 operator--() { data -= 1; return *this; }
    func i64 operator++() { data += 1; return *this; }
    func i64 operator--(int) { ++*this; return *this; }
    func i64 operator++(int) { ++*this; return *this; }

    func void operator+=(const i64 p_value) { data += p_value.data; }
    func void operator-=(const i64 p_value) { data -= p_value.data; }
    func void operator*=(const i64 p_value) { data *= p_value.data; }
    func void operator/=(const i64 p_value) { data /= p_value.data; }
    func void operator%=(const i64 p_value) { data %= p_value.data; }
    func void operator&=(const i64 p_value) { data &= p_value.data; }
    func void operator|=(const i64 p_value) { data |= p_value.data; }
    func void operator^=(const i64 p_value) { data ^= p_value.data; }
    func void operator<<=(const i64 p_value) { data <<= p_value.data; }
    func void operator>>=(const i64 p_value) { data >>= p_value.data; }

    func void operator+=(const signed long long p_value) { data += p_value; }
    func void operator-=(const signed long long p_value) { data -= p_value; }
    func void operator*=(const signed long long p_value) { data *= p_value; }
    func void operator/=(const signed long long p_value) { data /= p_value; }
    func void operator%=(const signed long long p_value) { data %= p_value; }
    func void operator&=(const signed long long p_value) { data &= p_value; }
    func void operator|=(const signed long long p_value) { data |= p_value; }
    func void operator^=(const signed long long p_value) { data ^= p_value; }
    func void operator<<=(const signed long long p_value) { data <<= p_value; }
    func void operator>>=(const signed long long p_value) { data >>= p_value; }

    func void operator=(const i64 p_value) { data = p_value.data; }
    func void operator=(const signed long long p_value) { data = p_value; }

    func bool operator==(const i64 p_value) const { return data == p_value.data; }
    func bool operator!=(const i64 p_value) const { return data != p_value.data; }
    func bool operator>(const i64 p_value) const { return data > p_value.data; }
    func bool operator>=(const i64 p_value) const { return data >= p_value.data; }
    func bool operator<(const i64 p_value) const { return data < p_value.data; }
    func bool operator<=(const i64 p_value) const { return data <= p_value.data; }

    func bool operator==(const signed long long p_value) const { return data == p_value; }
    func bool operator!=(const signed long long p_value) const { return data != p_value; }
    func bool operator>(const signed long long p_value) const { return data > p_value; }
    func bool operator>=(const signed long long p_value) const { return data >= p_value; }
    func bool operator<(const signed long long p_value) const { return data < p_value; }
    func bool operator<=(const signed long long p_value) const { return data <= p_value; }

    func signed long long get_value() const { return data; }
    func void set_value(const signed long long p_value) { data = p_value; }
};

// float - f32
struct f32 {
    private:
    float data;

    public:
    // Constructors - Deconstructors

    func f32() : data(0.0f) {}
    func f32(const float p_value) : data(p_value) {}

    // Operators

    func f32 operator+(const f32 p_value) const { return f32(data + p_value.data); }
    func f32 operator-(const f32 p_value) const { return f32(data - p_value.data); }
    func f32 operator*(const f32 p_value) const { return f32(data * p_value.data); }
    func f32 operator/(const f32 p_value) const { return f32(data / p_value.data); }

    func f32 operator+(const float p_value) const { return f32(data + p_value); }
    func f32 operator-(const float p_value) const { return f32(data - p_value); }
    func f32 operator*(const float p_value) const { return f32(data * p_value); }
    func f32 operator/(const float p_value) const { return f32(data / p_value); }

    func f32 operator-() const { return f32(-data); }

    func f32 operator--() const { return f32(data - 1.0f); }
    func f32 operator++() const { return f32(data + 1.0f); }
    func f32 operator--(int) { f32 old = *this; data -= 1.0f; return old; }
    func f32 operator++(int) { f32 old = *this; data += 1.0f; return old; }

    func void operator+=(const f32 p_value) { data += p_value.data; }
    func void operator-=(const f32 p_value) { data -= p_value.data; }
    func void operator*=(const f32 p_value) { data *= p_value.data; }
    func void operator/=(const f32 p_value) { data /= p_value.data; }

    func void operator+=(const float p_value) { data += p_value; }
    func void operator-=(const float p_value) { data -= p_value; }
    func void operator*=(const float p_value) { data *= p_value; }
    func void operator/=(const float p_value) { data /= p_value; }

    func void operator=(const f32 p_value) { data = p_value.data; }
    func void operator=(const float p_value) { data = p_value; }

    func bool operator==(const f32 p_value) const { return data == p_value.data; }
    func bool operator!=(const f32 p_value) const { return data != p_value.data; }
    func bool operator>(const f32 p_value) const { return data > p_value.data; }
    func bool operator>=(const f32 p_value) const { return data >= p_value.data; }
    func bool operator<(const f32 p_value) const { return data < p_value.data; }
    func bool operator<=(const f32 p_value) const { return data <= p_value.data; }

    func bool operator==(const float p_value) const { return data == p_value; }
    func bool operator!=(const float p_value) const { return data != p_value; }
    func bool operator>(const float p_value) const { return data > p_value; }
    func bool operator>=(const float p_value) const { return data >= p_value; }
    func bool operator<(const float p_value) const { return data < p_value; }
    func bool operator<=(const float p_value) const { return data <= p_value; }

    func float get_value() const { return data; }
    func void set_value(const float p_value) { data = p_value; }
};

// double - f64
struct f64 {
    private:
    double data;

    public:
    // Constructors - Deconstructors

    func f64() : data(0.0) {}
    func f64(const double p_value) : data(p_value) {}

    // Operators

    func f64 operator+(const f64 p_value) const { return f64(data + p_value.data); }
    func f64 operator-(const f64 p_value) const { return f64(data - p_value.data); }
    func f64 operator*(const f64 p_value) const { return f64(data * p_value.data); }
    func f64 operator/(const f64 p_value) const { return f64(data / p_value.data); }

    func f64 operator+(const double p_value) const { return f64(data + p_value); }
    func f64 operator-(const double p_value) const { return f64(data - p_value); }
    func f64 operator*(const double p_value) const { return f64(data * p_value); }
    func f64 operator/(const double p_value) const { return f64(data / p_value); }

    func f64 operator-() const { return f64(-data); }

    func f64 operator--() const { return f64(data - 1.0); }
    func f64 operator++() const { return f64(data + 1.0); }
    func f64 operator--(int) { f64 old = *this; data -= 1.0; return old; }
    func f64 operator++(int) { f64 old = *this; data += 1.0; return old; }

    func void operator+=(const f64 p_value) { data += p_value.data; }
    func void operator-=(const f64 p_value) { data -= p_value.data; }
    func void operator*=(const f64 p_value) { data *= p_value.data; }
    func void operator/=(const f64 p_value) { data /= p_value.data; }

    func void operator+=(const double p_value) { data += p_value; }
    func void operator-=(const double p_value) { data -= p_value; }
    func void operator*=(const double p_value) { data *= p_value; }
    func void operator/=(const double p_value) { data /= p_value; }

    func void operator=(const f64 p_value) { data = p_value.data; }
    func void operator=(const double p_value) { data = p_value; }

    func bool operator==(const f64 p_value) const { return data == p_value.data; }
    func bool operator!=(const f64 p_value) const { return data != p_value.data; }
    func bool operator>(const f64 p_value) const { return data > p_value.data; }
    func bool operator>=(const f64 p_value) const { return data >= p_value.data; }
    func bool operator<(const f64 p_value) const { return data < p_value.data; }
    func bool operator<=(const f64 p_value) const { return data <= p_value.data; }

    func bool operator==(const double p_value) const { return data == p_value; }
    func bool operator!=(const double p_value) const { return data != p_value; }
    func bool operator>(const double p_value) const { return data > p_value; }
    func bool operator>=(const double p_value) const { return data >= p_value; }
    func bool operator<(const double p_value) const { return data < p_value; }
    func bool operator<=(const double p_value) const { return data <= p_value; }

    func double get_value() const { return data; }
    func void set_value(const double p_value) { data = p_value; }
};

// Conversions between custom types

static func u8 to_u8(const u16 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const u32 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const u64 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const i8 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const i16 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const i32 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const i64 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const f32 p_value) { return u8((unsigned char)p_value.get_value()); }
static func u8 to_u8(const f64 p_value) { return u8((unsigned char)p_value.get_value()); }

static func u16 to_u16(const u8 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const u32 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const u64 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const i8 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const i16 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const i32 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const i64 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const f32 p_value) { return u16((unsigned short)p_value.get_value()); }
static func u16 to_u16(const f64 p_value) { return u16((unsigned short)p_value.get_value()); }

static func u32 to_u32(const u8 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const u16 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const u64 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const i8 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const i16 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const i32 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const i64 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const f32 p_value) { return u32((unsigned int)p_value.get_value()); }
static func u32 to_u32(const f64 p_value) { return u32((unsigned int)p_value.get_value()); }

static func u64 to_u64(const u8 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const u16 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const u32 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const i8 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const i16 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const i32 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const i64 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const f32 p_value) { return u64((unsigned long long)p_value.get_value()); }
static func u64 to_u64(const f64 p_value) { return u64((unsigned long long)p_value.get_value()); }

static func i8 to_i8(const u8 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const u16 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const u32 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const u64 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const i16 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const i32 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const i64 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const f32 p_value) { return i8((signed char)p_value.get_value()); }
static func i8 to_i8(const f64 p_value) { return i8((signed char)p_value.get_value()); }

static func i16 to_i16(const u8 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const u16 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const u32 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const u64 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const i8 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const i32 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const i64 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const f32 p_value) { return i16((signed short)p_value.get_value()); }
static func i16 to_i16(const f64 p_value) { return i16((signed short)p_value.get_value()); }

static func i32 to_i32(const u8 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const u16 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const u32 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const u64 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const i8 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const i16 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const i64 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const f32 p_value) { return i32((signed int)p_value.get_value()); }
static func i32 to_i32(const f64 p_value) { return i32((signed int)p_value.get_value()); }

static func i64 to_i64(const u8 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const u16 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const u32 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const u64 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const i8 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const i16 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const i32 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const f32 p_value) { return i64((signed long long)p_value.get_value()); }
static func i64 to_i64(const f64 p_value) { return i64((signed long long)p_value.get_value()); }

static func f32 to_f32(const u8 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const u16 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const u32 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const u64 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const i8 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const i16 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const i32 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const i64 p_value) { return f32((float)p_value.get_value()); }
static func f32 to_f32(const f64 p_value) { return f32((float)p_value.get_value()); }

static func f64 to_f64(const u8 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const u16 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const u32 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const u64 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const i8 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const i16 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const i32 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const i64 p_value) { return f64((double)p_value.get_value()); }
static func f64 to_f64(const f32 p_value) { return f64((double)p_value.get_value()); }


static constexpr const u8 U8_MAX = 0xff;
static constexpr const u16 U16_MAX = 0xffff;
static constexpr const u32 U32_MAX = 0xffffffff;
static constexpr const u64 U64_MAX = 0xffffffffffffffff;

static constexpr const i8 I8_MIN = 0x80;
static constexpr const i8 I8_MAX = 0x7f;
static constexpr const i16 I16_MIN = 0x8000;
static constexpr const i16 I16_MAX = 0x7fff;
static constexpr const i32 I32_MIN = 0x80000000;
static constexpr const i32 I32_MAX = 0x7fffffff;
static constexpr const i64 I64_MIN = 0x8000000000000000;
static constexpr const i64 I64_MAX = 0x7fffffffffffffff;

// Floats and Doubles cannot be represented with binary values

static constexpr const f32 F32_MIN = 1.175494e-38;
static constexpr const f32 F32_MAX = 3.402823e+38;
static constexpr const f64 F64_MIN = 2.225074e-308;
static constexpr const f64 F64_MAX = 1.797693e+308;


