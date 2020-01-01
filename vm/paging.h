#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <palloc.h>
#include "devices/machine.h"

extern const void *KERNEL_TEXT_BASE;
extern const void *KERNEL_TEXT_END;
extern const void *KERNEL_RODATA_BASE;
extern const void *KERNEL_RODATA_END;
extern const void *KERNEL_DATA_BASE;
extern const void *KERNEL_DATA_END;
extern const void *KERNEL_BSS_BASE;
extern const void *KERNEL_BSS_END;
extern const void *FREE_MEM_BASE;
extern const void *FREE_MEM_END;
extern uint32_t N_FREE_PAGES;

extern bool mmu_enabled;

#define N_PAGES(SIZE) (((SIZE) + (PAGE_SIZE - 1)) / PAGE_SIZE)

#define PAGE_OFFSET 12
#define ADDR_OFFSET(addr) (((uint64_t) addr) & 0xFFF)
#define PAGE_DOWN(addr) (((uint64_t) addr) & ~0XFFF)

#define VPN_N(addr, i) ((addr >> (PAGE_OFFSET + 9 * i)) & 0x1ff)
#define VPN_0(addr) VPN_N(addr, 0)
#define VPN_1(addr) VPN_N(addr, 1)
#define VPN_2(addr) VPN_N(addr, 2)

#define PTE_V_MASK 0x1
#define PTE_R_MASK 0x2
#define PTE_W_MASK 0x4
#define PTE_X_MASK 0x8
#define PTE_U_MASK 0x10
#define PTE_G_MASK 0x20
#define PTE_A_MASK 0x40
#define PTE_D_MASK 0x80
#define PTE_RSW_MASK 0x300
#define PTE_R_PERM (PTE_R_MASK)
#define PTE_X_PERM (PTE_X_MASK)
#define PTE_W_PERM (PTE_W_MASK)
#define PTE_RX_PERM (PTE_R_PERM | PTE_X_PERM)
#define PTE_RW_PERM (PTE_R_PERM | PTE_W_PERM)
#define PTE_RWX_PERM (PTE_R_PERM | PTE_W_PERM | PTE_X_PERM)

#define PTE_PPN0_OFFSET 10
#define PTE_PPN1_OFFSET 19
#define PTE_PPN2_OFFSET 27
#define PTE_RWX_OFFSET 1

#define PADDR_TO_PPN(paddr) (paddr >> PAGE_OFFSET)
#define PPN_TO_PTE_MASK(ppn) (ppn << PTE_PPN0_OFFSET)
#define PTE_PPN(pte) ((pte >> PTE_PPN0_OFFSET) & 0xfffffffffff)

#define PTE_VALID(pte) (pte & PTE_V_MASK)
#define PTE_PERM(pte) ((pte & (PTE_R_MASK | PTE_W_MASK | PTE_X_MASK)) >> PTE_RWX_OFFSET)

#define PT_LEVELS 3

#define SATP_MODE(mode) (((uint64_t) mode) << 60)
#define SATP_ASID(asid) (((uint64_t) asid) << 44)
#define SATP_PPN_MASK 0xfffffffffff

typedef uint64_t pte_t;
typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

typedef struct page_table {
    pte_t entries[PAGE_SIZE / sizeof(pte_t)];
} page_table_t;


void vm_init_early (void);

void vm_install_page (page_table_t *root, paddr_t phys_page, vaddr_t virt_page, uint64_t perm);
void vm_uninstall_page (page_table_t *root, vaddr_t virt_page);
void vm_access_page (page_table_t *root, vaddr_t virt_addr, bool dirty);

void vm_install_kernel (page_table_t *root);


paddr_t vm_translate (page_table_t *root, vaddr_t virt_addr);
pte_t *vm_get_pte (page_table_t *root, vaddr_t virt_addr, int level);

page_table_t *vm_get_current_table (void);
void vm_activate_address_space (page_table_t *root, uint16_t asid);

static inline void
vm_install_id_map (page_table_t *table, page_t *base, size_t n_pages, uint64_t perm)
{
    for (int i = 0; i < n_pages; i++) {
        vm_install_page (table, (paddr_t) &base[i], (vaddr_t) &base[i], perm);
    }
}