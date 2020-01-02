#include <palloc.h>
#include <stdbool.h>
#include <string.h>

#include "thread.h"
#include "vm/paging.h"

tid_t global_tid = 0;
// static void thread_idle (void *args);

static thread_t *current_thread;

thread_t *
thread_current (void)
{
    // thread_t *pointer; 
    // asm volatile (
    //     "mv %0, tp" : "=r"(pointer)
    // );
    return current_thread;
}

void
thread_run (thread_func_t f, void *args)
{
    f (args);
    thread_exit ();
}

void
thread_init (void)
{
    // initialize ready list, sleep list

    
    // Initial thread is stored at the bottom of the initial stack page
    thread_t *init_thread = (thread_t *) (INIT_STACK_PAGE + PAGE_SIZE - sizeof (thread_t));
    init_thread->id = global_tid++;
    strcpy(init_thread->name, "main");
    init_thread->priority = DEFAULT_PRIORITY;

    current_thread = init_thread;
}

thread_t *
thread_create (char *name, thread_func_t f, void *args)
{
    paddr_t thread_page = (paddr_t) alloc_page (PALLOC_CLEAR | PALLOC_VM_INSTALL);
    page_table_t *thread_pt = (page_table_t *) alloc_page (PALLOC_CLEAR | PALLOC_VM_INSTALL);
    vm_install_kernel (thread_pt);
    vm_install_page (thread_pt, thread_page, thread_page, PTE_RW_PERM);
    vm_uninstall_pagetable (thread_pt);

    thread_t *new_thread = (thread_t *) thread_page;
    new_thread->id = global_tid++;
    strcpy(new_thread->name, name);
    new_thread->priority = DEFAULT_PRIORITY;
    // REGFILE(new_thread->regfile, A0) = (uint64_t) f;
    // REGFILE(new_thread->regfile, A1) = (uint64_t) args;
    // REGFILE(new_thread->regfile, RA) = (uint64_t) thread_run;
    
    vm_uninstall_page (vm_get_current_table(), thread_page);
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
    if (thread_current() != NULL) {

    }
}

// static void
// thread_idle (void *args) 
// {
//     while (true) {
//         WFI();
//     }
// }

uint32_t
thread_get_thread_size (void)
{
    return sizeof(thread_t);
}