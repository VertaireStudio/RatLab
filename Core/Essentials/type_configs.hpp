/************************************/
/*        type_configs.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

/*All basic types which can be used in Variant, as well as any other scripts as a replacement for platform specific types.*/
/*Also used for type identification.*/
enum TYPES {
    ARRAY,
    BOOL,
    F32,
    F64,
    I8,
    I16,
    I32,
    I64,
    PACKED_ARRAY,
    STRING,
    U8,
    U16,
    U32,
    U64,
    VECTOR,
    VECTOR2,
    VECTOR2I,
    VECTOR3,
    VECTOR3I,
    VECTOR4,
    VECTOR4I,
};

// Warnings are treated as intentional.

static constexpr const unsigned char U8_MAX = 255;
static constexpr const unsigned short U16_MAX = 65535;
static constexpr const unsigned int U32_MAX = 4294967295;
static constexpr const unsigned long long U64_MAX = 18446744073709551615;

static constexpr const char I8_MIN = -128;
static constexpr const char I8_MAX = 127;
static constexpr const short I16_MIN = -32768;
static constexpr const short I16_MAX = 32767;
static constexpr const int I32_MIN = -2147483648;
static constexpr const int I32_MAX = 2147483647;
static constexpr const long long I64_MIN = -9223372036854775808;
static constexpr const long long I64_MAX = 9223372036854775807;