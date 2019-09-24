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
    li t0, 0x111
    csrw mideleg, t0

_smode_exception_delegation:
    li t0, 0xB3FF
    csrw medeleg, t0

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
.global enable_interrupts_s
enable_interrupts_s:
    li t0, 0x111
    csrw sie, t0
    ret

.align 2
m_interrupt_vector:
    DUMP_REGISTERS_INT

    mv      a0, sp
    csrr    a1, mcause
    csrr    a2, mepc
    jal     ra, machine_interrupt_handler

    RESTORE_REGISTERS_INT

    mret

.align 2
s_interrupt_vector:
    DUMP_REGISTERS_INT

    mv      a0, sp
    csrr    a1, scause
    csrr    a2, sepc
    jal     ra, supervisor_interrupt_handler

    RESTORE_REGISTERS_INT

    sret


.bss
.align 4
.global stack
stack:
    .skip STACK_SIZE

.align 4
.global s_stack
s_stack:
    .skip STACK_SIZE
