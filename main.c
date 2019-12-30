#include <stdio.h>
#include <stdbool.h>

#include "devices/serial.h"
#include "threads/interrupts.h"
#include "devices/timer.h"
#include "devices/machine.h"
#include "vm/paging.h"

void
main()
{
    serial_init ();
    printf ("FREE_MEM_START: 0x%lx\n", FREE_MEM_BASE);
    printf ("FREE_MEM_END: 0x%lx\n", FREE_MEM_END);
    printf ("KERNEL_MEM_START: 0x%lx\n", KERNEL_TEXT_BASE);
    printf ("KERNEL_MEM_END: 0x%lx\n", KERNEL_TEXT_END);
    printf ("N_KERNEL_PAGES: %d\n", N_PAGES(KERNEL_TEXT_END - KERNEL_TEXT_BASE));

    while (true)
        WFI ();
}
