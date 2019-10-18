#include "devices/platform.h"
#include "devices/machine.h"
#include <sbi.h>

.include "asm/switch.s"

.text
.align 4
.global m_interrupt_vector
m_interrupt_vector:
m_synch_exc:
    j handle_m_interrupt
.align 2
m_s_soft_intr:
    j handle_m_interrupt
.align 2
m_res_soft_intr:
    j handle_m_interrupt
.align 2
m_m_soft_intr:
    j handle_m_interrupt
.align 2
m_u_timer_intr:
    j handle_m_interrupt
.align 2
m_s_timer_intr:
    j handle_m_interrupt
.align 2
m_res_timer_intr:
    j handle_m_interrupt
.align 2
m_m_timer_intr:
    j m_send_timer_to_s
.align 2
m_u_extern_intr:
    j handle_m_interrupt
.align 2
m_s_extern_intr:
    j handle_m_interrupt
.align 2
m_res_extern_intr:
    j handle_m_interrupt
.align 2
m_m_extern_intr:
    j handle_m_interrupt
.align 2
m_rest_intr:
    j handle_m_interrupt
.align 2
    j handle_m_interrupt
.align 2
    j handle_m_interrupt
.align 2
    j handle_m_interrupt
.align 2
    j handle_m_interrupt

.text
.align 4
m_send_timer_to_s:
    csrrw t0, mscratch, t0
    addi sp, sp, -8
    DUMP_REG t1, 0


    la t0, CLINT_BASE + SIFIVE_TIMECMP_BASE
    li t1, -1UL
    sd t1, 0(t0)

    li t0, (1 << ST_IRQ_OFFSET)
    csrrs x0, mip, t0

    RESTORE_REG t1, 0
    addi sp, sp, 8
    csrrw t0, mscratch, t0
    mret

.text
.align 4
handle_m_interrupt:
    M_MODE_INTERRUPT_ENTRY
    jal ra, machine_interrupt_handler
    M_MODE_INTERRUPT_EXIT
    mret

.text
.align 4
.global s_interrupt_vector
s_interrupt_vector:
s_synch_exc:
    j handle_s_interrupt
.align 2
s_s_soft_intr:
    j handle_s_interrupt
.align 2
s_res_soft_intr:
    j handle_s_interrupt
.align 2
s_m_soft_intr:
    j handle_s_interrupt
.align 2
s_u_timer_intr:
    j handle_s_interrupt
.align 2
s_s_timer_intr:
    j handle_s_timer_interrupt
.align 2
s_res_timer_intr:
    j handle_s_interrupt
.align 2
s_m_timer_intr:
    j handle_s_interrupt
.align 2
s_u_extern_intr:
    j handle_s_interrupt
.align 2
s_s_extern_intr:
    j handle_s_interrupt
.align 2
s_res_extern_intr:
    j handle_s_interrupt
.align 2
s_m_extern_intr:
    j handle_s_interrupt
.align 2
s_rest_intr:
    j handle_s_interrupt
.align 2
    j handle_s_interrupt
.align 2
    j handle_s_interrupt
.align 2
    j handle_s_interrupt
.align 2
    j handle_s_interrupt

.text
.align 4
handle_s_interrupt:
    DUMP_REGISTERS_INT

    mv      a0, sp
    csrr    a1, scause
    csrr    a2, sstatus
    csrr    a3, sepc
    mv      a4, sp

    jal ra, supervisor_interrupt_handler

    csrr    t0, sepc
    addi    t0, t0, 4
    csrw    sepc, t0
    RESTORE_REGISTERS_INT

    sret

.text
.align 4
handle_s_timer_interrupt:
    DUMP_REGISTERS_INT

    jal ra, timer_interrupt

    li a0, SBI_CLEAR_INTR
    li a1, ST_IRQ_OFFSET
    ecall

    RESTORE_REGISTERS_INT
    sret


.text
.align 4
.global clear_s_intr
clear_s_intr:
    csrrc x0, mip, a0
    ret
