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
}
