/************************************/
/*          console.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ctime>

#if defined(_WIN32)
    #include <direct.h>
    #include <io.h>
    #define RATLAB_ISATTY(stream) (_isatty(_fileno(stream)) != 0)
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define RATLAB_ISATTY(stream) (::isatty(::fileno(stream)) != 0)
#endif

// Turns a macro into a string literal, used to report the compiler version.
#define RATLAB_STRINGIFY_HELPER(p_value) #p_value
#define RATLAB_STRINGIFY(p_value) RATLAB_STRINGIFY_HELPER(p_value)

// The Console type: The shared output plumbing of the Tester and the Benchmarker.
// It owns the terminal detection, the colors, and the number formatting, so that both
// runners lay their tables out identically and stay readable in a plain CI log.
// Colors and in-place progress are switched off automatically when stdout is not a terminal.
class Console {
    public:
    // Whether ANSI escape sequences may be written to the output.
    bool color = false;
    // Whether the output is a live terminal, which allows redrawing a line in place.
    bool interactive = false;

    // Constructor.
    // NOTE: Not 'func' - terminal detection is a runtime-only query.
    Console() {
        interactive = RATLAB_ISATTY(stdout);
        color = interactive;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Colors ──────────────────────────────────────────────────────────────────────────────

    func static const char *bold() { return "\x1b[1m"; }
    func static const char *dim() { return "\x1b[2m"; }
    func static const char *red() { return "\x1b[31m"; }
    func static const char *green() { return "\x1b[32m"; }
    func static const char *yellow() { return "\x1b[33m"; }
    func static const char *cyan() { return "\x1b[36m"; }
    func static const char *reset() { return "\x1b[0m"; }

    // Returns the given escape sequence, or nothing at all when the output is not a terminal.
    func const char *paint(const char *p_code) const { return color ? p_code : ""; }

    // Returns the separator placed between the parts of a summary line.
    func const char *separator() const { return color ? " \xE2\x80\xA2 " : " | "; }

    // Returns the singular or the plural form, depending on the given amount.
    func static const char *plural(const unsigned long long p_count, const char *p_singular,
                                   const char *p_plural) {
        return p_count == 1ull ? p_singular : p_plural;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Glyphs ─────────────────────────────────────────────────────────────────────────────

    // Returns the status marker of a test case: a check mark, a cross, or their ASCII
    // counterparts when the output cannot be trusted to render anything but ASCII.
    func const char *glyph_ok() const { return color ? "\xE2\x9C\x93" : "+"; }
    func const char *glyph_fail() const { return color ? "\xE2\x9C\x97" : "x"; }
    /*-------------------------------------------------------------------------------*/

    // ── Layout ──────────────────────────────────────────────────────────────────────────────

    // Erases the current line and moves the cursor back to its start, so that the next write
    // overwrites whatever was on it. Does nothing when the output is not a live terminal.
    // NOTE: Not 'func' - 'printf' is a runtime-only operation.
    void clear_line() const {
        if (interactive) {
            std::printf("\r\x1b[2K");
        }
    }

    // Writes the name of the test case which is currently running, in place, so that the
    // name stays visible even if the process dies halfway through it.
    // NOTE: Not 'func' - 'printf' is a runtime-only operation.
    void progress(const char *p_name, const std::size_t p_width) const {
        if (!interactive) {
            return;
        }
        std::printf("\r\x1b[2K  %-*s  ", (int)p_width, p_name);
        std::fflush(stdout);
    }

    // Writes a horizontal rule of the given width, using box drawing characters when the
    // output is a terminal and plain dashes otherwise.
    // NOTE: Not 'func' - 'printf' is a runtime-only operation.
    void rule(const std::size_t p_width) const {
        const char *fill = color ? "\xE2\x94\x80" : "-";
        for (std::size_t index = 0; index < p_width; ++index) {
            std::printf("%s", fill);
        }
        std::printf("\n");
    }
    /*-------------------------------------------------------------------------------*/

    // ── Number formatting ──────────────────────────────────────────────────────────────────

    // Writes an unsigned integer with thousands separators, e.g. 87266902 becomes '87,266,902'.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    static void format_count(char *r_buffer, const std::size_t p_size,
                             const unsigned long long p_value) {
        // The digits are collected starting at the least significant one.
        char digits[24];
        std::size_t length = 0;
        unsigned long long rest = p_value;
        do {
            digits[length++] = (char)('0' + (int)(rest % 10ull));
            rest /= 10ull;
        } while (rest > 0ull);

        // They are written back starting at the most significant one, a separator is placed
        // whenever the amount of digits left to write is a multiple of three.
        std::size_t written = 0;
        for (std::size_t position = 0; position < length; ++position) {
            if (position > 0 && (length - position) % 3 == 0) {
                if (written + 1 >= p_size) {
                    break;
                }
                r_buffer[written++] = ',';
            }
            if (written + 1 >= p_size) {
                break;
            }
            r_buffer[written++] = digits[length - position - 1];
        }
        r_buffer[written] = '\0';
    }

    // Writes a duration given in nanoseconds using the unit which keeps it readable,
    // e.g. 12594000 becomes '12.594 ms'.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    static void format_duration(char *r_buffer, const std::size_t p_size, const double p_ns) {
        if (p_ns < 1000.0) {
            std::snprintf(r_buffer, p_size, "%.0f ns", p_ns);
        } else if (p_ns < 1000000.0) {
            std::snprintf(r_buffer, p_size, "%.3f us", p_ns / 1000.0);
        } else if (p_ns < 1000000000.0) {
            std::snprintf(r_buffer, p_size, "%.3f ms", p_ns / 1000000.0);
        } else {
            std::snprintf(r_buffer, p_size, "%.3f s", p_ns / 1000000000.0);
        }
    }

    // Writes an amount of iterations per second using a magnitude prefix,
    // e.g. 7135613856 becomes '7.14 G/s'.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    static void format_rate(char *r_buffer, const std::size_t p_size, const double p_rate) {
        if (p_rate < 1000.0) {
            std::snprintf(r_buffer, p_size, "%.0f /s", p_rate);
        } else if (p_rate < 1000000.0) {
            std::snprintf(r_buffer, p_size, "%.2f K/s", p_rate / 1000.0);
        } else if (p_rate < 1000000000.0) {
            std::snprintf(r_buffer, p_size, "%.2f M/s", p_rate / 1000000.0);
        } else if (p_rate < 1000000000000.0) {
            std::snprintf(r_buffer, p_size, "%.2f G/s", p_rate / 1000000000.0);
        } else {
            std::snprintf(r_buffer, p_size, "%.2f T/s", p_rate / 1000000000000.0);
        }
    }

    // Writes a string such as '12 checks' or '1 check', right aligned in the given width.
    // NOTE: Not 'func' - 'snprintf' is a runtime-only operation.
    static void format_checks(char *r_buffer, const std::size_t p_size,
                              const unsigned long long p_checks) {
        std::snprintf(r_buffer, p_size, "%llu %s", p_checks, p_checks == 1ull ? "check" : "checks");
    }

    // Writes the current local date and time as '2026-10-02 15:04:05', so that a report file
    // says when it was produced.
    // NOTE: Not 'func' - the clock and 'strftime' are runtime-only operations.
    static void format_timestamp(char *r_buffer, const std::size_t p_size) {
        const std::time_t now = std::time(nullptr);
        // 'localtime' returns a pointer to shared storage, so the result is copied out of it.
        std::tm parts = {};
#if defined(_WIN32)
        localtime_s(&parts, &now);
#else
        localtime_r(&now, &parts);
#endif
        if (std::strftime(r_buffer, p_size, "%Y-%m-%d %H:%M:%S", &parts) == 0) {
            std::snprintf(r_buffer, p_size, "unknown");
        }
    }
    /*-------------------------------------------------------------------------------*/

    // ── Files ──────────────────────────────────────────────────────────────────────────────

    // Creates the given directory when it does not exist yet, so that a report can be written
    // into a directory the working directory does not have yet. Only a single level is
    // created, which is all the report directory needs.
    // Returns whether the directory is available afterwards.
    // NOTE: Not 'func' - creating a directory is a runtime-only operation.
    static bool ensure_directory(const char *p_path) {
#if defined(_WIN32)
        const int status = _mkdir(p_path);
#else
        const int status = ::mkdir(p_path, 0755);
#endif
        if (status == 0) {
            return true;
        }
        // An already existing directory is exactly what was asked for, the error is the only
        // one which is not a failure here.
        return errno == EEXIST;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Environment ────────────────────────────────────────────────────────────────────────

    // Returns the name of the platform the workspace is being compiled for.
    func static const char *platform_name() {
#if defined(LINUX_ENABLED)
        return "Linux";
#elif defined(WINDOWS_ENABLED)
        return "Windows";
#elif defined(MACOS_ENABLED)
        return "macOS";
#elif defined(IOS_ENABLED)
        return "iOS";
#elif defined(ANDROID_ENABLED)
        return "Android";
#elif defined(WEB_ENABLED)
        return "Web";
#else
        return "Unknown";
#endif
    }

    // Returns the name and version of the compiler in use.
    func static const char *compiler_name() {
#if defined(CLANG_ENABLED)
        return "Clang " RATLAB_STRINGIFY(__clang_major__) "." RATLAB_STRINGIFY(__clang_minor__);
#elif defined(GNUC_ENABLED)
        return "GCC " RATLAB_STRINGIFY(__GNUC__) "." RATLAB_STRINGIFY(__GNUC_MINOR__);
#elif defined(MSVC_ENABLED)
        return "MSVC " RATLAB_STRINGIFY(_MSC_VER);
#else
        return "Unknown";
#endif
    }

    // Returns the C++ standard the workspace is being compiled for, as it is reported by the
    // compiler. NOTE: this is '__cplusplus' mapped onto the enum of the engine, which has to be
    // read through 'to_name' to be printable.
    func static const char *cpp_standard_name() {
        return to_name(CURRENT_CPP_VERSION);
    }

    // Returns the name of a C++ version of the engine, as used in the report headers.
    func static const char *to_name(const CPP_VERSIONS p_version) {
        switch (p_version) {
            case CPP_VERSIONS::CPP_17: return "C++17";
            case CPP_VERSIONS::CPP_20: return "C++20";
            case CPP_VERSIONS::CPP_23: return "C++23";
            case CPP_VERSIONS::CPP_26: return "C++26";
            default: return "C++?";
        }
    }
};
