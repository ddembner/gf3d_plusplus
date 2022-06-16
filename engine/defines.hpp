#pragma once

// signed int types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// floating point types
typedef float f32;
typedef double f64;

static_assert(sizeof(i8) == 1, "Expected i8 to be 1");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8");

static_assert(sizeof(u8) == 1, "Expected u8 to be 1");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8");

constexpr u64 TO_KB(const u64& size) { return size * 1024; }
constexpr u64 TO_MB(const u64& size) { return size * 1024 * 1024; }
constexpr u64 TO_GB(const u64& size) { return size * 1024 * 1024 * 1024; }

constexpr u32 INVALID_ID = 0xFFFFFFFF;