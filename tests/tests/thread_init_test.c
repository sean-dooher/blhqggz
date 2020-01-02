#include <test.h>
#include <stdio.h>
#include <string.h>
#include "devices/serial.h"
#include "vm/paging.h"
#include "threads/thread.h"

void test_thread_init ()
{
    vm_init_early ();
    serial_init ();
    thread_init ();

    assert (strcmp(thread_current ()->name, "main") == 0, "Should be running main thread");
    assert (thread_current ()->id == 0, "Main thread should have id = 0");
}

void
run_test()
{
    test_thread_init ();
}