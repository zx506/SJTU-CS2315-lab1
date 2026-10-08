/* Reference implementations used by btest.  These functions express the
 * required semantics and are not subject to the student coding rules. */

#include <limits.h>
#include <stdint.h>

#include "bits.h"

int test_bang(int x)
{
    int sign = x < 0;
    int magnitude = x < 0 ? -x : x;
    return (int)(((unsigned)sign << 31) | (unsigned)magnitude);
}

int test_dividePower2(int x, int n)
{
    return x / (1 << n);
}

int test_bitCount(int x)
{
    unsigned value = (unsigned)x;
    int result = 0;
    int i;

    for (i = 0; i < 32; i++)
        result += (value >> i) & 1u;
    return result;
}

int test_fitsBits(int x, int n)
{
    int64_t minimum = -(INT64_C(1) << (n - 1));
    int64_t maximum = (INT64_C(1) << (n - 1)) - 1;
    int64_t value = x;

    return value >= minimum && value <= maximum;
}

int test_getByte(int x, int n)
{
    return (int)(((unsigned)x >> (n << 3)) & 0xffu);
}

int test_isGreater(int x, int y)
{
    return x > y;
}

int test_isNonNegative(int x)
{
    return x >= 0;
}

int test_logicalShift(int x, int n)
{
    return (int)((unsigned)x >> n);
}

int test_satAdd(int x, int y)
{
    int64_t sum = (int64_t)x + (int64_t)y;

    if (sum > INT_MAX)
        return INT_MAX;
    if (sum < INT_MIN)
        return INT_MIN;
    return (int)sum;
}

int test_logicalNeg(int x)
{
    return !x;
}

unsigned test_floatAbsVal(unsigned uf)
{
    unsigned magnitude = uf & 0x7fffffffu;

    if (magnitude > 0x7f800000u)
        return uf;
    return magnitude;
}

unsigned test_floatScale2(unsigned uf)
{
    unsigned sign = uf & 0x80000000u;
    unsigned exponent = (uf >> 23) & 0xffu;
    unsigned fraction = uf & 0x007fffffu;

    if (exponent == 0xffu)
        return uf;
    if (exponent == 0u)
        return sign | ((uf & 0x7fffffffu) << 1);

    exponent++;
    if (exponent == 0xffu)
        fraction = 0u;
    return sign | (exponent << 23) | fraction;
}

int test_floatFloat2Int(unsigned uf)
{
    unsigned sign = uf >> 31;
    unsigned exponent = (uf >> 23) & 0xffu;
    unsigned fraction = uf & 0x007fffffu;
    int unbiased = (int)exponent - 127;
    uint64_t magnitude;
    int64_t result;

    if (exponent == 0xffu || unbiased > 30)
        return (int)0x80000000u;
    if (unbiased < 0)
        return 0;

    magnitude = (uint64_t)(fraction | 0x00800000u);
    if (unbiased > 23)
        magnitude <<= unbiased - 23;
    else
        magnitude >>= 23 - unbiased;

    result = sign ? -(int64_t)magnitude : (int64_t)magnitude;
    if (result < INT_MIN || result > INT_MAX)
        return (int)0x80000000u;
    return (int)result;
}

unsigned test_floatPower2(int x)
{
    if (x < -149)
        return 0u;
    if (x < -126)
        return 1u << (x + 149);
    if (x <= 127)
        return (unsigned)(x + 127) << 23;
    return 0x7f800000u;
}

unsigned test_fp8E4M3ToFloat(unsigned u8)
{
    unsigned value = u8 & 0xffu;
    unsigned sign = (value & 0x80u) << 24;
    unsigned exponent = (value >> 3) & 0x0fu;
    unsigned fraction = value & 0x07u;
    unsigned exponent32;
    unsigned fraction32;
    int leading_bit;

    /* OCP E4M3 has two NaN encodings and no infinity encodings. */
    if (exponent == 0x0fu && fraction == 0x07u)
        return 0x7fc00000u;

    if (exponent != 0u) {
        exponent32 = exponent + 120u; /* (exponent - 7) + 127 */
        fraction32 = fraction << 20;
        return sign | (exponent32 << 23) | fraction32;
    }

    if (fraction == 0u)
        return sign;

    if (fraction & 0x04u)
        leading_bit = 2;
    else if (fraction & 0x02u)
        leading_bit = 1;
    else
        leading_bit = 0;

    /* An E4M3 subnormal has value fraction * 2^-9. */
    exponent32 = (unsigned)(118 + leading_bit);
    fraction32 = (fraction << (23 - leading_bit)) & 0x007fffffu;
    return sign | (exponent32 << 23) | fraction32;
}
