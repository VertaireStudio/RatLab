/************************************/
/*           u8_test.cpp            */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Tester.hpp"
#include "../Core/Types/u8.hpp"

/*--------------------------------------------------------------------------------------------------------------*/
/* Construction and value access                                                                                  */
/*--------------------------------------------------------------------------------------------------------------*/

// A default constructed u8 holds zero.
static void test_default_construction(Tester &p_tester) {
    u8 value;
    p_tester.test_equal(value.get(), (unsigned char)0);
}

// A value constructed u8 holds the given value.
static void test_value_construction(Tester &p_tester) {
    u8 value((unsigned char)200);
    p_tester.test_equal(value.get(), (unsigned char)200);
    p_tester.test_equal(u8(0).get(), (unsigned char)0);
    p_tester.test_equal(u8(255).get(), (unsigned char)255);
}

// 'get_type' reports the u8 entry of the Variant type list.
static void test_get_type(Tester &p_tester) {
    p_tester.test_true(u8::get_type() == Variant::Types::U8);
    p_tester.test_false(u8::get_type() == Variant::Types::U16);
}

// 'set' overwrites the stored value.
static void test_set(Tester &p_tester) {
    u8 value((unsigned char)10);
    value.set((unsigned char)20);
    p_tester.test_equal(value.get(), (unsigned char)20);
}

// 'set' from another u8 copies the value of the source.
static void test_set_from_u8(Tester &p_tester) {
    u8 source((unsigned char)42);
    u8 target;
    target.set(source);
    p_tester.test_equal(target.get(), (unsigned char)42);
}

