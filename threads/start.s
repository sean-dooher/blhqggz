#include "interrupts.h"

.section .text.init

.include "asm/bootup.s"
.include "asm/switch.s"

.global _start
_start:
    # set up interrupt vector
    la      t0, m_interrupt_vector
    csrw    mtvec, t0

    la      t0, s_interrupt_vector
    csrw    stvec, t0

    # sleep all cores except core 0
    csrr t0, mhartid
    bnez t0, _proc_sleep

_stack_setup:
    # set up stack pointer
    la sp, stack + STACK_SIZE

_smode_interrupt_delegation:
    li t0, 0xFFF
    csrrs x0, mideleg, t0

_smode_exception_delegation:
    li t0, 0xB1FF
    csrrs x0, medeleg, t0

# # FIXME: Figure out why smode interrupts aren't working
# # _smode_global_int_enable:
#     # enable s mode interrupts globally
#     li t0, 0x22
#     csrrs x0, mstatus, t0

#     # enable s mode interrupts
#     la t0, 0x222
#     csrrs x0, sie, t0

#     csrrsi a0, sstatus, 0x2

#     # enable smode interrupts in mie
#     li t0, 0xaaa
#     csrrs x0, mie, t0

#     # enable s mode interrupts globally
#     li t0, 0x2
#     csrrs x0, sstatus, t0

_mode_enable_interrupts:
    li t0, 0xaaa
    csrrs x0, mie, t0

_pmp_setup:
    # set up PMP for supervisor to access memory without error
    
    # set up mtvec to skip this if PMP not supported
    la t0, 1f
    csrrw t0, mtvec, t0

    # set the PMP addr to the whole addr space
    not t1, zero
    csrw pmpaddr0, t1

    # set up permissions as RWX and NA_POT
    li t1, PMP_NAPOT | PMP_R | PMP_X | PMP_W
    csrw pmpcfg0, t1

    # switch mtvec back to normal hanler
    .align 2
    1: csrw mtvec, t0

_ret_to_smode:
    # set up privilege mode to return to (S = 1)
    li t0, PRIV_S << MPP_OFFSET
    csrrs x0, mstatus, t0

    # set up address to jump to on switch to smode
    la t0, _s_mode_start
    csrw mepc, t0

    mret

_s_mode_start:
    jal ra, main

    li a0, 0
    jal ra, poweroff

_proc_sleep:
    wfi

.text
.align 4
.global init_interrupts_s
init_interrupts_s:
    csrrsi a0, sstatus, 0x2
    csrr a0, sstatus
    ret

.align 4
.global set_interrupt_level_s
set_interrupt_level_s:
    csrr t0, sie
    or a0, a0, t0
    csrrw a0, sie, a0

    csrr a0,sie
    ret

.align 4
.global software_interrupt_s
software_interrupt_s:
    csrrsi x0, sip, 0x2
    ret

.align 2
m_interrupt_vector:
    DUMP_REGISTERS_INT

    mv      a0, sp
    csrr    a1, mcause
    csrr    a2, mstatus
    csrr    a3, mepc
    mv      a4, sp
    jal     ra, machine_interrupt_handler

    csrr    t0, mepc
    addi    t0, t0, 4
    csrw    mepc, t0 

    RESTORE_REGISTERS_INT

    mret

.align 2
s_interrupt_vector:
    DUMP_REGISTERS_INT

    mv      a0, sp
    csrr    a1, scause
    csrr    a2, sstatus
    csrr    a3, sepc
    mv      a4, sp
    jal     ra, supervisor_interrupt_handler

    csrr    t0, sepc
    addi    t0, t0, 4
    csrw    sepc, t0

    RESTORE_REGISTERS_INT

    sret

.bss
.align 4
.global stack
stack:
    .skip STACK_SIZE
