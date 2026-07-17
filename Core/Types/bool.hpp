/************************************/
/*            bool.hpp              */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/type_configs.hpp"

#include "../Essentials/type_configs.hpp"
#include "../Essentials/def_configs.hpp"

// Empty structs, used for conversion in 'type_conversion.cpp'

struct u8;
struct u16;
struct u32;
struct u64;
struct i8;
struct i16;
struct i32;
struct i64;
struct f32;
struct f64;


struct Bool {
    static const Bool TRUE;
    static const Bool FALSE;

    // Data
    bool value;

    // Constructor
    func Bool() : value(0) {}
    func Bool(bool p_value) : value(p_value) {}
    
    template<typename T>
    func Bool(T p_value) : value((bool)p_value) {}

    // Getsets
    func bool get() const { return value; }
    func void set(bool p_value) { value = p_value; }

    // Operators
    func Bool operator+(Bool p_value) const { return value || p_value.value ? TRUE : FALSE; }
    func void operator+=(Bool p_value) { if (!value && p_value.value) { value = p_value.value; } }
    func Bool operator-(Bool p_value) const { return value && p_value.value ? FALSE : TRUE; }
    func void operator-=(Bool p_value) { if (p_value.value) { value = false; } }
    func Bool operator*(Bool p_value) const { return value && p_value.value ? TRUE : FALSE; }
    func void operator*=(Bool p_value) { if (value && !p_value.value) { value = p_value.value; } }
    func Bool operator&&(Bool p_value) const { return value && p_value.value; }
    func Bool operator||(Bool p_value) const { return value || p_value.value; }

    func Bool operator!() { return Bool(!value); }

    func bool operator==(Bool p_value) const { return value == p_value.value; }
    func bool operator!=(Bool p_value) const { return value != p_value.value; }
    func bool operator>(Bool p_value) const { return value > p_value.value; }
    func bool operator>=(Bool p_value) const { return value >= p_value.value; }
    func bool operator<(Bool p_value) const { return value < p_value.value; }
    func bool operator<=(Bool p_value) const { return value <= p_value.value; }

    func void operator++() { value ? value = false : value = true; }
    func void operator--() { value ? value = false : value = true; }


    // Helper Functions
    func TYPES get_type() { return TYPES::BOOL; }

    // Math Functions
    // Logic functions based on Sebastian Lague's "Explaining how computers work" video:
    // https://www.youtube.com/watch?v=QZwneRb-zqA&t=50s

    static func Bool AND(Bool p_input1, Bool p_input2) { return p_input1 && p_input2; }
    static func Bool NOT(Bool p_input) { return !p_input; }
    static func Bool NAND(Bool p_input1, Bool p_input2) { return NOT(AND(p_input1, p_input2)); }
    static func Bool OR(Bool p_input1, Bool p_input2) { return NAND(NOT(p_input1), NOT(p_input2)); }
    static func Bool XOR(Bool p_input1, Bool p_input2) { return AND(OR(p_input1, p_input2), NAND(p_input1, p_input2)); }
    static func Bool NXOR(Bool p_input1, Bool p_input2) { return NOT(XOR(p_input1, p_input2)); }

    // Converters

    func operator u8();
    func operator u16();
    func operator u32();
    func operator u64();
    func operator i8();
    func operator i16();
    func operator i32();
    func operator i64();
    func operator f32();
    func operator f64();
};

inline const func Bool Bool::TRUE = {true};
inline const func Bool Bool::FALSE = {false};