#include "thread.h"
#include "string.h"

tid_t global_tid = 0;

thread_t *
thread_current (void)
{
    thread_t *pointer; 
    asm volatile (
        "mv %0 tp" : "=r"(pointer)
    );
    return pointer;
}


void
thread_init (void)
{

}


thread_t *
thread_create (char *name, thread_func_t f, uint64_t *args, int nargs)
{
    // thread_t *new_thread = kalloc_page();
    // new_thread->id = global_tid++;
    // strcpy(new_thread->name, name);
    // new_thread->priority = DEFAULT_PRIORITY;
    // REGFILE(new_thread->regfile, SP) = new_thread->stack;
    // new_thread->regfile.pc = f;
    // new_thread->magic = 0xDEADBEEF;
    // return new_thread;
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
