/*
 * btest.c - Correctness tester and scorer for the CS:APP Data Lab.
 *
 * Integer puzzles use sampled boundary-heavy domains.  Binary32 puzzles use
 * deterministic special values plus pseudo-random bit patterns.  FP8 E4M3 is
 * tested exhaustively over all 256 encodings.
 */

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "btest.h"
#include "bits.h"

extern student_struct student;
extern test_rec test_set[];

#define MAX_TEST_VALUES 1024
#define RANDOM_INT_VALUES 16
#define RANDOM_FLOAT_VALUES 256

static int grade = 0;
static int self_check = 0;
static int error_limit = 1000;
static char *test_fname = NULL;
static int global_rating = 0;

static int reference_sanity_checks(void)
{
    static const struct {
        unsigned input;
        unsigned expected;
    } fp8_cases[] = {
        {0x00u, 0x00000000u}, {0x80u, 0x80000000u},
        {0x01u, 0x3b000000u}, {0x07u, 0x3c600000u},
        {0x08u, 0x3c800000u}, {0x38u, 0x3f800000u},
        {0x3cu, 0x3fc00000u}, {0x77u, 0x43700000u},
        {0x7eu, 0x43e00000u}, {0xfeu, 0xc3e00000u},
        {0x7fu, 0x7fc00000u}, {0xffu, 0x7fc00000u}
    };
    static const struct {
        int input;
        unsigned expected;
    } power_cases[] = {
        {-150, 0x00000000u}, {-149, 0x00000001u},
        {-127, 0x00400000u}, {-126, 0x00800000u},
        {0, 0x3f800000u}, {127, 0x7f000000u},
        {128, 0x7f800000u}
    };
    int failures = 0;
    int checks = 0;
    int i;

    for (i = 0; i < (int)(sizeof(fp8_cases) / sizeof(fp8_cases[0])); i++) {
        unsigned actual = test_fp8E4M3ToFloat(fp8_cases[i].input);
        checks++;
        if (actual != fp8_cases[i].expected) {
            fprintf(stderr,
                    "btest: FP8 reference check failed for 0x%02x: "
                    "0x%08x != 0x%08x\n",
                    fp8_cases[i].input, actual, fp8_cases[i].expected);
            failures++;
        }
    }

    for (i = 0; i < (int)(sizeof(power_cases) / sizeof(power_cases[0])); i++) {
        unsigned actual = test_floatPower2(power_cases[i].input);
        checks++;
        if (actual != power_cases[i].expected) {
            fprintf(stderr,
                    "btest: floatPower2 reference check failed for %d: "
                    "0x%08x != 0x%08x\n",
                    power_cases[i].input, actual, power_cases[i].expected);
            failures++;
        }
    }

    checks += 7;
    failures += test_bang(-5) != (int)0x80000005u;
    failures += test_floatAbsVal(0xbf800000u) != 0x3f800000u;
    failures += test_floatAbsVal(0xffc12345u) != 0xffc12345u;
    failures += test_floatScale2(0x00400000u) != 0x00800000u;
    failures += test_floatScale2(0x7f7fffffu) != 0x7f800000u;
    failures += test_floatFloat2Int(0x3f800000u) != 1;
    failures += test_floatFloat2Int(0xcf000000u) != INT_MIN;

    if (failures == 0)
        printf("Reference sanity checks: %d passed.\n", checks);
    else
        fprintf(stderr, "btest: %d reference sanity checks failed\n", failures);
    return failures;
}

static uint32_t next_random(uint32_t *state)
{
    *state = *state * UINT32_C(1664525) + UINT32_C(1013904223);
    return *state;
}

static void append_unique(uint32_t values[], int *count, uint32_t value)
{
    int i;

    for (i = 0; i < *count; i++) {
        if (values[i] == value)
            return;
    }
    if (*count >= MAX_TEST_VALUES) {
        fprintf(stderr, "btest: internal test-value capacity exceeded\n");
        exit(2);
    }
    values[(*count)++] = value;
}

static int in_int_range(int value, int minimum, int maximum)
{
    return value >= minimum && value <= maximum;
}

