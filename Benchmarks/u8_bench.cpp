/************************************/
/*          u8_bench.cpp            */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Benchmarker.hpp"
#include "../Core/Types/u8.hpp"

/*-----------------------------------------------------------------------------------------------*/
// Measurement notes
//
// The bodies below are compiled with optimizations enabled, which means the compiler is free
// to delete any loop it can prove to be pointless. A body such as 'total += value.get()' is
// therefore folded into a single multiplication, and '++value; --value;' is removed entirely,
// both of which would report a timing of zero.
//
// Every loop keeps a value which is passed through 'Benchmarker::do_not_optimize' on each
// iteration. The generated code is a single register operation, so the loop survives and the
// reported number still reflects the cost of the u8 operation itself.
/*-----------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------*/
/* Construction and value access                                                                                  */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the cost of default and value construction.
static void bench_construction(const unsigned long long p_iterations) {
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        const u8 defaulted;
        const u8 valued((unsigned char)index);
        Benchmarker::do_not_optimize(defaulted);
        Benchmarker::do_not_optimize(valued);
    }
}

// Measures the cost of copying an existing u8.
static void bench_copy(const unsigned long long p_iterations) {
    const u8 source((unsigned char)42);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        const u8 copy(source);
        Benchmarker::do_not_optimize(copy);
    }
}

// Measures the cost of reading a value out of a u8.
static void bench_get(const unsigned long long p_iterations) {
    const u8 source((unsigned char)42);
    unsigned long long total = 0;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        total += source.get();
        Benchmarker::do_not_optimize(total);
    }
    Benchmarker::do_not_optimize(total);
}

// Measures the cost of writing a value into a u8.
static void bench_set(const unsigned long long p_iterations) {
    u8 target;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        target.set((unsigned char)index);
        Benchmarker::do_not_optimize(target);
    }
    Benchmarker::do_not_optimize(target);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Arithmetic                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the arithmetic operators, all five of them for both overload sets.
static void bench_arithmetic(const unsigned long long p_iterations) {
    const u8 first((unsigned char)37);
    const u8 second((unsigned char)5);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(first + second);
        Benchmarker::do_not_optimize(first - second);
        Benchmarker::do_not_optimize(first * second);
        Benchmarker::do_not_optimize(first / second);
        Benchmarker::do_not_optimize(first % second);
        Benchmarker::do_not_optimize(first + (unsigned char)5);
        Benchmarker::do_not_optimize(first - (unsigned char)5);
        Benchmarker::do_not_optimize(first * (unsigned char)5);
        Benchmarker::do_not_optimize(first / (unsigned char)5);
        Benchmarker::do_not_optimize(first % (unsigned char)5);
    }
}

// Measures the same arithmetic, written back into an u8 after every operation.
static void bench_arithmetic_accumulated(const unsigned long long p_iterations) {
    const u8 first((unsigned char)37);
    const u8 second((unsigned char)5);
    u8 accumulator((unsigned char)0);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        accumulator = first + second;
        Benchmarker::do_not_optimize(accumulator);
        accumulator = accumulator - second;
        Benchmarker::do_not_optimize(accumulator);
        accumulator = accumulator * second;
        Benchmarker::do_not_optimize(accumulator);
    }
    Benchmarker::do_not_optimize(accumulator);
}

// Measures the guard against a division by zero, taken on every division.
static void bench_division_by_zero_guard(const unsigned long long p_iterations) {
    const u8 dividend((unsigned char)255);
    const u8 zero((unsigned char)0);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(dividend / zero);
    }
}

// Measures the increment and decrement operators, which read, write and re-check the access.
static void bench_increment(const unsigned long long p_iterations) {
    u8 value((unsigned char)0);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        ++value;
        Benchmarker::do_not_optimize(value);
        --value;
        Benchmarker::do_not_optimize(value);
    }
    Benchmarker::do_not_optimize(value);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Bitwise operations and shifts                                                                                  */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the bitwise operators.
