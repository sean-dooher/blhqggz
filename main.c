#include <stdio.h>
#include <stdbool.h>
#include <palloc.h>

#include "devices/serial.h"
#include "threads/interrupts.h"
#include "devices/timer.h"
#include "devices/machine.h"
#include "vm/paging.h"

void
main()
{
    vm_init_early ();
    serial_init ();

    printf ("FREE_MEM_START: 0x%lx\n", FREE_MEM_BASE);
    printf ("FREE_MEM_END: 0x%lx\n", FREE_MEM_END);
    printf ("KERNEL_MEM_START: 0x%lx\n", KERNEL_TEXT_BASE);
    printf ("KERNEL_MEM_END: 0x%lx\n", KERNEL_TEXT_END);
    printf ("N_KERNEL_PAGES: %d\n", N_PAGES(KERNEL_TEXT_END - KERNEL_TEXT_BASE));
    printf ("N_FREE_PAGES: %d\n", N_FREE_PAGES);

    size_t pages_allocated = -1;
    while (true) {
        pages_allocated++;
        if (alloc_page() == NULL) {
            break;
        }
    }

    printf("Pages Written: %ld\n", pages_allocated);
}