static int gen_int_range(uint32_t values[], int minimum, int maximum)
{
    static const int important[] = {
        INT_MIN, INT_MIN + 1, -1073741824, -65536, -256, -128,
        -33, -32, -31, -16, -8, -5, -4, -3, -2, -1,
        0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 31, 32, 33,
        127, 128, 255, 256, 65535, 65536, 1073741823,
        1073741824, INT_MAX - 1, INT_MAX
    };
    int64_t span = (int64_t)maximum - (int64_t)minimum;
    uint32_t state = UINT32_C(0x15213);
    int count = 0;
    int i;

    if (span <= 32) {
        int64_t value;
        for (value = minimum; value <= maximum; value++)
            append_unique(values, &count, (uint32_t)(int32_t)value);
        return count;
    }

    for (i = 0; i < 5; i++) {
        append_unique(values, &count, (uint32_t)(minimum + i));
        append_unique(values, &count, (uint32_t)(maximum - i));
    }

    for (i = 0; i < (int)(sizeof(important) / sizeof(important[0])); i++) {
        if (in_int_range(important[i], minimum, maximum))
            append_unique(values, &count, (uint32_t)important[i]);
    }

    for (i = 0; i < RANDOM_INT_VALUES; i++) {
        uint64_t width = (uint64_t)span + 1u;
        int64_t value = (int64_t)minimum + (next_random(&state) % width);
        append_unique(values, &count, (uint32_t)(int32_t)value);
    }

    return count;
}

static int gen_int_exhaustive(uint32_t values[], int minimum, int maximum)
{
    int64_t span = (int64_t)maximum - (int64_t)minimum + 1;
    int count = 0;
    int64_t value;

    if (span < 0 || span > MAX_TEST_VALUES) {
        fprintf(stderr, "btest: exhaustive integer domain is too large\n");
        exit(2);
    }

    for (value = minimum; value <= maximum; value++)
        values[count++] = (uint32_t)(int32_t)value;
    return count;
}

static int gen_float32_bits(uint32_t values[])
{
    static const uint32_t special[] = {
        UINT32_C(0x00000000), UINT32_C(0x80000000),
        UINT32_C(0x00000001), UINT32_C(0x80000001),
        UINT32_C(0x00000002), UINT32_C(0x80000002),
        UINT32_C(0x003fffff), UINT32_C(0x803fffff),
        UINT32_C(0x00400000), UINT32_C(0x80400000),
        UINT32_C(0x007ffffe), UINT32_C(0x807ffffe),
        UINT32_C(0x007fffff), UINT32_C(0x807fffff),
        UINT32_C(0x00800000), UINT32_C(0x80800000),
        UINT32_C(0x00800001), UINT32_C(0x80800001),
        UINT32_C(0x3f000000), UINT32_C(0xbf000000),
        UINT32_C(0x3f7fffff), UINT32_C(0xbf7fffff),
        UINT32_C(0x3f800000), UINT32_C(0xbf800000),
        UINT32_C(0x3f800001), UINT32_C(0xbf800001),
        UINT32_C(0x3fffffff), UINT32_C(0xbfffffff),
        UINT32_C(0x40000000), UINT32_C(0xc0000000),
        UINT32_C(0x4effffff), UINT32_C(0xceffffff),
        UINT32_C(0x4f000000), UINT32_C(0xcf000000),
        UINT32_C(0x4f000001), UINT32_C(0xcf000001),
        UINT32_C(0x7f7fffff), UINT32_C(0xff7fffff),
        UINT32_C(0x7f800000), UINT32_C(0xff800000),
        UINT32_C(0x7f800001), UINT32_C(0xff800001),
        UINT32_C(0x7fc00000), UINT32_C(0xffc00000),
        UINT32_C(0x7fffffff), UINT32_C(0xffffffff)
    };
    static const unsigned exponents[] = {
        0x00u, 0x01u, 0x7du, 0x7eu, 0x7fu, 0x9du, 0x9eu,
        0x9fu, 0xfeu, 0xffu
    };
    static const unsigned fractions[] = {
        0u, 1u, 0x003fffffu, 0x00400000u, 0x007fffffu
    };
    uint32_t state = UINT32_C(0xdad1ab);
    int count = 0;
    int i;
    int e;
    int f;
    int s;

    for (i = 0; i < (int)(sizeof(special) / sizeof(special[0])); i++)
        append_unique(values, &count, special[i]);

    for (s = 0; s < 2; s++) {
        for (e = 0; e < (int)(sizeof(exponents) / sizeof(exponents[0])); e++) {
            for (f = 0; f < (int)(sizeof(fractions) / sizeof(fractions[0])); f++) {
                uint32_t value = ((uint32_t)s << 31) |
                    ((uint32_t)exponents[e] << 23) | fractions[f];
                append_unique(values, &count, value);
            }
        }
    }

    for (i = 0; i < RANDOM_FLOAT_VALUES; i++)
        append_unique(values, &count, next_random(&state));

    return count;
}

