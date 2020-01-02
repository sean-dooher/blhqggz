#pragma once

#include <stdint.h>
#include <riscv.h>

#define DEFAULT_PRIORITY 32
#define MIN_PRIORITY 0
#define MAX_PRIORITY 64

#define REGFILE(regfile, register) ((regfile).reg[REGFILE_N(register)])
#define REGFILE_N(register) (register - 1)

typedef struct regfile {
    uint64_t reg[32];
} regfile_t;


typedef uint32_t tid_t;

/**
First page of thread:

0KB
+----------------------------------+
|                                  |
|                                  |
|                                  |
|                 ^                |
|                 |                |
|                 |                |
|                 |                |
|                 |                |
|           Kernel Stack           |
+----------------------------------+
|                                  |
|             thread_t             |
|                                  |
+----------------------------------+
4KB

This design allows for the kernel to allocate more space for its stack if necessary.
Upon context switch away from the thread, the stack pointer is saved with in the thread struct

All registers are dumped onto the kernel stack upon interrupt/exception
**/
typedef struct thread
{
    regfile_t regfile; /* saved user registers for this thread */
    tid_t id;          /* id of this thread */
    char name[32];     /* human readable name for this thread (for debugging) */
    uint8_t priority;  /* priority of this thread (0-255) */
    uint8_t *sp;       /* kernel stack pointer (virtual address) */
} thread_t;

typedef void (*thread_func_t) (void *args);

void      thread_init    (void);
thread_t *thread_current (void);

thread_t *thread_create  (char *name, thread_func_t f, void *args);
void      thread_exit    (void);

void      thread_yield   (void);
void      thread_switch  (thread_t *thread);

uint32_t  thread_get_thread_size (void);
