#include <stdio.h>
#include <stdint.h>
#include <palloc.h>

#include "paging.h"
#include "devices/csr.h"
#include "devices/platform.h"
#include "devices/test.h"

extern uint8_t _free_mem_start;
extern uint8_t _free_mem_end;
extern uint8_t _text_base;
extern uint8_t _text_end;
extern uint8_t init_stack;

const void *KERNEL_TEXT_BASE = &_text_base;
const void *KERNEL_TEXT_END = &_text_end;
const void *FREE_MEM_BASE = &_free_mem_start;
const void *FREE_MEM_END = &_free_mem_end;
uint32_t N_FREE_PAGES;

void
vm_init_early () 
{
    N_FREE_PAGES = N_PAGES (FREE_MEM_END - FREE_MEM_BASE);
    palloc_init ();
    
    uint64_t rx_perm = PTE_R_MASK | PTE_X_MASK | PTE_A_MASK;
    uint64_t rwx_perm = PTE_R_MASK | PTE_W_MASK | PTE_X_MASK | PTE_A_MASK | PTE_D_MASK;

    // create root page table
    page_table_t *table = (page_table_t *) alloc_page(PALLOC_CLEAR);
    vm_install_page (table, (paddr_t) table, (vaddr_t) table, rwx_perm);

    // set up ID map for kernel pages
    page_t *kernel_pages = (page_t *) KERNEL_TEXT_BASE;
    uint8_t n_kernel_pages = N_PAGES (KERNEL_TEXT_END - KERNEL_TEXT_BASE);
    for (int i = 0; i < n_kernel_pages; i++) {
        vm_install_page (table, (paddr_t) &kernel_pages[i], (vaddr_t) &kernel_pages[i], rx_perm);
    }

    // set up ID map for initial kernel stack
    page_t *stack_pages = (page_t *) PAGE_DOWN(FREE_MEM_END);
    uint8_t n_stack_pages = STACK_SIZE / PAGE_SIZE;
    for (int i = n_stack_pages; i > 0 ; i--) {
        vm_install_page (table, (paddr_t) stack_pages - i, (vaddr_t) stack_pages - i, rwx_perm);
    }

    // set up ID map for MMIO
    vm_install_page (table, CLINT_BASE, CLINT_BASE, rwx_perm);
    vm_install_page (table, UART0_BASE, UART0_BASE, rwx_perm);
    vm_install_page (table, TEST_CTRL_ADDR, TEST_CTRL_ADDR, rwx_perm);


    vm_activate_address_space (table, 0x0);
}

void
vm_install_page (page_table_t *table, paddr_t phys_page, vaddr_t virt_page, uint64_t perm)
{
    page_table_t *root = table;
    for (int i = PT_LEVELS - 1; i > 0; i--) {
        uint64_t vpn = VPN_N(virt_page, i);
        pte_t *pte_p = &table->entries[vpn];
        if ((*pte_p & PTE_V_MASK) == 0) {
            paddr_t next_table = (paddr_t) alloc_page(PALLOC_CLEAR);
            uint64_t pte = PPN_TO_PTE_MASK(PADDR_TO_PPN(next_table)) | PTE_V_MASK;
            *pte_p = pte;

            uint64_t rwx_perm = PTE_R_MASK | PTE_W_MASK | PTE_X_MASK | PTE_A_MASK | PTE_D_MASK;
            vm_install_page(root, next_table, next_table, rwx_perm);
        }

        table = (page_table_t *) (PTE_PPN(*pte_p) << PAGE_OFFSET);
    }

    uint64_t vpn = VPN_N(virt_page, 0);
    uint64_t pte = PTE_V_MASK | perm | PPN_TO_PTE_MASK(PADDR_TO_PPN(phys_page));
    table->entries[vpn] = pte;
}

void
vm_activate_address_space (page_table_t *root, uint16_t asid)
{
    uint64_t satp = SATP_MODE(0x8) | SATP_ASID(asid) | ((paddr_t) root >> PAGE_OFFSET);
    printf("SATP: 0x%lx\n", satp);

    asm volatile ("sfence.vma");
    CSRW(satp, satp);
}

paddr_t
vm_walk_table (page_table_t *root, vaddr_t virt_addr, int level)
{
    uint64_t vpn = VPN_N(virt_addr, level);
    pte_t pte = root->entries[vpn];

    if ((pte & PTE_V_MASK) == 0) {
        return (paddr_t) NULL;
    }

    paddr_t paddr = PTE_PPN(pte) << PAGE_OFFSET;

    if (level == 0) {
        return paddr | ADDR_OFFSET(virt_addr);
    } else {
        return vm_walk_table ((page_table_t *) paddr, virt_addr, level - 1);
    }
}