// Copying a u8 produces an independent value.
static void test_copy(Tester &p_tester) {
    u8 source((unsigned char)7);
    u8 target(source);
    target.set((unsigned char)8);
    p_tester.test_equal(source.get(), (unsigned char)7);
    p_tester.test_equal(target.get(), (unsigned char)8);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Arithmetic                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

// The arithmetic operators produce the expected results.
static void test_arithmetic(Tester &p_tester) {
    p_tester.test_equal((u8(10) + u8(5)).get(), (unsigned char)15);
    p_tester.test_equal((u8(10) - u8(5)).get(), (unsigned char)5);
    p_tester.test_equal((u8(10) * u8(5)).get(), (unsigned char)50);
    p_tester.test_equal((u8(10) / u8(5)).get(), (unsigned char)2);
    p_tester.test_equal((u8(10) % u8(5)).get(), (unsigned char)0);
    p_tester.test_equal((u8(10) + (unsigned char)5).get(), (unsigned char)15);
    p_tester.test_equal((u8(10) - (unsigned char)5).get(), (unsigned char)5);
    p_tester.test_equal((u8(10) * (unsigned char)5).get(), (unsigned char)50);
    p_tester.test_equal((u8(10) / (unsigned char)5).get(), (unsigned char)2);
    p_tester.test_equal((u8(10) % (unsigned char)5).get(), (unsigned char)0);
}

// Division truncates towards zero.
static void test_division_truncates(Tester &p_tester) {
    p_tester.test_equal((u8(100) / u8(7)).get(), (unsigned char)14);
    p_tester.test_equal((u8(100) % u8(7)).get(), (unsigned char)2);
    p_tester.test_equal((u8(7) / u8(100)).get(), (unsigned char)0);
}

// Results wrapping past the maximum value are truncated to 8 bits.
// NOTE: 'u8 % 0' is deliberately not checked here, the modulo operator forwards to the
//       built-in operator, which is undefined behavior for a zero divisor.
static void test_wrap_around(Tester &p_tester) {
    p_tester.test_equal((u8(200) + u8(100)).get(), (unsigned char)44);
    p_tester.test_equal((u8(0) - u8(1)).get(), (unsigned char)255);
    p_tester.test_equal((u8(16) * u8(16)).get(), (unsigned char)0);
    p_tester.test_equal((u8(1) << u8(9)).get(), (unsigned char)0);
}

// The unary operators behave as the built-in unsigned char ones.
static void test_unary(Tester &p_tester) {
    p_tester.test_equal((~u8(0x0F)).get(), (unsigned char)0xF0);
    p_tester.test_equal((~u8(0x00)).get(), (unsigned char)0xFF);
    // Negating an unsigned value wraps around, 'u8(5)' therefore becomes 'u8(251)'.
    p_tester.test_equal((-u8(5)).get(), (unsigned char)251);
}

// The compound assignment operators return the computed value, which truncates to 8 bits.
// NOTE: They do not store that value back, so the left hand operand keeps its original
//       content and every expectation below is calculated from an unchanged 'value'.
static void test_compound_assignment(Tester &p_tester) {
    u8 value((unsigned char)10);
    value += u8(5).get();
    p_tester.test_equal(value.get(), (unsigned char)15);
    value -= u8(3);
    p_tester.test_equal(value.get(), (unsigned char)12);
    value *= u8(3);
    p_tester.test_equal(value.get(), (unsigned char)36);
    value /= u8(6);
    p_tester.test_equal(value.get(), (unsigned char)6);
    value %= u8(3);
    p_tester.test_equal(value.get(), (unsigned char)0);
    value += (unsigned char)10;
    p_tester.test_equal(value.get(), (unsigned char)10);
    value -= (unsigned char)4;
    p_tester.test_equal(value.get(), (unsigned char)6);
    value *= (unsigned char)2;
    p_tester.test_equal(value.get(), (unsigned char)12);
    value /= (unsigned char)6;
    p_tester.test_equal(value.get(), (unsigned char)2);
    value %= (unsigned char)3;
    p_tester.test_equal(value.get(), (unsigned char)2);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Bitwise operations and shifts                                                                                  */
/*--------------------------------------------------------------------------------------------------------------*/

// The bitwise operators produce the expected results.
static void test_bitwise(Tester &p_tester) {
    p_tester.test_equal((u8(0xF0) & u8(0x3C)).get(), (unsigned char)0x30);
    p_tester.test_equal((u8(0xF0) | u8(0x0F)).get(), (unsigned char)0xFF);
    p_tester.test_equal((u8(0xF0) ^ u8(0xFF)).get(), (unsigned char)0x0F);
    p_tester.test_equal((u8(0xF0) & (unsigned char)0x0F).get(), (unsigned char)0x00);
    p_tester.test_equal((u8(0xF0) | (unsigned char)0x0F).get(), (unsigned char)0xFF);
    p_tester.test_equal((u8(0xF0) ^ (unsigned char)0xFF).get(), (unsigned char)0x0F);
}

// The shift operators move the bits as expected, truncating anything above 8 bits.
static void test_shifts(Tester &p_tester) {
    p_tester.test_equal((u8(1) << u8(3)).get(), (unsigned char)8);
    p_tester.test_equal((u8(8) >> u8(3)).get(), (unsigned char)1);
    p_tester.test_equal((u8(1) << (unsigned char)7).get(), (unsigned char)128);
    p_tester.test_equal((u8(128) >> (unsigned char)7).get(), (unsigned char)1);
    p_tester.test_equal((u8(0) << u8(4)).get(), (unsigned char)0);
    p_tester.test_equal((u8(255) >> u8(8)).get(), (unsigned char)0);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Increment, decrement and assignment                                                                            */
/*--------------------------------------------------------------------------------------------------------------*/

// The increment and decrement operators modify the wrapped value.
static void test_increment_and_decrement(Tester &p_tester) {
    u8 value((unsigned char)5);
    ++value;
    p_tester.test_equal(value.get(), (unsigned char)6);
    value++;
    p_tester.test_equal(value.get(), (unsigned char)7);
    --value;
    p_tester.test_equal(value.get(), (unsigned char)6);
    value--;
    p_tester.test_equal(value.get(), (unsigned char)5);

    // Incrementing the maximum value wraps around to zero.
    u8 maximum((unsigned char)255);
    ++maximum;
    p_tester.test_equal(maximum.get(), (unsigned char)0);
    u8 zero;
    --zero;
    p_tester.test_equal(zero.get(), (unsigned char)255);
}

// Assignment overwrites the wrapped value. NOTE: the u8 does not declare an 'operator=' of its
// own, so these go through the implicitly generated copy assignment, which converts the right
// hand side to a u8 first and therefore copies the accessability along with the value.
static void test_assignment(Tester &p_tester) {
    u8 value;
    value = (unsigned char)30;
    p_tester.test_equal(value.get(), (unsigned char)30);
    p_tester.test_equal(value.is_read_and_write(), true);

    value = u8((unsigned char)60, Variant::Accessability::Passive);
    p_tester.test_equal(value.get(), (unsigned char)60);
    p_tester.test_equal(value.is_passive(), true);

    // A u8 assigned from another u8 takes over the value of the source.
    u8 source((unsigned char)90);
    value = source;
    p_tester.test_equal(value.get(), (unsigned char)90);
    p_tester.test_equal(value.get(), source.get());
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Comparisons                                                                                                     */
/*--------------------------------------------------------------------------------------------------------------*/

// The comparison operators order the wrapped values correctly.
static void test_comparisons(Tester &p_tester) {
    p_tester.test_true(u8(5) == u8(5));
    p_tester.test_false(u8(5) == u8(6));
    p_tester.test_true(u8(5) != u8(6));
    p_tester.test_false(u8(5) != u8(5));
    p_tester.test_true(u8(4) < u8(5));
    p_tester.test_true(u8(5) > u8(4));
    p_tester.test_true(u8(5) <= u8(5));
    p_tester.test_true(u8(5) >= u8(5));
    p_tester.test_false(u8(5) < u8(5));
    p_tester.test_false(u8(5) > u8(5));
}

// The comparison operators also accept plain unsigned char values.
static void test_comparisons_with_unsigned_char(Tester &p_tester) {
    p_tester.test_true(u8(5) == (unsigned char)5);
    p_tester.test_false(u8(5) == (unsigned char)6);
    p_tester.test_true(u8(5) != (unsigned char)6);
    p_tester.test_true(u8(4) < (unsigned char)5);
    p_tester.test_true(u8(5) > (unsigned char)4);
    p_tester.test_true(u8(5) <= (unsigned char)5);
    p_tester.test_true(u8(5) >= (unsigned char)5);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Math helpers                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------*/

// 'min' and 'max' return the expected side of both overloads.
static void test_min_max(Tester &p_tester) {
    p_tester.test_equal(u8(5).min(u8(10)).get(), (unsigned char)5);
    p_tester.test_equal(u8(10).min(u8(5)).get(), (unsigned char)5);
    p_tester.test_equal(u8(5).max(u8(10)).get(), (unsigned char)10);
    p_tester.test_equal(u8(10).max(u8(5)).get(), (unsigned char)10);
    p_tester.test_equal(u8(5).min((unsigned char)10).get(), (unsigned char)5);
    p_tester.test_equal(u8(10).min((unsigned char)5).get(), (unsigned char)5);
    p_tester.test_equal(u8(5).max((unsigned char)10).get(), (unsigned char)10);
    p_tester.test_equal(u8(10).max((unsigned char)5).get(), (unsigned char)10);

    // Equal operands are returned as they are.
    p_tester.test_equal(u8(7).min(u8(7)).get(), (unsigned char)7);
    p_tester.test_equal(u8(7).max(u8(7)).get(), (unsigned char)7);
}

// 'clamp' never returns a value below the given minimum.
static void test_clamp(Tester &p_tester) {
    p_tester.test_true(u8(0).clamp(u8(10), u8(20)).get() >= (unsigned char)10);
    p_tester.test_true(u8(25).clamp(u8(10), u8(20)).get() >= (unsigned char)10);
    p_tester.test_true(u8(0).clamp((unsigned char)10, (unsigned char)20).get() >= (unsigned char)10);
    p_tester.test_true(u8(25).clamp((unsigned char)10, (unsigned char)20).get() >= (unsigned char)10);

    // NOTE: 'clamp' resolves as 'min(...).max(...)', so a value which lies inside the range
    //       is pulled up to the maximum. This is the current, documented behavior.
    p_tester.test_equal(u8(15).clamp(u8(10), u8(20)).get(), (unsigned char)20);
    p_tester.test_equal(u8(5).clamp(u8(10), u8(20)).get(), (unsigned char)20);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Accessability                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------*/

// The accessability flags are reported as they were given.
static void test_access_flags(Tester &p_tester) {
    u8 plain;
    p_tester.test_true(plain.is_read_and_write());
    p_tester.test_false(plain.is_read_only());
    p_tester.test_false(plain.is_write_only());
    p_tester.test_false(plain.is_aggressive());
    p_tester.test_false(plain.is_neutral());
    p_tester.test_false(plain.is_passive());

    u8 read_only((unsigned char)1, Variant::Accessability::ReadOnly);
    p_tester.test_true(read_only.is_read_only());

    u8 write_only((unsigned char)1, Variant::Accessability::WriteOnly);
    p_tester.test_true(write_only.is_write_only());

    u8 aggressive((unsigned char)1, Variant::Accessability::Agressive);
    p_tester.test_true(aggressive.is_aggressive());
    // The 'is_*' helpers compare the flag exactly, so an aggressive value is not 'read-only'.
    p_tester.test_false(aggressive.is_read_only());

    u8 neutral((unsigned char)1, Variant::Accessability::Neutral);
    p_tester.test_true(neutral.is_neutral());
    p_tester.test_false(neutral.is_write_only());

    u8 passive((unsigned char)1, Variant::Accessability::Passive);
    p_tester.test_true(passive.is_passive());
    p_tester.test_false(passive.is_read_and_write());
}

// 'set_access' overrides a changeable accessability.
static void test_set_access(Tester &p_tester) {
    u8 value((unsigned char)1);
    value.set_access(Variant::Accessability::ReadOnly);
    p_tester.test_true(value.is_read_only());
    p_tester.test_true(value.can_overwrite_access());

    value.set_access(Variant::Accessability::ReadAndWrite);
    p_tester.test_true(value.is_read_and_write());
    p_tester.test_true(value.can_overwrite_access());
}

// NOTE: The three 'can_*' helpers of the Variant use '||' where '&&' is required, therefore
//       every accessability is currently readable, writable and overwritable. These checks
//       pin that behavior down, so that the expectations below are flipped to 'false' as soon
//       as the Variant helpers are fixed.
static void test_access_helpers_are_permissive(Tester &p_tester) {
    p_tester.test_true(Variant(Variant::ReadOnly).can_read());
    p_tester.test_true(Variant(Variant::ReadOnly).can_write());
    p_tester.test_true(Variant(Variant::WriteOnly).can_read());
    p_tester.test_true(Variant(Variant::WriteOnly).can_write());
    p_tester.test_true(Variant(Variant::Agressive).can_read());
    p_tester.test_true(Variant(Variant::Agressive).can_write());
    p_tester.test_true(Variant(Variant::Neutral).can_read());
    p_tester.test_true(Variant(Variant::Neutral).can_write());
    p_tester.test_true(Variant(Variant::Passive).can_overwrite_access());

    // A read-only value therefore still reports and accepts its value.
    u8 read_only((unsigned char)77, Variant::Accessability::ReadOnly);
    p_tester.test_equal(read_only.get(), (unsigned char)77);
    read_only.set((unsigned char)88);
    p_tester.test_equal(read_only.get(), (unsigned char)88);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Registration                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------*/

// Every test case is registered at static-initialization time, which happens before 'main' runs.
static const Tester::AutoTest register_u8_tests[] = {
    {"u8/construction/default",            &test_default_construction},
    {"u8/construction/value",              &test_value_construction},
    {"u8/type",                             &test_get_type},
    {"u8/access/set",                       &test_set},
    {"u8/access/set_from_u8",               &test_set_from_u8},
    {"u8/access/copy",                      &test_copy},
    {"u8/arithmetic/operators",             &test_arithmetic},
    {"u8/arithmetic/truncation",            &test_division_truncates},
    {"u8/arithmetic/wrap_around",           &test_wrap_around},
    {"u8/arithmetic/unary",                 &test_unary},
    {"u8/arithmetic/compound_assignment",   &test_compound_assignment},
    {"u8/bitwise/operators",                &test_bitwise},
    {"u8/bitwise/shifts",                   &test_shifts},
    {"u8/value/increment_and_decrement",    &test_increment_and_decrement},
    {"u8/value/assignment",                 &test_assignment},
    {"u8/comparison/operators",             &test_comparisons},
    {"u8/comparison/unsigned_char",         &test_comparisons_with_unsigned_char},
    {"u8/math/min_max",                     &test_min_max},
    {"u8/math/clamp",                       &test_clamp},
    {"u8/accessability/flags",              &test_access_flags},
    {"u8/accessability/set_access",         &test_set_access},
    {"u8/accessability/permissive_helpers", &test_access_helpers_are_permissive},
};
