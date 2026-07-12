/************************************/
/*           variant.hpp            */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "array.hpp"
#include "bool.hpp"
#include "f32.hpp"
#include "f64.hpp"
#include "i8.hpp"
#include "i16.hpp"
#include "i32.hpp"
#include "i64.hpp"
#include "packed_array.hpp"
#include "string.hpp"
#include "u8.hpp"
#include "u16.hpp"
#include "u32.hpp"
#include "u64.hpp"
#include "vector.hpp"

struct Variant {
    private:
    // Data
    Bool value_bool;
    u8 value_u8;
    u16 value_u16;
    u32 value_u32;
    u64 value_u64;
    i8 value_i8;
    i16 value_i16;
    i32 value_i32;
    i64 value_i64;
    f32 value_f32;
    f64 value_f64;

    TYPES current_type;
    bool no_type_specified = false;

    public:
    // Constructor
    // Default: f32 type

    func Variant() : value_f32(0.0f), current_type(TYPES::F32), no_type_specified(true) {}
    
    func Variant(Bool p_value)  : value_bool(false), current_type(TYPES::BOOL) {}
    func Variant(u8 p_value)    : value_u8(0), current_type(TYPES::U8) {}
    func Variant(u16 p_value)   : value_u16(0), current_type(TYPES::U16) {}
    func Variant(u32 p_value)   : value_u32(0), current_type(TYPES::U32) {}
    func Variant(u64 p_value)   : value_u64(0), current_type(TYPES::U64) {}
    func Variant(i8 p_value)    : value_i8(0), current_type(TYPES::I8) {}
    func Variant(i16 p_value)   : value_i16(0), current_type(TYPES::I16) {}
    func Variant(i32 p_value)   : value_i32(0), current_type(TYPES::I32) {}
    func Variant(i64 p_value)   : value_i64(0), current_type(TYPES::I64) {}
    func Variant(f32 p_value)   : value_f32(0.0f), current_type(TYPES::F32) {}
    func Variant(f64 p_value)   : value_f64(0.0f), current_type(TYPES::F64) {}

    func Variant(bool p_value)                  : value_bool(Bool(p_value)), current_type(TYPES::BOOL) {}
    func Variant(unsigned char p_value)         : value_u8(u8(p_value)), current_type(TYPES::U8) {}
    func Variant(unsigned short p_value)        : value_u16(u16(p_value)), current_type(TYPES::U16) {}
    func Variant(unsigned int p_value)          : value_u32(u32(p_value)), current_type(TYPES::U32) {}
    func Variant(unsigned long long p_value)    : value_u64(u64(p_value)), current_type(TYPES::U64) {}
    func Variant(char p_value)                  : value_i8(i8(p_value)), current_type(TYPES::I8) {}
    func Variant(short p_value)                 : value_i16(i16(p_value)), current_type(TYPES::I16) {}
    func Variant(int p_value)                   : value_i32(i32(p_value)), current_type(TYPES::I32) {}
    func Variant(long long p_value)             : value_i64(i64(p_value)), current_type(TYPES::I64) {}
    func Variant(float p_value)                 : value_f32(f32(p_value)), current_type(TYPES::F32) {}
    func Variant(double p_value)                : value_f64(f64(p_value)), current_type(TYPES::F64) {}

    // Helper Functions

    func TYPES get_current_type() const { return current_type; }
    func void set_current_type(TYPES p_type) { current_type = p_type; }
    func bool is_unknown_type() const { return no_type_specified; }

    // Getsets

    template<typename T>
    func T get_current_value() const {
        switch (get_current_type()) {
            case TYPES::BOOL: { return value_bool; }
            case TYPES::U8: { return value_u8; }
            case TYPES::U16: { return value_u16; }
            case TYPES::U32: { return value_u32; }
            case TYPES::U64: { return value_u64; }
            case TYPES::I8: { return value_i8; }
            case TYPES::I16: { return value_i16; }
            case TYPES::I32: { return value_i32; }
            case TYPES::I64: { return value_i64; }
            case TYPES::F32: { return value_f32; }
            case TYPES::F64: { return value_f64; }
        }
        
        return value_f32; // default value return
    }

