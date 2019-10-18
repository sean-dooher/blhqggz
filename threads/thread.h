#include <stdint.h>

typedef struct regfile {
    uint64_t reg[16];
} regfile_t;


enum {
    A0 = 1,
    A1 = 2,
    A2 = 3
};

typedef uint32_t tid_t;

/* sizeof thread should be exactly a single page */
typedef struct thread
{
    tid_t id;
    char name[32];

    regfile_t regfile;

    uint8_t priority;
    uint32_t magic;
} thread_t;

typedef void (*thread_func_t) (uint64_t *args, int nargs);

void      thread_init    (void);
thread_t *thread_current (void);

thread_t *thread_create  (char *name, thread_func_t f, uint64_t *args, int nargs);

void      thread_yield   (void);
