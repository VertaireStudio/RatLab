/************************************/
/*         def_configs.hpp          */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

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
#if defined(__GNUC__)
    #define always_inline __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define always_inline __forceinline
#else
    #define always_inline_ inline
#endif

// Only inlines a function when compiling the project to a 'dev' version.
#if defined(COMPILE_DEV)
    #define test_inline always_inline
#else
    #define test_inline
#endif

// Will never inline a function, regardless of compiling configurations.
#if defined(__GNUC__)
    #define no_inline __attribute__((noinline))
#elif defined(_MSC_VER)
    #define no_inline __declspec(noinline)
#else
    #define no_inline
#endif

// Will apply either 'constexpr' or 'consteval' depending on the use case.
// NOTE: 'constexpr' and 'consteval' is best used above C++17, otherwise no compile-time evaluation will happen.
#if defined(CXX) && CXX >= 20
    #if defined(COMPILE_CONSTEVAL)
        #define func consteval
    #else
        #define func constexpr
    #endif
#else
    #define func
#endif