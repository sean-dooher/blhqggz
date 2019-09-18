#include "interrupts.h"

.section .text.init

.equ STACK_SIZE, 4096
.equ N_CONTEXT_REG, 16
.equ CONTEXT_SIZE, (N_CONTEXT_REG << 3)

.equ PMP_R, 0x01
.equ PMP_W, 0x02
.equ PMP_X, 0x04

.equ PMP_NAPOT, 0x18

.macro DUMP_REG reg, n
sd \reg, (\n << 3)(sp)
.endm

.macro RESTORE_REG reg, n
lw \reg, (\n << 3)(sp)
.endm

.macro DUMP_REGISTERS
addi sp, sp, -CONTEXT_SIZE
DUMP_REG ra, 0
DUMP_REG a0, 1
DUMP_REG a1, 2
DUMP_REG a2, 3
DUMP_REG a3, 4
DUMP_REG a4, 5
DUMP_REG a5, 6
DUMP_REG a6, 7
DUMP_REG a7, 8
DUMP_REG t0, 9
DUMP_REG t1, 10
DUMP_REG t2, 11
DUMP_REG t3, 12
DUMP_REG t4, 13
DUMP_REG t5, 14
DUMP_REG t6, 15
.endm

.macro RESTORE_REGISTERS
RESTORE_REG ra, 0
RESTORE_REG a0, 1
RESTORE_REG a1, 2
RESTORE_REG a2, 3
RESTORE_REG a3, 4
RESTORE_REG a4, 5
RESTORE_REG a5, 6
RESTORE_REG a6, 7
RESTORE_REG a7, 8
RESTORE_REG t0, 9
RESTORE_REG t1, 10
RESTORE_REG t2, 11
RESTORE_REG t3, 12
RESTORE_REG t4, 13
RESTORE_REG t5, 14
RESTORE_REG t6, 15
addi sp, sp, CONTEXT_SIZE
.endm

.global _start
_start:
    # set up interrupt vector
    la      t0, interrupt_vector
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
    csrr t0, mstatus

    addi t1, zero, 1
    slli t1, t1, 11
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
interrupt_vector:
    DUMP_REGISTERS

    mv      a0, sp
    csrr    a1, mcause
    csrr    a2, mepc
    jal     ra, interrupt_handler

    RESTORE_REGISTERS

    mret

.align 2
s_interrupt_vector:
    DUMP_REGISTERS

    mv      a0, sp
    csrr    a1, scause
    csrr    a2, sepc
    jal     ra, interrupt_handler

    RESTORE_REGISTERS

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