    template<typename T>
    func bool is_same_as_current_value(T p_value) const { return current_type == p_value.get_type(); }
    func bool is_variant_same_as_current_value(Variant p_value) const { return get_current_type() == p_value.get_current_type(); }

    template<typename T>
    func void set(T p_value) { get_current_value<typeof(p_value)>() = p_value; }

    // Conversions

    func operator Bool() const { return value_bool; }
    func operator u8() const { return value_u8; }
    func operator u16() const { return value_u16; }
    func operator u32() const { return value_u32; }
    func operator u64() const { return value_u64; }
    func operator i8() const { return value_i8; }
    func operator i16() const { return value_i16; }
    func operator i32() const { return value_i32; }
    func operator i64() const { return value_i64; }
    func operator f32() const { return value_f32; }
    func operator f64() const { return value_f64; }

    // Operators (only between Variant types with the same current value)
    func Variant operator+(Variant p_value) const {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? Variant(value_bool + p_value.value_bool) : *this; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? Variant(value_u8 + p_value.value_u8) : *this; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? Variant(value_u16 + p_value.value_u16) : *this; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? Variant(value_u32 + p_value.value_u32) : *this; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? Variant(value_u64 + p_value.value_u64) : *this; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? Variant(value_i8 + p_value.value_i8) : *this; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? Variant(value_i16 + p_value.value_i16) : *this; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? Variant(value_i32 + p_value.value_i32) : *this; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? Variant(value_i64 + p_value.value_i64) : *this; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? Variant(value_f32 + p_value.value_f32) : *this; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? Variant(value_f64 + p_value.value_f64) : *this; }
        }
        
        return Variant(value_f32 + p_value.value_f32); // Default value return
    }
    func void operator+=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { if (is_variant_same_as_current_value(p_value))  { value_bool += p_value.value_bool; } return; }
            case TYPES::U8: { if (is_variant_same_as_current_value(p_value))  { value_u8 += p_value.value_u8; } return; }
            case TYPES::U16: { if (is_variant_same_as_current_value(p_value))  { value_u16 += p_value.value_u16; } return; }
            case TYPES::U32: { if (is_variant_same_as_current_value(p_value))  { value_u32 += p_value.value_u32; } return; }
            case TYPES::U64: { if (is_variant_same_as_current_value(p_value))  { value_u64 += p_value.value_u64; } return; }
            case TYPES::I8: { if (is_variant_same_as_current_value(p_value))  { value_i8 += p_value.value_i8; } return; }
            case TYPES::I16: { if (is_variant_same_as_current_value(p_value))  { value_i16 += p_value.value_i16; } return; }
            case TYPES::I32: { if (is_variant_same_as_current_value(p_value))  { value_i32 += p_value.value_i32; } return; }
            case TYPES::I64: { if (is_variant_same_as_current_value(p_value))  { value_i64 += p_value.value_i64; } return; }
            case TYPES::F32: { if (is_variant_same_as_current_value(p_value))  { value_f32 += p_value.value_f32; } return; }
            case TYPES::F64: { if (is_variant_same_as_current_value(p_value))  { value_f64 += p_value.value_f64; } return; }
        }
    }
    func Variant operator-(Variant p_value) const {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? Variant(value_bool - p_value.value_bool) : *this; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? Variant(value_u8 - p_value.value_u8) : *this; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? Variant(value_u16 - p_value.value_u16) : *this; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? Variant(value_u32 - p_value.value_u32) : *this; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? Variant(value_u64 - p_value.value_u64) : *this; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? Variant(value_i8 - p_value.value_i8) : *this; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? Variant(value_i16 - p_value.value_i16) : *this; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? Variant(value_i32 - p_value.value_i32) : *this; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? Variant(value_i64 - p_value.value_i64) : *this; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? Variant(value_f32 - p_value.value_f32) : *this; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? Variant(value_f64 - p_value.value_f64) : *this; }
        }
        
        return Variant(value_f32 - p_value.value_f32); // Default value return
    }
    func void operator-=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { if (is_variant_same_as_current_value(p_value))  { value_bool -= p_value.value_bool; } return; }
            case TYPES::U8: { if (is_variant_same_as_current_value(p_value))  { value_u8 -= p_value.value_u8; } return; }
            case TYPES::U16: { if (is_variant_same_as_current_value(p_value))  { value_u16 -= p_value.value_u16; } return; }
            case TYPES::U32: { if (is_variant_same_as_current_value(p_value))  { value_u32 -= p_value.value_u32; } return; }
            case TYPES::U64: { if (is_variant_same_as_current_value(p_value))  { value_u64 -= p_value.value_u64; } return; }
            case TYPES::I8: { if (is_variant_same_as_current_value(p_value))  { value_i8 -= p_value.value_i8; } return; }
            case TYPES::I16: { if (is_variant_same_as_current_value(p_value))  { value_i16 -= p_value.value_i16; } return; }
            case TYPES::I32: { if (is_variant_same_as_current_value(p_value))  { value_i32 -= p_value.value_i32; } return; }
            case TYPES::I64: { if (is_variant_same_as_current_value(p_value))  { value_i64 -= p_value.value_i64; } return; }
            case TYPES::F32: { if (is_variant_same_as_current_value(p_value))  { value_f32 -= p_value.value_f32; } return; }
            case TYPES::F64: { if (is_variant_same_as_current_value(p_value))  { value_f64 -= p_value.value_f64; } return; }
        }
    }
    func Variant operator*(Variant p_value) const {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? Variant(value_bool - p_value.value_bool) : *this; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? Variant(value_u8 - p_value.value_u8) : *this; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? Variant(value_u16 - p_value.value_u16) : *this; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? Variant(value_u32 - p_value.value_u32) : *this; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? Variant(value_u64 - p_value.value_u64) : *this; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? Variant(value_i8 - p_value.value_i8) : *this; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? Variant(value_i16 - p_value.value_i16) : *this; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? Variant(value_i32 - p_value.value_i32) : *this; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? Variant(value_i64 - p_value.value_i64) : *this; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? Variant(value_f32 - p_value.value_f32) : *this; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? Variant(value_f64 - p_value.value_f64) : *this; }
        }
        
        return Variant(value_f32 - p_value.value_f32); // Default value return
    }
    func void operator*=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { if (is_variant_same_as_current_value(p_value))  { value_bool *= p_value.value_bool; } return; }
            case TYPES::U8: { if (is_variant_same_as_current_value(p_value))  { value_u8 *= p_value.value_u8; } return; }
            case TYPES::U16: { if (is_variant_same_as_current_value(p_value))  { value_u16 *= p_value.value_u16; } return; }
            case TYPES::U32: { if (is_variant_same_as_current_value(p_value))  { value_u32 *= p_value.value_u32; } return; }
            case TYPES::U64: { if (is_variant_same_as_current_value(p_value))  { value_u64 *= p_value.value_u64; } return; }
            case TYPES::I8: { if (is_variant_same_as_current_value(p_value))  { value_i8 *= p_value.value_i8; } return; }
            case TYPES::I16: { if (is_variant_same_as_current_value(p_value))  { value_i16 *= p_value.value_i16; } return; }
            case TYPES::I32: { if (is_variant_same_as_current_value(p_value))  { value_i32 *= p_value.value_i32; } return; }
            case TYPES::I64: { if (is_variant_same_as_current_value(p_value))  { value_i64 *= p_value.value_i64; } return; }
            case TYPES::F32: { if (is_variant_same_as_current_value(p_value))  { value_f32 *= p_value.value_f32; } return; }
            case TYPES::F64: { if (is_variant_same_as_current_value(p_value))  { value_f64 *= p_value.value_f64; } return; }
        }
    }
    func Variant operator/(Variant p_value) const {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? Variant(value_bool / p_value.value_bool) : *this; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? Variant(value_u8 / p_value.value_u8) : *this; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? Variant(value_u16 / p_value.value_u16) : *this; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? Variant(value_u32 / p_value.value_u32) : *this; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? Variant(value_u64 / p_value.value_u64) : *this; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? Variant(value_i8 / p_value.value_i8) : *this; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? Variant(value_i16 / p_value.value_i16) : *this; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? Variant(value_i32 / p_value.value_i32) : *this; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? Variant(value_i64 / p_value.value_i64) : *this; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? Variant(value_f32 / p_value.value_f32) : *this; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? Variant(value_f64 / p_value.value_f64) : *this; }
        }
        
        return Variant(value_f32 - p_value.value_f32); // Default value return
    }
    func void operator/=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { if (is_variant_same_as_current_value(p_value))  { value_bool /= p_value.value_bool; } return; }
            case TYPES::U8: { if (is_variant_same_as_current_value(p_value))  { value_u8 /= p_value.value_u8; } return; }
            case TYPES::U16: { if (is_variant_same_as_current_value(p_value))  { value_u16 /= p_value.value_u16; } return; }
            case TYPES::U32: { if (is_variant_same_as_current_value(p_value))  { value_u32 /= p_value.value_u32; } return; }
            case TYPES::U64: { if (is_variant_same_as_current_value(p_value))  { value_u64 /= p_value.value_u64; } return; }
            case TYPES::I8: { if (is_variant_same_as_current_value(p_value))  { value_i8 /= p_value.value_i8; } return; }
            case TYPES::I16: { if (is_variant_same_as_current_value(p_value))  { value_i16 /= p_value.value_i16; } return; }
            case TYPES::I32: { if (is_variant_same_as_current_value(p_value))  { value_i32 /= p_value.value_i32; } return; }
            case TYPES::I64: { if (is_variant_same_as_current_value(p_value))  { value_i64 /= p_value.value_i64; } return; }
            case TYPES::F32: { if (is_variant_same_as_current_value(p_value))  { value_f32 /= p_value.value_f32; } return; }
            case TYPES::F64: { if (is_variant_same_as_current_value(p_value))  { value_f64 /= p_value.value_f64; } return; }
        }
    }
    func void operator=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_bool); } return; }
            case TYPES::U8: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_u8); } return; }
            case TYPES::U16: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_u16); } return; }
            case TYPES::U32: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_u32); } return; }
            case TYPES::U64: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_u64); } return; }
            case TYPES::I8: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_i8); } return; }
            case TYPES::I16: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_i16); } return; }
            case TYPES::I32: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_i32); } return; }
            case TYPES::I64: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_i64); } return; }
            case TYPES::F32: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_f32); } return; }
            case TYPES::F64: { if (is_variant_same_as_current_value(p_value))  { set(p_value.value_f64); } return; }
        }
    }
    func bool operator==(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool == p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 == p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 == p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 == p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 == p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 == p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 == p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 == p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 == p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 == p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 == p_value.value_f64 : false; }
        }
    }
    func bool operator!=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool != p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 != p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 != p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 != p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 != p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 != p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 != p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 != p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 != p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 != p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 != p_value.value_f64 : false; }
        }
    }
    func bool operator>(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool > p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 > p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 > p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 > p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 > p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 > p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 > p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 > p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 > p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 > p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 > p_value.value_f64 : false; }
        }
    }
    func bool operator>=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool >= p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 >= p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 >= p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 >= p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 >= p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 >= p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 >= p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 >= p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 >= p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 >= p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 >= p_value.value_f64 : false; }
        }
    }
    func bool operator<(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool < p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 < p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 < p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 < p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 < p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 < p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 < p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 < p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 < p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 < p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 < p_value.value_f64 : false; }
        }
    }
    func bool operator<=(Variant p_value) {
        switch (get_current_type()) {
            case TYPES::BOOL: { return is_variant_same_as_current_value(p_value) ? value_bool <= p_value.value_bool : false; }
            case TYPES::U8: { return is_variant_same_as_current_value(p_value) ? value_u8 <= p_value.value_u8 : false; }
            case TYPES::U16: { return is_variant_same_as_current_value(p_value) ? value_u16 <= p_value.value_u16 : false; }
            case TYPES::U32: { return is_variant_same_as_current_value(p_value) ? value_u32 <= p_value.value_u32 : false; }
            case TYPES::U64: { return is_variant_same_as_current_value(p_value) ? value_u64 <= p_value.value_u64 : false; }
            case TYPES::I8: { return is_variant_same_as_current_value(p_value) ? value_i8 <= p_value.value_i8 : false; }
            case TYPES::I16: { return is_variant_same_as_current_value(p_value) ? value_i16 <= p_value.value_i16 : false; }
            case TYPES::I32: { return is_variant_same_as_current_value(p_value) ? value_i32 <= p_value.value_i32 : false; }
            case TYPES::I64: { return is_variant_same_as_current_value(p_value) ? value_i64 <= p_value.value_i64 : false; }
            case TYPES::F32: { return is_variant_same_as_current_value(p_value) ? value_f32 <= p_value.value_f32 : false; }
            case TYPES::F64: { return is_variant_same_as_current_value(p_value) ? value_f64 <= p_value.value_f64 : false; }
        }
    }
    func void operator++() {
        switch (get_current_type()) {
            case TYPES::BOOL: { ++value_bool; return; }
            case TYPES::U8: { ++value_u8; return; }
            case TYPES::U16: { ++value_u16; return; }
            case TYPES::U32: { ++value_u32; return; }
            case TYPES::U64: { ++value_u64; return; }
            case TYPES::I8: { ++value_i8; return; }
            case TYPES::I16: { ++value_i16; return; }
            case TYPES::I32: { ++value_i32; return; }
            case TYPES::I64: { ++value_i64; return; }
            case TYPES::F32: { ++value_f32; return; }
            case TYPES::F64: { ++value_f64; return; }
        }
    }
    func void operator--() {
        switch (get_current_type()) {
            case TYPES::BOOL: { --value_bool; return; }
            case TYPES::U8: { --value_u8; return; }
            case TYPES::U16: { --value_u16; return; }
            case TYPES::U32: { --value_u32; return; }
            case TYPES::U64: { --value_u64; return; }
            case TYPES::I8: { --value_i8; return; }
            case TYPES::I16: { --value_i16; return; }
            case TYPES::I32: { --value_i32; return; }
            case TYPES::I64: { --value_i64; return; }
            case TYPES::F32: { --value_f32; return; }
            case TYPES::F64: { --value_f64; return; }
        }
    }
    func void operator!() {
        switch (get_current_type()) {
            case TYPES::BOOL: { !value_bool; return; }
            case TYPES::U8: { !value_u8; return; }
            case TYPES::U16: { !value_u16; return; }
            case TYPES::U32: { !value_u32; return; }
            case TYPES::U64: { !value_u64; return; }
            case TYPES::I8: { !value_i8; return; }
            case TYPES::I16: { !value_i16; return; }
            case TYPES::I32: { !value_i32; return; }
            case TYPES::I64: { !value_i64; return; }
            case TYPES::F32: { !value_f32; return; }
            case TYPES::F64: { !value_f64; return; }
        }
    }

    // Math Functions
    // TODO: Make universally available functions for Variant

    
};