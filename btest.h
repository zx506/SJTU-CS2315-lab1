/* CS:APP Data Lab test-harness declarations. */

#ifndef DATALAB_BTEST_H
#define DATALAB_BTEST_H

typedef struct {
    char *name;
    char *student_id;
} student_struct;

typedef enum {
    SIG_INT_VOID = 0,
    SIG_INT_INT,
    SIG_INT_INT_INT,
    SIG_INT_INT_INT_INT,
    SIG_UINT_UINT,
    SIG_INT_UINT,
    SIG_UINT_INT,
    SIG_UINT_UINT_UINT
} test_signature;

/* Keep every supported function-pointer type explicit. */
typedef union {
    int (*int_void)(void);
    int (*int_int)(int);
    int (*int_int_int)(int, int);
    int (*int_int_int_int)(int, int, int);
    unsigned (*uint_uint)(unsigned);
    int (*int_uint)(unsigned);
    unsigned (*uint_int)(int);
    unsigned (*uint_uint_uint)(unsigned, unsigned);
} function_ref;

typedef enum {
    ARG_INT_RANGE = 0,
    ARG_INT_EXHAUSTIVE,
    ARG_FLOAT32_BITS,
    ARG_UINT8_EXHAUSTIVE
} arg_domain;

typedef struct {
    char *name;
    function_ref solution_funct;
    function_ref test_funct;
    int args;
    char *ops;
    int op_limit;
    int rating;
    int arg_ranges[3][2];
    test_signature signature;
    arg_domain arg_domains[3];
} test_rec, *test_ptr;

extern test_rec test_set[];

#endif
