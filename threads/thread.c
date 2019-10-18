#include "thread.h"

thread_t *
thread_current (void)
{
    return 0x0;
}


void
thread_init (void)
{

}


thread_t *
thread_create (char *name, thread_func_t f, uint64_t *args, int nargs)
{
    return 0;
}

void
thread_yield (void)
{

}


static void
thread_idle (void) 
{
    WFI();
}
