#include "devices/serial.h"
#include "exit.h"
#include "threads/interrupts.h"
#include <stdio.h>
#include <stdbool.h>
#include "devices/timer.h"
#include "devices/clint.h"

void
main()
{
    serial_init ();
    time_init ();

    clint_schedule_ms (2000);

    uint64_t i;
    for (i = 0; ; i++)
        printf("read_time: %ld\n", time_read ());
}
