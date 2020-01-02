#include <stdio.h>
#include <stdbool.h>
#include <palloc.h>

#include "devices/serial.h"
#include "threads/interrupts.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "devices/machine.h"
#include "vm/paging.h"

void
main()
{
    vm_init_early ();
    serial_init ();
    thread_init ();

    thread_t *curr_thread = thread_current ();
    printf ("Current Thread: %s (%d)\n", curr_thread->name, curr_thread->id);
}
