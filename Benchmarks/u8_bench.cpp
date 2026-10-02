/************************************/
/*          u8_bench.cpp            */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Benchmarker.hpp"
#include "../Core/Types/u8.hpp"

/*-------------------------------------------------------------------------------------------------------------*/
/* How a benchmark is written here                                                                             */
/*-------------------------------------------------------------------------------------------------------------*/
//
// A body is a lambda which is handed a 'Bencher' and spends the iterations of a sample through
// it. The framework owns the loop, the calibration and the statistics; the body only says what
// the work is:
//
//     group.bench_function("get", [](Bencher &p_bencher) {
//         p_bencher.iter([] { return source.get(); });
//     });
//
// Two separate things have to be held back from the optimizer, and they need two separate tools
// for it. Both are compiled with optimizations enabled, which means the compiler is free to
// evaluate anything it can prove at compile time and to delete anything it can prove nobody
// looks at.
//
// The operands are made opaque with 'Benchmarker::black_box', which hands a value to the
// compiler and gets it back without telling it where the value came from. Writing
// 'const u8 first(37); const u8 second(5);' and then measuring 'first + second' folds the
// addition away while the workspace is being compiled: both operands are literals, so their sum
// is a literal too. The barrier is a volatile assembly statement, which also keeps the work
// around it inside the loop, so an operation whose operands happen to be the same on every
// iteration is still measured.
//
// Every body below carries an operand of its own which changes from one iteration to the next.
// That is not what keeps the body alive — the barrier does that — it is what makes the operands
// the ones a real caller passes, which are not the same twice: a divide by a value which never
// changes is a divide the compiler is free to reason about in a way it cannot reason about a
// real one. The counter is cheap (0.28 ns per iteration on GCC 13.3, measured against a body
// which is nothing but the counter and the barrier) and it is what keeps a body honest about
// what it was given.
//
// The results are kept alive by the framework itself: 'iter' hands whatever a routine returns to
// the compiler barrier before it is dropped, so a body does not have to do it. What a body does
// have to do is keep the intermediate results of a sequence of operations, which is what
// 'Benchmarker::black_box' is called for again inside the loop.
//
// An operand that differs on every iteration is not by itself a guarantee that the body measures
// what it says. A predicate the compiler can prove has the same answer every time is folded
// away no matter how carefully its input is hidden: the three 'can_*' helpers of the Variant
// answer 'true' for every accessability they are given, so the bodies below which ask about the
// accessability measure the handling of the operand rather than the query. Each of them says so
// where it is written, because a benchmark which quietly measures nothing is worse than no
// benchmark at all.
//
// The one operand deliberately left as a constant is the shift amount in 'bitwise/shifts'. The
// point of that body is the shift by an immediate, and an amount the compiler does not know
// would have it emit the shift by a register instead, which is a different instruction and a
// different thing to measure. The shifted value still carries the counter, so the shift itself
// cannot be folded away.
/*-------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------*/
/* Construction and value access                                                                                  */
/*--------------------------------------------------------------------------------------------------------------*/