static void bench_bitwise(const unsigned long long p_iterations) {
    const u8 first((unsigned char)0xF0);
    const u8 second((unsigned char)0x3C);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(first & second);
        Benchmarker::do_not_optimize(first | second);
        Benchmarker::do_not_optimize(first ^ second);
    }
}

// Measures the shift operators.
static void bench_shifts(const unsigned long long p_iterations) {
    const u8 value((unsigned char)0x81);
    const u8 amount((unsigned char)3);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(value << amount);
        Benchmarker::do_not_optimize(value >> amount);
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Comparisons                                                                                                     */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the comparison operators against another u8.
static void bench_comparison(const unsigned long long p_iterations) {
    const u8 first((unsigned char)128);
    const u8 second((unsigned char)128);
    unsigned long long hits = 0;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        hits += first == second ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
        hits += first < second ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
        hits += first >= second ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
    }
    Benchmarker::do_not_optimize(hits);
}

// Measures the comparison operators against a plain unsigned char.
static void bench_comparison_unsigned_char(const unsigned long long p_iterations) {
    const u8 first((unsigned char)128);
    unsigned long long hits = 0;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        hits += first == (unsigned char)128 ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
        hits += first != (unsigned char)128 ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
    }
    Benchmarker::do_not_optimize(hits);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Math helpers                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the min and max helpers.
static void bench_min_max(const unsigned long long p_iterations) {
    const u8 first((unsigned char)128);
    const u8 second((unsigned char)64);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(first.min(second));
        Benchmarker::do_not_optimize(first.max(second));
    }
}

// Measures the clamp helper, which calls min and max in sequence.
static void bench_clamp(const unsigned long long p_iterations) {
    const u8 first((unsigned char)128);
    const u8 minimum((unsigned char)10);
    const u8 maximum((unsigned char)200);
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        Benchmarker::do_not_optimize(first.clamp(minimum, maximum));
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Accessability                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the accessability queries, which every guarded operation performs.
static void bench_access_queries(const unsigned long long p_iterations) {
    const u8 value((unsigned char)128);
    unsigned long long hits = 0;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        hits += value.can_read() ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
        hits += value.can_write() ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
        hits += value.can_overwrite_access() ? 1ull : 0ull;
        Benchmarker::do_not_optimize(hits);
    }
    Benchmarker::do_not_optimize(hits);
}

// Measures the cost of the guarded 'set', which pays for the accessability checks.
static void bench_guarded_set(const unsigned long long p_iterations) {
    u8 value;
    for (unsigned long long index = 0; index < p_iterations; ++index) {
        value.set((unsigned char)index);
        Benchmarker::do_not_optimize(value.get());
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Registration                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------*/

// Every benchmark is registered at static-initialization time, which happens before 'main' runs.
static const Benchmarker::AutoBenchmark register_u8_benchmarks[] = {
    {"u8/construction",             &bench_construction},
    {"u8/copy",                     &bench_copy},
    {"u8/get",                      &bench_get},
    {"u8/set",                      &bench_set},
    {"u8/arithmetic",               &bench_arithmetic},
    {"u8/arithmetic/accumulated",   &bench_arithmetic_accumulated},
    {"u8/arithmetic/div_by_zero",   &bench_division_by_zero_guard},
    {"u8/arithmetic/increment",     &bench_increment},
    {"u8/bitwise",                  &bench_bitwise},
    {"u8/bitwise/shifts",           &bench_shifts},
    {"u8/comparison",               &bench_comparison},
    {"u8/comparison/unsigned_char", &bench_comparison_unsigned_char},
    {"u8/math/min_max",             &bench_min_max},
    {"u8/math/clamp",               &bench_clamp},
    {"u8/accessability/queries",    &bench_access_queries},
    {"u8/accessability/guarded_set", &bench_guarded_set},
};
