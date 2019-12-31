#include <stdio.h>
#include <stdint.h>
#include <palloc.h>

#include "paging.h"
#include "devices/csr.h"
#include "devices/platform.h"
#include "devices/test.h"

extern uint8_t _text_base;
extern uint8_t _text_end;
extern uint8_t _rodata_base;
extern uint8_t _rodata_end;
extern uint8_t _data_base;
extern uint8_t _data_end;
extern uint8_t __bss_base;
extern uint8_t __bss_end;
extern uint8_t _free_mem_base;
extern uint8_t _free_mem_end;
extern uint8_t init_stack;

const void *KERNEL_TEXT_BASE = &_text_base;
const void *KERNEL_TEXT_END = &_text_end;
const void *KERNEL_RODATA_BASE = &_rodata_base;
const void *KERNEL_RODATA_END = &_rodata_end;
const void *KERNEL_DATA_BASE = &_data_base;
const void *KERNEL_DATA_END = &_data_end;
const void *KERNEL_BSS_BASE = &__bss_base;
const void *KERNEL_BSS_END = &__bss_end;
const void *FREE_MEM_BASE = &_free_mem_base;
const void *FREE_MEM_END = &_free_mem_end;
uint32_t N_FREE_PAGES;

bool mmu_enabled;

void
vm_init_early () 
{
    mmu_enabled = false;
    N_FREE_PAGES = N_PAGES (FREE_MEM_END - FREE_MEM_BASE);
    palloc_init ();

    // create root page table
    page_table_t *table = (page_table_t *) alloc_page(PALLOC_CLEAR);
    vm_install_page (table, (paddr_t) table, (vaddr_t) table, PTE_RW_PERM);

    // install kernel text
    vm_install_id_map (table, (page_t *) KERNEL_TEXT_BASE, N_PAGES(KERNEL_TEXT_END - KERNEL_TEXT_BASE), PTE_RX_PERM);

    // install kernel rodata
    vm_install_id_map (table, (page_t *) KERNEL_RODATA_BASE, N_PAGES(KERNEL_RODATA_END - KERNEL_RODATA_BASE), PTE_R_PERM);

    // install kernel data
    vm_install_id_map (table, (page_t *) KERNEL_DATA_BASE, N_PAGES(KERNEL_DATA_END - KERNEL_DATA_BASE), PTE_RW_PERM);

    // install kernel bss
    vm_install_id_map (table, (page_t *) KERNEL_BSS_BASE, N_PAGES(KERNEL_BSS_END - KERNEL_BSS_BASE), PTE_RW_PERM);

    // set up ID map for initial kernel stack
    uint8_t n_stack_pages = STACK_SIZE / PAGE_SIZE;
    page_t *stack_pages = ((page_t *) PAGE_DOWN(FREE_MEM_END)) - n_stack_pages;
    vm_install_id_map (table, stack_pages, n_stack_pages, PTE_RWX_PERM);

    // set up ID map for bitmap metadata
    vm_install_id_map (table, (page_t *) FREE_MEM_BASE, N_PALLOC_BITMAP_PAGES, PTE_RWX_PERM);

    // set up ID map for MMIO
    vm_install_page (table, CLINT_BASE, CLINT_BASE, PTE_RWX_PERM);
    vm_install_page (table, UART0_BASE, UART0_BASE, PTE_RWX_PERM);
    vm_install_page (table, TEST_CTRL_ADDR, TEST_CTRL_ADDR, PTE_RWX_PERM);


    vm_activate_address_space (table, 0x0);
    mmu_enabled = true;
}

void
vm_install_page (page_table_t *table, paddr_t phys_page, vaddr_t virt_page, uint64_t perm)
{
    debug("PHYS: 0x%lx to VIRT: 0x%lx\n", phys_page, virt_page);
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
    // mmu_enabled = true;
    uint64_t satp = SATP_MODE(0x8) | SATP_ASID(asid) | ((paddr_t) root >> PAGE_OFFSET);
    debug("SATP: 0x%lx\n", satp);

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

page_table_t *
vm_get_current_table (void)
{
    uint64_t satp;
    CSRR(satp, satp);

    return (page_table_t *) ((satp & SATP_PPN_MASK) << PAGE_OFFSET);
}
