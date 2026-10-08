/* Function declarations for the CS:APP Data Lab test harness. */

#ifndef DATALAB_BITS_H
#define DATALAB_BITS_H

/* Integer puzzles. */
int bang(int x);
int dividePower2(int x, int n);
int bitCount(int x);
int fitsBits(int x, int n);
int getByte(int x, int n);
int isGreater(int x, int y);
int isNonNegative(int x);
int logicalShift(int x, int n);
int satAdd(int x, int y);
int logicalNeg(int x);

/* Floating-point and AI-format puzzles. */
unsigned floatAbsVal(unsigned uf);
unsigned floatScale2(unsigned uf);
int floatFloat2Int(unsigned uf);
unsigned floatPower2(int x);
unsigned fp8E4M3ToFloat(unsigned u8);

/* Reference functions used by btest. */
int test_bang(int x);
int test_dividePower2(int x, int n);
int test_bitCount(int x);
int test_fitsBits(int x, int n);
int test_getByte(int x, int n);
int test_isGreater(int x, int y);
int test_isNonNegative(int x);
int test_logicalShift(int x, int n);
int test_satAdd(int x, int y);
int test_logicalNeg(int x);
unsigned test_floatAbsVal(unsigned uf);
unsigned test_floatScale2(unsigned uf);
int test_floatFloat2Int(unsigned uf);
unsigned test_floatPower2(int x);
unsigned test_fp8E4M3ToFloat(unsigned u8);

#endif
