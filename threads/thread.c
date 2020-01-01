#include "thread.h"
#include "string.h"
#include <palloc.h>
#include <stdbool.h>

tid_t global_tid = 0;
static void thread_idle (void **args, int nargs);

thread_t *
thread_current (void)
{
    thread_t *pointer; 
    asm volatile (
        "mv %0, tp" : "=r"(pointer)
    );
    return pointer;
}

void
thread_run (thread_func_t f, void **args, int nargs)
{
    f (args, nargs);
    thread_exit ();
}

void
thread_init (void)
{
    thread_t *init_thread = thread_create ("init", thread_idle, NULL, 0);
    thread_switch (init_thread);
}

thread_t *
thread_create (char *name, thread_func_t f, void **args, int nargs)
{
    thread_t *new_thread = (thread_t *) alloc_page (PALLOC_CLEAR | PALLOC_VM_INSTALL);
    new_thread->id = global_tid++;
    strcpy(new_thread->name, name);
    new_thread->priority = DEFAULT_PRIORITY;
    REGFILE(new_thread->regfile, A0) = (uint64_t) f;
    REGFILE(new_thread->regfile, A1) = (uint64_t) args;
    REGFILE(new_thread->regfile, A2) = nargs;
    REGFILE(new_thread->regfile, SP) = (uint64_t) new_thread->stack;
    new_thread->regfile.pc = (uint64_t) thread_run;
    new_thread->magic = 0xDEADBEEF;
    return new_thread;
}

void
thread_exit ()
{

}

void
thread_yield (void)
{

}

void
thread_switch (thread_t *thread)
{

}

static void
thread_idle (void **args, int nargs) 
{
    while (true) {
        WFI();
    }
}
