#include <test.h>
#include <stdio.h>
#include <palloc.h>
#include <stdbool.h>
#include "devices/serial.h"
#include "vm/paging.h"

void test_basic_vm ()
{
    vm_init_early ();
    serial_init ();

    page_t *phys_page = alloc_page (PALLOC_NO_FLAGS);
    page_t *virt_page1 = (page_t *) 0x500000000;
    page_t *virt_page2 = (page_t *) 0x600000000;

    vm_install_page (vm_get_current_table(), (paddr_t) phys_page, (vaddr_t) virt_page1, PTE_RW_PERM);
    vm_install_page (vm_get_current_table(), (paddr_t) phys_page, (vaddr_t) virt_page2, PTE_RW_PERM);

    for (int i = 0; i < sizeof(virt_page1->data); i++) {
        virt_page1->data[i] = 0xFA;
        assert (virt_page1->data[i] == virt_page2->data[i], "Change in one page should be reflected in other");
    }
}

void
run_test()
{
    test_basic_vm ();
}