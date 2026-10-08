/*
 * CS:APP Data Lab 学生作答文件
 *
 * 请在本文件中填写个人信息并完成全部 15 个函数。提交时通常只需提交
 * 本文件。不要引入 <stdio.h>，否则旧版 dlc 可能无法正确分析代码。
 */

#include "btest.h"
#include <limits.h>

/*
 * 个人信息
 *
 * 本实验必须由每位学生独立完成。请填写本人的姓名和学号，两个字段
 * 都不能为空。
 */
student_struct student =
{
    /* 姓名 */
    "",
    /* 学号 */
    ""
};

/*
 * 整数题统一编码规则
 *
 * 每个整数题只能使用：
 *   1. 0 到 255（0xFF）之间的整数常量；
 *   2. 函数参数和 int 类型的局部变量；
 *   3. 一元运算符 ! 和 ~；
 *   4. 二元运算符 &、^、|、+、<< 和 >>。
 *
 * 禁止使用：
 *   1. if、else、for、while、do、switch 等控制结构；
 *   2. 宏、数组、结构体、联合体和全局变量；
 *   3. 新增函数或调用任何函数；
 *   4. &&、||、-、*、/、%、比较运算符和三目运算符；
 *   5. 任何形式的类型转换。
 *
 * 个别题目会进一步限制允许运算符。赋值符号 = 不计入运算符数量。
 * 可以假设 int 为 32 位二进制补码，有符号右移采用算术右移。
 */

/*
 * bitCount - 返回 x 的二进制表示中值为 1 的位数。
 *   示例：bitCount(5) = 2，bitCount(7) = 3
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：40
 *   分值：4
 */
int bitCount(int x) {
  return 2;
}

/*
 * fitsBits - 判断 x 能否使用 n 位二进制补码表示。
 *   输入范围：1 <= n <= 32
 *   示例：fitsBits(5, 3) = 0，fitsBits(-4, 3) = 1
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：15
 *   分值：2
 */
int fitsBits(int x, int n) {
  return 2;
}

/*
 * getByte - 提取 x 的第 n 个字节。
 *   字节从最低有效字节开始编号，编号范围为 0 到 3。
 *   示例：getByte(0x12345678, 1) = 0x56
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：6
 *   分值：2
 */
int getByte(int x, int n) {
  return 2;
}

/*
 * isGreater - 如果 x > y 返回 1，否则返回 0。
 *   示例：isGreater(4, 5) = 0，isGreater(5, 4) = 1
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：24
 *   分值：3
 */
int isGreater(int x, int y) {
  return 2;
}

/*
 * isNonNegative - 如果 x >= 0 返回 1，否则返回 0。
 *   示例：isNonNegative(-1) = 0，isNonNegative(0) = 1
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：6
 *   分值：3
 */
int isNonNegative(int x) {
  return 2;
}

/*
 * logicalShift - 将 x 逻辑右移 n 位。
 *   输入范围：1 <= n <= 31
 *   示例：logicalShift(0x87654321, 4) = 0x08765432
 *   合法运算符：~ & ^ | + << >>
 *   最大运算符数：16
 *   分值：3
 */
int logicalShift(int x, int n) {
  return 2;
}

/*
 * satAdd - 计算饱和加法 x + y。
 *   正溢出时返回最大有符号整数，负溢出时返回最小有符号整数。
 *   示例：satAdd(0x40000000, 0x40000000) = 0x7fffffff
 *         satAdd(0x80000000, 0xffffffff) = 0x80000000
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：30
 *   分值：4
 */
int satAdd(int x, int y) {
  return 2;
}

/*
 * logicalNeg - 不使用 ! 运算符实现逻辑非。
 *   示例：logicalNeg(3) = 0，logicalNeg(0) = 1
 *   合法运算符：~ & ^ | + << >>
 *   最大运算符数：12
 *   分值：4
 */
int logicalNeg(int x) {
  return 2;
}

/*
 * dividePower2 - 计算 x / (2^n)，结果向 0 舍入。
 *   输入范围：0 <= n <= 30
 *   示例：dividePower2(15, 1) = 7，dividePower2(-33, 4) = -2
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：15
 *   分值：3
 */
int dividePower2(int x, int n) {
  return 2;
}

/*
 * bang - 将 32 位二进制补码转换为 sign-magnitude（原码）位模式。
 *   返回值最高位为符号位，低 31 位为 |x|。
 *   可以假设 x != TMin，因为 |TMin| 无法放入 31 位数值字段。
 *   示例：bang(5) = 0x00000005，bang(-5) = 0x80000005
 *   合法运算符：! ~ & ^ | + << >>
 *   最大运算符数：12
 *   分值：4
 */
int bang(int x) {
  return 2;
}

/*
 * 浮点题统一编码规则
 *
 * 以下函数处理浮点数或低精度格式的整数位模式。允许使用 int、unsigned、
 * 任意整数常量、整数运算、位运算、比较和控制结构。
 *
 * 禁止使用浮点类型、浮点常量、浮点运算、类型转换、宏、数组、结构体、
 * 联合体以及任何函数调用。参数和返回值均应作为原始位模式处理。
 */

/*
 * floatAbsVal - 返回 binary32 数值 f 的绝对值所对应的位模式。
 *   uf 是 f 的 32 位表示。如果 uf 表示 NaN，必须原样返回 uf。
 *   合法操作：任意整数或 unsigned 操作以及控制结构
 *   最大运算符数：10
 *   分值：2
 */
unsigned floatAbsVal(unsigned uf) {
  return 2;
}

/*
 * floatScale2 - 返回 2*f 所对应的 binary32 位模式。
 *   uf 是 f 的 32 位表示。如果 uf 表示 NaN，必须原样返回 uf。
 *   需要正确处理零、非规格化数、规格化数和溢出到无穷的情况。
 *   合法操作：任意整数或 unsigned 操作以及控制结构
 *   最大运算符数：30
 *   分值：4
 */
unsigned floatScale2(unsigned uf) {
  return 2;
}

/*
 * floatFloat2Int - 返回将 binary32 数值 f 转换为 int 后的结果。
 *   uf 是 f 的 32 位表示。转换结果向 0 舍入。
 *   对 NaN、无穷或超出 int 范围的数，返回 0x80000000u。
 *   合法操作：任意整数或 unsigned 操作以及控制结构
 *   最大运算符数：30
 *   分值：4
 */
int floatFloat2Int(unsigned uf) {
  return 2;
}

/*
 * floatPower2 - 返回 2.0^x 所对应的 binary32 位模式。
 *   结果小于 binary32 最小非零值时返回正零；结果过大时返回正无穷。
 *   合法操作：任意整数或 unsigned 操作以及控制结构
 *   最大运算符数：30
 *   分值：4
 */
unsigned floatPower2(int x) {
  return 2;
}

/*
 * fp8E4M3ToFloat - 将 OCP FP8 E4M3 编码转换为 IEEE 754 binary32。
 *   u8 的范围为 0 到 255，低 8 位保存一个完整的 E4M3 编码。
 *   必须保留正零和负零。E4M3 没有无穷编码；0x7f 和 0xff 表示 NaN，
 *   此时统一返回 binary32 的规范 NaN 0x7fc00000u。
 *   合法操作：任意整数或 unsigned 操作以及控制结构
 *   最大运算符数：40（旧版 dlc 不检查这道新增题）
 *   分值：4
 */
unsigned fp8E4M3ToFloat(unsigned u8) {
  return 2;
}
