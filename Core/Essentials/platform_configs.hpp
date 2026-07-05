/************************************/
/*      platform_configs.hpp        */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

/*All platforms which are considered supported.*/
enum PLATFORMS {
    UNKNOWN_PLATFORM,
    WINDOWS,
    LINUX,
    MACOS,
    IOS,
    WEB,
};

/*All Architextures which are considered supported.*/
enum ARCHITEXTURES {
    UNKNOWN_ARCHITEXTURE,
    BIT32,
    BIT64,
    ARM,
    AMD,
};

enum CXX_VERSIONS {
    UNKNOWN_CXX_VERSION,
    CXX_17,
    CXX_20,
    CXX_23,
    CXX_26,
};

// Platform Detection
#if defined(_WIN32)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::WINDOWS;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::BIT32;
#elif defined(_WIN64)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::WINDOWS;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::BIT64;
#elif defined(__linux__) || defined(__linux) || defined(linux)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::LINUX;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::UNKNOWN_ARCHITEXTURE;
#elif defined(__ANDROID__)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::ANDROID;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::UNKNOWN_ARCHITEXTURE;
#elif defined(__APPLE__) && defined(__MACH__)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::MACOS;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::UNKNOWN_ARCHITEXTURE;
#elif defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::IOS;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::UNKNOWN_ARCHITEXTURE;
#elif defined(__EMSCRIPTEN__) || defined(WASM) || defined(WEBASSEMBLY)
static constexpr const PLATFORMS CURRENT_PLATFORM = PLATFORMS::WEB;
static constexpr const ARCHITEXTURES CURRENT_PLATFORM_ARCHITEXTURE = ARCHITEXTURES::UNKNOWN_ARCHITEXTURE;
#endif

// C++ Version Detection
// A new principle lies here: Use 'CXX' macro for macros, and use 'CXX_VERSION' variable for everything else.
#if defined(CXX)
    #if CXX == 17
static constexpr const CXX_VERSIONS CXX_VERSION = CXX_VERSIONS::CXX_17;
    #elif CXX == 20
static constexpr const CXX_VERSIONS CXX_VERSION = CXX_VERSIONS::CXX_20;
    #elif CXX == 23
static constexpr const CXX_VERSIONS CXX_VERSION = CXX_VERSIONS::CXX_23;
    #elif CXX == 26
static constexpr const CXX_VERSIONS CXX_VERSION = CXX_VERSIONS::CXX_26;
    #endif
#else
static constexpr const CXX_VERSIONS CXX_VERSION = CXX_VERSIONS::UNKNOWN_CXX_VERSION;
#endif