// Measures the cost of default and value construction, in one body.
//
// NOTE: Only the value construction is really measured. A default-constructed u8 has nothing
// in it to vary, so it is a constant the compiler builds once and the only thing left of it in
// the loop is the read below. The two cannot be measured apart: a default construction which
// differed on every iteration would no longer be the default construction this benchmark is
// about.
//
// A group function is the one function of the workspace which cannot be 'static': it is named
// from 'main.cpp' by 'BENCHMARK_GROUP', and a static function of one translation unit is
// invisible to another.
void bench_u8(Benchmarker &p_benchmarker) {
    BenchmarkGroup group = p_benchmarker.benchmark_group("u8");
    /*----------------------------------------------------------------------------------------------------------*/

    group.bench_function("construction", [](Bencher &p_bencher) {
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 defaulted;
            const u8 valued(counter);
            return valued.get() + defaulted.get();
        });
    });

    // Measures the cost of copying an existing u8. The source is built from the counter rather
    // than kept outside the loop, because a copy of a value which never changes is a copy the
    // compiler can do once: what is measured here is a copy, and building what is copied is the
    // only way to make it a copy at all.
    group.bench_function("copy", [](Bencher &p_bencher) {
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 source = Benchmarker::black_box<u8>(counter);
            const u8 copy(source);
            return copy;
        });
    });

    // Measures the cost of reading a value out of a u8.
    group.bench_function("get", [](Bencher &p_bencher) {
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 source = Benchmarker::black_box<u8>(counter);
            return source.get();
        });
    });

    // Measures the cost of writing a value into a u8.
    group.bench_function("set", [](Bencher &p_bencher) {
        u8 target = Benchmarker::black_box<u8>((unsigned char)0);
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            target.set(counter);
            return target;
        });
    });
    /*--------------------------------------------------------------------------------------------------------------*/
    /* Arithmetic                                                                                                  */
    /*--------------------------------------------------------------------------------------------------------------*/

    // Measures the arithmetic operators, all five of them for both overload sets. Every result
    // is kept, since a sequence which only keeps its last one is nine operations short.
    group.bench_function("arithmetic", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            const u8 second = Benchmarker::black_box<u8>((unsigned char)5);
            u8 accumulator = Benchmarker::black_box<u8>(first + second);
            accumulator = Benchmarker::black_box<u8>(first - second);
            accumulator = Benchmarker::black_box<u8>(first * second);
            accumulator = Benchmarker::black_box<u8>(first / second);
            accumulator = Benchmarker::black_box<u8>(first % second);
            accumulator = Benchmarker::black_box<u8>(first + (unsigned char)5);
            accumulator = Benchmarker::black_box<u8>(first - (unsigned char)5);
            accumulator = Benchmarker::black_box<u8>(first * (unsigned char)5);
            accumulator = Benchmarker::black_box<u8>(first / (unsigned char)5);
            accumulator = Benchmarker::black_box<u8>(first % (unsigned char)5);
            return accumulator;
        });
    });

    // Measures the same arithmetic, written back into an u8 after every operation.
    group.bench_function("arithmetic/accumulated", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            const u8 second = Benchmarker::black_box<u8>((unsigned char)5);
            u8 accumulator = Benchmarker::black_box<u8>(0);
            accumulator = Benchmarker::black_box<u8>(first + second);
            accumulator = Benchmarker::black_box<u8>(accumulator - second);
            accumulator = Benchmarker::black_box<u8>(accumulator * second);
            return accumulator;
        });
    });

    // Measures the guard against a division by zero, taken on every division.
    group.bench_function("arithmetic/div_by_zero", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            // The divisor has to be opaque. Left as a literal the compiler proves at compile
            // time that it is zero, folds the guard away and hands back the maximum value
            // without ever dividing, which is the opposite of what this body is measuring.
            const u8 dividend = Benchmarker::black_box<u8>(counter);
            const u8 zero = Benchmarker::black_box<u8>((unsigned char)0);
            const u8 divided = Benchmarker::black_box<u8>(dividend / zero);
            return divided;
        });
    });

    // Measures the increment operator, which reads, writes and re-checks the access.
    //
    // NOTE: The pair '++value; --value;' is not measured, because it is the identity for every
    // possible input: the compiler proves the pair leaves the value as it was and hoists it
    // out of the loop. No obstacle the framework has can catch that, since the value the pair
    // leaves behind is the same on every iteration. A chain of increments is the same work
    // without the property which makes it free.
    group.bench_function("arithmetic/increment", [](Bencher &p_bencher) {
        u8 value = Benchmarker::black_box<u8>((unsigned char)0);
        p_bencher.iter([&] {
            const u8 incremented = Benchmarker::black_box<u8>(++value);
            return incremented;
        });
    });
    /*--------------------------------------------------------------------------------------------------------------*/
    /* Bitwise operations and shifts                                                                              */
    /*--------------------------------------------------------------------------------------------------------------*/

    // Measures the bitwise operators.
    group.bench_function("bitwise", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            const u8 second = Benchmarker::black_box<u8>((unsigned char)0x3C);
            const u8 anded = Benchmarker::black_box<u8>(first & second);
            const u8 ored = Benchmarker::black_box<u8>(first | second);
            const u8 upped = Benchmarker::black_box<u8>(first ^ second);
            return anded.get() + ored.get() + upped.get();
        });
    });

    // Measures the shift operators.
    group.bench_function("bitwise/shifts", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 value = Benchmarker::black_box<u8>(counter);
            // The amount stays a known constant on purpose, so that the compiler emits the
            // shift by an immediate: an amount it cannot know selects the shift by a register
            // instead, which is a different instruction and a different thing to measure. The
            // shifted value carries the counter, so the shift itself cannot be folded away.
            const u8 shifted_left = Benchmarker::black_box<u8>(value << (unsigned char)3);
            const u8 shifted_right = Benchmarker::black_box<u8>(value >> (unsigned char)3);
            return shifted_left.get() + shifted_right.get();
        });
    });
    /*--------------------------------------------------------------------------------------------------------------*/
    /* Comparisons                                                                                                  */
    /*--------------------------------------------------------------------------------------------------------------*/

    // Measures the comparison operators against another u8. Every comparison is kept on its own:
// a body which only reported whether the two were equal could answer it once and reuse the
// answer for the other two.
group.bench_function("comparison", [](Bencher &p_bencher) {
        unsigned char counter = 128;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            const u8 second = Benchmarker::black_box<u8>((unsigned char)128);
            unsigned long long hits = 0ull;
            hits += Benchmarker::black_box<bool>(first == second) ? 1ull : 0ull;
            hits += Benchmarker::black_box<bool>(first < second) ? 1ull : 0ull;
            hits += Benchmarker::black_box<bool>(first >= second) ? 1ull : 0ull;
            return hits;
        });
    });

    // Measures the comparison operators against a plain unsigned char.
    group.bench_function("comparison/unsigned_char", [](Bencher &p_bencher) {
        unsigned char counter = 128;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            unsigned long long hits = 0ull;
            hits += Benchmarker::black_box<bool>(first == (unsigned char)128) ? 1ull : 0ull;
            hits += Benchmarker::black_box<bool>(first != (unsigned char)128) ? 1ull : 0ull;
            return hits;
        });
    });
    /*--------------------------------------------------------------------------------------------------------------*/
    /* Math helpers                                                                                                  */
    /*--------------------------------------------------------------------------------------------------------------*/

    // Measures the min and max helpers.
    group.bench_function("math/min_max", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 first = Benchmarker::black_box<u8>(counter);
            const u8 second = Benchmarker::black_box<u8>((unsigned char)64);
            const u8 lowest = Benchmarker::black_box<u8>(first.min(second));
            const u8 highest = Benchmarker::black_box<u8>(first.max(second));
            return lowest.get() + highest.get();
        });
    });

    // Measures the clamp helper, which calls min and max in sequence.
    group.bench_function("math/clamp", [](Bencher &p_bencher) {
        unsigned char counter = 1;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const u8 value = Benchmarker::black_box<u8>(counter);
            const u8 lowest = Benchmarker::black_box<u8>((unsigned char)10);
            const u8 highest = Benchmarker::black_box<u8>((unsigned char)200);
            const u8 clamped = Benchmarker::black_box<u8>(value.clamp(lowest, highest));
            return clamped;
        });
    });
    /*--------------------------------------------------------------------------------------------------------------*/
    /* Accessability                                                                                                */
    /*--------------------------------------------------------------------------------------------------------------*/

    // Measures the accessability queries, which every guarded operation performs.
    //
    // NOTE: The accessability of the value is varied from one iteration to the next, so that
    // this body measures the queries as soon as they are work to do. Today they are not: the
    // three 'can_*' helpers of the Variant use '||' where '&&' is required, which leaves every
    // access readable, writable and overwritable, so the queries are constants the compiler
    // folds away and what this body measures is the handling of the operand. The tests pin
    // that down in 'u8/accessability/permissive_helpers', and this number moves the day it is
    // flipped.
    group.bench_function("accessability/queries", [](Bencher &p_bencher) {
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const Variant::Accessability access = static_cast<Variant::Accessability>(counter & 3u);
            const u8 value = Benchmarker::black_box<u8>(u8(counter, access));
            unsigned long long hits = 0ull;
            hits += value.can_read() ? 1ull : 0ull;
            hits += value.can_write() ? 1ull : 0ull;
            hits += value.can_overwrite_access() ? 1ull : 0ull;
            return hits;
        });
    });

    // Measures the cost of the guarded 'set', which is a guarded write and therefore pays for
    // the accessability checks before it stores anything.
    //
    // NOTE: As in 'accessability/queries', the accessability is varied so that the body
    // measures the guard as soon as there is one to measure. Until the 'can_*' helpers are
    // fixed this is an ordinary store, which is what the number says.
    group.bench_function("accessability/guarded_set", [](Bencher &p_bencher) {
        unsigned char counter = 0;
        p_bencher.iter([&] {
            counter = (unsigned char)(counter + 1u);
            const Variant::Accessability access = static_cast<Variant::Accessability>(counter & 3u);
            u8 value = Benchmarker::black_box<u8>(u8(0, access));
            value.set(counter);
            return value;
        });
    });

    group.finish();
}