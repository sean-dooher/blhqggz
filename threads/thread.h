#pragma once

#include <stdint.h>
#include <riscv.h>

#define DEFAULT_PRIORITY 32
#define MIN_PRIORITY 0
#define MAX_PRIORITY 64

#define REGFILE(regfile, register) ((regfile).reg[REGFILE_N(register)])
#define REGFILE_N(register) (regfile_n[register])

static const uint8_t regfile_n[32] = {
    -1, // X0
    0, // RA
    -1, // SP
    -1, // GP
    -1, // TP
    -1, // T0
    10, // T1
    11, // T2
    -1, // S0
    -1, // S1
    1, // A0
    2, // A1
    3, // A2
    4, // A3
    5, // A4
    6, // A5
    7, // A6
    8, // A7
    -1, // S2
    -1, // S3
    -1, // S4
    -1, // S5
    -1, // S6
    -1, // S7
    -1, // S8
    -1, // S9
    -1, // S10
    -1, // S11
    12, // T3
    13, // T4
    14, // T5
    15, // T6
};

typedef struct regfile {
    uint64_t reg[16];
    uint64_t pc;
} regfile_t;


typedef uint32_t tid_t;

/* sizeof thread should be exactly a single page */
typedef struct thread
{
    tid_t id;
    char name[32];

    regfile_t regfile;

    uint8_t priority;
    char stack[1024];
    uint32_t magic;
} thread_t;

typedef void (*thread_func_t) (uint64_t *args, int nargs);

void      thread_init    (void);
thread_t *thread_current (void);

thread_t *thread_create  (char *name, thread_func_t f, uint64_t *args, int nargs);

void      thread_yield   (void);