static int gen_uint8_exhaustive(uint32_t values[])
{
    int i;
    for (i = 0; i < 256; i++)
        values[i] = (uint32_t)i;
    return 256;
}

static int generate_values(uint32_t values[], arg_domain domain,
                           int minimum, int maximum)
{
    switch (domain) {
    case ARG_INT_RANGE:
        return gen_int_range(values, minimum, maximum);
    case ARG_INT_EXHAUSTIVE:
        return gen_int_exhaustive(values, minimum, maximum);
    case ARG_FLOAT32_BITS:
        return gen_float32_bits(values);
    case ARG_UINT8_EXHAUSTIVE:
        return gen_uint8_exhaustive(values);
    default:
        fprintf(stderr, "btest: unknown argument domain\n");
        exit(2);
    }
}

static uint32_t call_function(function_ref function, test_signature signature,
                              const uint32_t args[3])
{
    switch (signature) {
    case SIG_INT_VOID:
        return (uint32_t)function.int_void();
    case SIG_INT_INT:
        return (uint32_t)function.int_int((int32_t)args[0]);
    case SIG_INT_INT_INT:
        return (uint32_t)function.int_int_int(
            (int32_t)args[0], (int32_t)args[1]);
    case SIG_INT_INT_INT_INT:
        return (uint32_t)function.int_int_int_int(
            (int32_t)args[0], (int32_t)args[1], (int32_t)args[2]);
    case SIG_UINT_UINT:
        return function.uint_uint(args[0]);
    case SIG_INT_UINT:
        return (uint32_t)function.int_uint(args[0]);
    case SIG_UINT_INT:
        return function.uint_int((int32_t)args[0]);
    case SIG_UINT_UINT_UINT:
        return function.uint_uint_uint(args[0], args[1]);
    default:
        fprintf(stderr, "btest: unknown function signature\n");
        exit(2);
    }
}

static int returns_unsigned(test_signature signature)
{
    return signature == SIG_UINT_UINT || signature == SIG_UINT_INT ||
        signature == SIG_UINT_UINT_UINT;
}

static void print_argument(uint32_t value, arg_domain domain)
{
    if (domain == ARG_FLOAT32_BITS)
        printf("0x%08x", value);
    else if (domain == ARG_UINT8_EXHAUSTIVE)
        printf("0x%02x", value);
    else
        printf("%d[0x%08x]", (int32_t)value, value);
}

static void report_failure(test_ptr test, const uint32_t args[3],
                           uint32_t result, uint32_t expected)
{
    int i;

    printf("Test %s(", test->name);
    for (i = 0; i < test->args; i++) {
        if (i > 0)
            printf(", ");
        print_argument(args[i], test->arg_domains[i]);
    }
    printf(") failed.\n");

    if (returns_unsigned(test->signature)) {
        printf("  Gives 0x%08x (%u).\n", result, result);
        printf("  Should be 0x%08x (%u).\n", expected, expected);
    } else {
        printf("  Gives %d[0x%08x].\n", (int32_t)result, result);
        printf("  Should be %d[0x%08x].\n", (int32_t)expected, expected);
    }
}

static int test_function(test_ptr test, int report)
{
    uint32_t test_values[3][MAX_TEST_VALUES];
    int test_counts[3] = {1, 1, 1};
    function_ref solution = self_check ? test->test_funct : test->solution_funct;
    int errors = 0;
    int limits[3];
    int i;
    int a1;
    int a2;
    int a3;

    for (i = 0; i < test->args; i++) {
        test_counts[i] = generate_values(
            test_values[i], test->arg_domains[i],
            test->arg_ranges[i][0], test->arg_ranges[i][1]);
    }
    for (i = test->args; i < 3; i++)
        test_values[i][0] = 0u;

    limits[0] = test->args > 0 ? test_counts[0] : 1;
    limits[1] = test->args > 1 ? test_counts[1] : 1;
    limits[2] = test->args > 2 ? test_counts[2] : 1;

    for (a1 = 0; a1 < limits[0]; a1++) {
        for (a2 = 0; a2 < limits[1]; a2++) {
            for (a3 = 0; a3 < limits[2]; a3++) {
                uint32_t args[3] = {
                    test_values[0][a1], test_values[1][a2], test_values[2][a3]
                };
                uint32_t result = call_function(solution, test->signature, args);
                uint32_t expected = call_function(
                    test->test_funct, test->signature, args);

                if (result != expected) {
                    if (report && errors < error_limit)
                        report_failure(test, args, result, expected);
                    errors++;
                }
            }
        }
    }

    if (!grade && report && errors > error_limit)
        printf("... %d total errors for function %s\n", errors, test->name);
    return errors;
}

