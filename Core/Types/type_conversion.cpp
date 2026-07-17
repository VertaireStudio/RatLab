/************************************/
/*       type_conversion.hpp        */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "array.hpp"
#include "bool.hpp"
#include "f32.hpp"
#include "f64.hpp"
#include "i8.hpp"
#include "i16.hpp"
#include "i32.hpp"
#include "i64.hpp"
#include "string.hpp"
#include "u8.hpp"
#include "u16.hpp"
#include "u32.hpp"
#include "u64.hpp"
#include "vector.hpp"

// TODO: Make conversion for Variant types

// Conversions between types

func Bool::operator u8()  { return Bool(value); }
func Bool::operator u16() { return Bool(value); }
func Bool::operator u32() { return Bool(value); }
func Bool::operator u64() { return Bool(value); }
func Bool::operator i8()  { return Bool(value); }
func Bool::operator i16() { return Bool(value); }
func Bool::operator i32() { return Bool(value); }
func Bool::operator i64() { return Bool(value); }
func Bool::operator f32() { return Bool(value); }
func Bool::operator f64() { return Bool(value); }

func u8::operator Bool()  { return u8(value); }
func u8::operator u16() { return u8(value); }
func u8::operator u32() { return u8(value); }
func u8::operator u64() { return u8(value); }
func u8::operator i8()  { return u8(value); }
func u8::operator i16() { return u8(value); }
func u8::operator i32() { return u8(value); }
func u8::operator i64() { return u8(value); }
func u8::operator f32() { return u8(value); }
func u8::operator f64() { return u8(value); }

func u16::operator Bool()  { return u16(value); }
func u16::operator u8() { return u16(value); }
func u16::operator u32() { return u16(value); }
func u16::operator u64() { return u16(value); }
func u16::operator i8()  { return u16(value); }
func u16::operator i16() { return u16(value); }
func u16::operator i32() { return u16(value); }
func u16::operator i64() { return u16(value); }
func u16::operator f32() { return u16(value); }
func u16::operator f64() { return u16(value); }

func u32::operator Bool()  { return u32(value); }
func u32::operator u8() { return u32(value); }
func u32::operator u16() { return u32(value); }
func u32::operator u64() { return u32(value); }
func u32::operator i8()  { return u32(value); }
func u32::operator i16() { return u32(value); }
func u32::operator i32() { return u32(value); }
func u32::operator i64() { return u32(value); }
func u32::operator f32() { return u32(value); }
func u32::operator f64() { return u32(value); }

func u64::operator Bool()  { return u64(value); }
func u64::operator u8() { return u64(value); }
func u64::operator u16() { return u64(value); }
func u64::operator u32() { return u64(value); }
func u64::operator i8()  { return u64(value); }
func u64::operator i16() { return u64(value); }
func u64::operator i32() { return u64(value); }
func u64::operator i64() { return u64(value); }
func u64::operator f32() { return u64(value); }
func u64::operator f64() { return u64(value); }

func i8::operator Bool()  { return i8(value); }
func i8::operator u8() { return i8(value); }
func i8::operator u16() { return i8(value); }
func i8::operator u32() { return i8(value); }
func i8::operator u64()  { return i8(value); }
func i8::operator i16() { return i8(value); }
func i8::operator i32() { return i8(value); }
func i8::operator i64() { return i8(value); }
func i8::operator f32() { return i8(value); }
func i8::operator f64() { return i8(value); }

func i16::operator Bool()  { return i16(value); }
func i16::operator u8() { return i16(value); }
func i16::operator u16() { return i16(value); }
func i16::operator u32() { return i16(value); }
func i16::operator u64()  { return i16(value); }
func i16::operator i8() { return i16(value); }
func i16::operator i32() { return i16(value); }
func i16::operator i64() { return i16(value); }
func i16::operator f32() { return i16(value); }
func i16::operator f64() { return i16(value); }

func i32::operator Bool()  { return i32(value); }
func i32::operator u8() { return i32(value); }
func i32::operator u16() { return i32(value); }
func i32::operator u32() { return i32(value); }
func i32::operator u64()  { return i32(value); }
func i32::operator i8() { return i32(value); }
func i32::operator i16() { return i32(value); }
func i32::operator i64() { return i32(value); }
func i32::operator f32() { return i32(value); }
func i32::operator f64() { return i32(value); }

func i64::operator Bool()  { return i64(value); }
func i64::operator u8() { return i64(value); }
func i64::operator u16() { return i64(value); }
func i64::operator u32() { return i64(value); }
func i64::operator u64()  { return i64(value); }
func i64::operator i8() { return i64(value); }
func i64::operator i16() { return i64(value); }
func i64::operator i32() { return i64(value); }
func i64::operator f32() { return i64(value); }
func i64::operator f64() { return i64(value); }

func f32::operator Bool()  { return f32(value); }
func f32::operator u8() { return f32(value); }
func f32::operator u16() { return f32(value); }
func f32::operator u32() { return f32(value); }
func f32::operator u64()  { return f32(value); }
func f32::operator i8() { return f32(value); }
func f32::operator i16() { return f32(value); }
func f32::operator i32() { return f32(value); }
func f32::operator i64() { return f32(value); }
func f32::operator f64() { return f32(value); }

func f64::operator Bool()  { return f64(value); }
func f64::operator u8() { return f64(value); }
func f64::operator u16() { return f64(value); }
func f64::operator u32() { return f64(value); }
func f64::operator u64()  { return f64(value); }
func f64::operator i8() { return f64(value); }
func f64::operator i16() { return f64(value); }
func f64::operator i32() { return f64(value); }
func f64::operator i64() { return f64(value); }
func f64::operator f32() { return f64(value); }