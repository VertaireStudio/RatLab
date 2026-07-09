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
#include <limits>

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

static constexpr const unsigned char U8_MAX = std::numeric_limits<unsigned char>::max();
static constexpr const unsigned short U16_MAX = std::numeric_limits<unsigned short>::max();
static constexpr const unsigned int U32_MAX = std::numeric_limits<unsigned int>::max();
static constexpr const unsigned long long U64_MAX = std::numeric_limits<unsigned long long>::max();

static constexpr const char I8_MIN = std::numeric_limits<char>::min();
static constexpr const char I8_MAX = std::numeric_limits<char>::max();
static constexpr const short I16_MIN = std::numeric_limits<short>::min();
static constexpr const short I16_MAX = std::numeric_limits<short>::max();
static constexpr const int I32_MIN = std::numeric_limits<int>::min();
static constexpr const int I32_MAX = std::numeric_limits<int>::max();
static constexpr const long long I64_MIN = std::numeric_limits<long long>::min();
static constexpr const long long I64_MAX = std::numeric_limits<long long>::max();

static constexpr const float FLOAT_MIN = std::numeric_limits<float>::min();
static constexpr const float FLOAT_MAX = std::numeric_limits<float>::max();
static constexpr const double DOUBLE_MIN = std::numeric_limits<double>::min();
static constexpr const double DOUBLE_MAX = std::numeric_limits<double>::max();