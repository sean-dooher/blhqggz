#include <stdint.h>

typedef struct clint_t {
    uint32_t msip[4];
    char UNUSED1[16364];
    uint64_t mtimecmp[4];
    char UNUSED2[32728];
    uint64_t mtime;
} clint_t;


clint_t *CLINT = (clint_t *) 0x200000;

void
clint_init (void)
{
    CLINT->mtime = 0;
    CLINT->mtimecmp[0] = 0;
}

uint64_t
clint_read_mtime (void) {
    return CLINT->mtime;
}