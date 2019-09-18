#include "interrupts.h"

.section .text.init

.equ STACK_SIZE, 4096
.equ N_CONTEXT_REG, 16
.equ CONTEXT_SIZE, (N_CONTEXT_REG << 3)

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

#define basic_stack
.global _start
_start:
    # set up interrupt vector
    la      t0, interrupt_vector
    csrw    mtvec, t0

    # sleep all cores except core 0
    csrr t0, mhartid
    bnez t0, proc_sleep

    # set up stack pointer
    la sp, stack + 4096

    j jump_to_c

jump_to_c:
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


.bss
.align 4
.global stack
stack:
    .skip STACK_SIZE
