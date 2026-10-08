/* Puzzle metadata consumed by btest. */

#include <limits.h>
#include <stddef.h>

#include "btest.h"
#include "bits.h"

#define TMin INT_MIN
#define TMax INT_MAX

test_rec test_set[] = {
    {"bang", {.int_int = bang}, {.int_int = test_bang}, 1,
     "! ~ & ^ | + << >>", 12, 4,
     {{TMin + 1, TMax}, {0, 0}, {0, 0}},
     SIG_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"dividePower2", {.int_int_int = dividePower2},
     {.int_int_int = test_dividePower2}, 2,
     "! ~ & ^ | + << >>", 15, 3,
     {{TMin, TMax}, {0, 30}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"bitCount", {.int_int = bitCount}, {.int_int = test_bitCount}, 1,
     "! ~ & ^ | + << >>", 40, 4,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"fitsBits", {.int_int_int = fitsBits}, {.int_int_int = test_fitsBits}, 2,
     "! ~ & ^ | + << >>", 15, 2,
     {{TMin, TMax}, {1, 32}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"getByte", {.int_int_int = getByte}, {.int_int_int = test_getByte}, 2,
     "! ~ & ^ | + << >>", 6, 2,
     {{TMin, TMax}, {0, 3}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"isGreater", {.int_int_int = isGreater}, {.int_int_int = test_isGreater}, 2,
     "! ~ & ^ | + << >>", 24, 3,
     {{TMin, TMax}, {TMin, TMax}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"isNonNegative", {.int_int = isNonNegative},
     {.int_int = test_isNonNegative}, 1,
     "! ~ & ^ | + << >>", 6, 3,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"logicalShift", {.int_int_int = logicalShift},
     {.int_int_int = test_logicalShift}, 2,
     "~ & ^ | + << >>", 16, 3,
     {{TMin, TMax}, {1, 31}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"satAdd", {.int_int_int = satAdd}, {.int_int_int = test_satAdd}, 2,
     "! ~ & ^ | + << >>", 30, 4,
     {{TMin, TMax}, {TMin, TMax}, {0, 0}},
     SIG_INT_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"logicalNeg", {.int_int = logicalNeg}, {.int_int = test_logicalNeg}, 1,
     "~ & ^ | + << >>", 12, 4,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_INT_INT, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"floatAbsVal", {.uint_uint = floatAbsVal},
     {.uint_uint = test_floatAbsVal}, 1,
     "integer/unsigned operations", 10, 2,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_UINT_UINT, {ARG_FLOAT32_BITS, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"floatScale2", {.uint_uint = floatScale2},
     {.uint_uint = test_floatScale2}, 1,
     "integer/unsigned operations", 30, 4,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_UINT_UINT, {ARG_FLOAT32_BITS, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"floatFloat2Int", {.int_uint = floatFloat2Int},
     {.int_uint = test_floatFloat2Int}, 1,
     "integer/unsigned operations", 30, 4,
     {{TMin, TMax}, {0, 0}, {0, 0}},
     SIG_INT_UINT, {ARG_FLOAT32_BITS, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"floatPower2", {.uint_int = floatPower2},
     {.uint_int = test_floatPower2}, 1,
     "integer/unsigned operations", 30, 4,
     {{-200, 200}, {0, 0}, {0, 0}},
     SIG_UINT_INT, {ARG_INT_EXHAUSTIVE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {"fp8E4M3ToFloat", {.uint_uint = fp8E4M3ToFloat},
     {.uint_uint = test_fp8E4M3ToFloat}, 1,
     "integer/unsigned operations", 40, 4,
     {{0, 255}, {0, 0}, {0, 0}},
     SIG_UINT_UINT, {ARG_UINT8_EXHAUSTIVE, ARG_INT_RANGE, ARG_INT_RANGE}},

    {NULL, {.int_void = NULL}, {.int_void = NULL}, 0, NULL, 0, 0,
     {{0, 0}, {0, 0}, {0, 0}},
     SIG_INT_VOID, {ARG_INT_RANGE, ARG_INT_RANGE, ARG_INT_RANGE}}
};
