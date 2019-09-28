.include "asm/switch.s"
.include "asm/bootup.s"

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

.macro csr_m_to_s csr_name, reg
    csrr \reg, m\csr_name
    csrw s\csr_name, \reg
.endm

.macro convert_mstatus reg
addi sp, sp, -8
DUMP_REG    t1, 0

# grab MPIE and move to SPIE

# set MPIE to 0

# set SPP to correct val from MPP

# set MPP to S_MODE

RESTORE_REG t1, 0
addi sp, sp, 8
.endm

.text
.align 4
m_send_timer_to_s:
    csrrw t0, mscratch, t0
    addi sp, sp, -8
    DUMP_REG t1, 0


    la t0, CLINT_TIMECMP
    ld t0, 0(t0)
    li t1, -1UL
    sd t1, 0(t0)

    li t0, (1 << SPIE_OFFSET)
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
