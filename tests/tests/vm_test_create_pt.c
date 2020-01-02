#include <test.h>
#include <stdio.h>
#include <palloc.h>
#include <stdbool.h>
#include "devices/serial.h"
#include "vm/paging.h"

void test_create_pt ()
{
    vm_init_early ();
    serial_init ();

    page_table_t *root_table = (page_table_t *) alloc_page (PALLOC_CLEAR | PALLOC_VM_INSTALL);

    vm_install_kernel (root_table);
    vm_install_page (root_table, (paddr_t) &root_table, (vaddr_t) &root_table, PTE_RW_PERM);
    vm_uninstall_pagetable (root_table);
    assert(vm_translate (vm_get_current_table(), (vaddr_t) root_table) == (paddr_t) NULL, "Should have removed the root table from original address space");

    vm_activate_address_space (root_table, 0x1);
    assert ((paddr_t) vm_get_current_table () == (paddr_t) root_table, "Should have switched tables");
}

void
run_test()
{
    test_create_pt ();
}