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
    bnez t0, proc_sleep

    # set up PMP for supervisor to access memory without error
    
    # set up mtvec to skip this if PMP not supported
    la t0, 1f
    csrrw t0, mtvec, t0

    # set the PMP addr to the whole addr space
    not t1, zero
    csrw pmpaddr0, t1

    # set up permissions as RWX and NA_POT
    addi t1, zero, PMP_NAPOT
    ori t1, t1, PMP_R
    ori t1, t1, PMP_W
    ori t1, t1, PMP_X
    csrw pmpcfg0, t1

    # switch mtvec back to normal hanler
    .align 2
    1: csrw mtvec, t0
  

    # set up privilege mode to return to (S = 1)
    
    # Create 
    addi t1, zero, PRIV_S
    slli t1, t1, MPP_OFFSET
    
    csrr t0, mstatus
    or t0, t0, t1

    # set up address to jump to on switch to smode
    la t0, s_mode_start
    csrw mepc, t0

    # set up stack pointer
    la sp, stack + STACK_SIZE

    csrw mstatus, t0

    mret

s_mode_start:
    jal ra, main

proc_sleep:
    wfi

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