static int run_tests(int report)
{
    double points = 0.0;
    double max_points = 0.0;
    int errors = 0;
    int matched = 0;
    int i;

    if (grade)
        printf("Score\tErrors\tFunction\n");

    for (i = 0; test_set[i].name != NULL; i++) {
        test_ptr test = &test_set[i];

        if (test_fname == NULL || strcmp(test->name, test_fname) == 0) {
            int rating = global_rating ? global_rating : test->rating;
            int test_errors = test_function(test, report);
            double score = test_errors == 0 ? 1.0 : test_errors == 1 ? 0.5 : 0.0;
            double test_points = rating * score;

            matched++;
            errors += test_errors;
            points += test_points;
            max_points += rating;

            if (grade)
                printf(" %.1f\t%d\t%s\n", test_points, test_errors, test->name);
            if (report)
                printf("Test %s score: %.2f/%.2f\n",
                       test->name, test_points, (double)rating);
        }
    }

    if (matched == 0) {
        fprintf(stderr, "btest: unknown function '%s'\n", test_fname);
        return -1;
    }

    if (grade)
        printf("Total points: %.2f/%.2f\n", points, max_points);
    else
        printf("Overall correctness score: %.2f/%.2f\n", points, max_points);
    return errors;
}

static void usage(char *command, int status)
{
    printf("Usage: %s [-v 0|1] [-haSg] [-f <func>] [-e <errors>]\n", command);
    printf("  -e <n>    Limit errors reported for each function to n\n");
    printf("  -f <name> Test only the named function\n");
    printf("  -g        Print compact grading summary\n");
    printf("  -h        Print this message\n");
    printf("  -a        Omit the student-information check\n");
    printf("  -S        Self-check the grader using its reference functions\n");
    printf("  -r <n>    Give every problem the same weight n\n");
    printf("  -v <n>    Verbosity: 0 for final score, 1 for details\n");
    exit(status);
}

int main(int argc, char *argv[])
{
    int verbose_level = 1;
    int student_check = 1;
    int errors;
    int option;

    while ((option = getopt(argc, argv, "haSgv:f:e:r:")) != -1) {
        switch (option) {
        case 'h':
            usage(argv[0], 0);
            break;
        case 'a':
            student_check = 0;
            break;
        case 'S':
            self_check = 1;
            student_check = 0;
            break;
        case 'g':
            grade = 1;
            break;
        case 'v':
            verbose_level = atoi(optarg);
            if (verbose_level < 0 || verbose_level > 1)
                usage(argv[0], 1);
            break;
        case 'f':
            test_fname = optarg;
            break;
        case 'e':
            error_limit = atoi(optarg);
            if (error_limit < 0)
                usage(argv[0], 1);
            break;
        case 'r':
            global_rating = atoi(optarg);
            if (global_rating < 0)
                usage(argv[0], 1);
            break;
        default:
            usage(argv[0], 1);
        }
    }

    if (grade) {
        error_limit = 0;
        verbose_level = 0;
    }

    if (self_check) {
        printf("Grader self-check: using reference functions as submissions.\n");
        if (reference_sanity_checks() != 0)
            return 2;
    }

    if (student_check) {
        if (*student.name == '\0' || *student.student_id == '\0') {
            fprintf(stderr,
                    "btest: fill in your name and student ID in bits.c\n");
            return 2;
        }
        printf("Student: %s\n", student.name);
        printf("Student ID: %s\n", student.student_id);
        printf("\n");
    }

    errors = run_tests(verbose_level > 0);
    if (errors < 0)
        return 2;

    if (!grade) {
        if (errors > 0)
            printf("%d errors encountered.\n", errors);
        else
            printf("All tests passed.\n");
    }

    return errors == 0 ? 0 : 1;
}
