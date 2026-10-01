/************************************/
/*         def_configs.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "platform_configs.hpp"

// Implementation is inspired by Godot Engine's 'typedefs.h' file:
// https://github.com/godotengine/godot/blob/master/core/typedefs.h

// Remove already existing defines that RatLab will use, to control how it's defined manually.
#ifdef always_inline
#undef always_inline
#endif
#ifdef test_inline
#undef test_inline
#endif
#ifdef func
#undef func
#endif

// Always inlines a function, regardless of compiling configurations.
#if defined(always_inline)
#undef always_inline
#endif

#if GNUC_ENABLED || CLANG_ENABLED
    #define always_inline __attribute__((always_inline)) inline
#elif MSVC_ENABLED
    #define always_inline __forceinline
#else
    #define always_inline inline
#endif

// Will never inline a function, regardless of compiling configurations.
#if defined(no_inline)
#undef no_inline
#endif

#if GNUC_ENABLED || CLANG_ENABLED
    #define no_inline __attribute__((noinline))
#elif MSVC_ENABLED
    #define no_inline __declspec(noinline)
#else
    #define no_inline
#endif

// Toggling whether the entire workspace will be compiled with 'consteval' instead of 'constexpr'.
#ifdef CONSTEVAL_ENABLED
    #undef CONSTEVAL_ENABLED
#endif

#define CONSTEVAL_ENABLED 0

// Will apply either 'constexpr' or 'consteval' depending on the use case.
// NOTE: 'constexpr' and 'consteval' is best used above C++17, otherwise no compile-time evaluation will happen.
#if CONSTEVAL_ENABLED
    #define func consteval
#elif CPP_VERSION >= 17
    #define func constexpr
#else
    #define func
#endif
