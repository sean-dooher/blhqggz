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
    serial_init();
    init_time();

    clint_schedule_ms (2000);

    while (true)
        printf("read_time: %ld\n", read_time());
}
