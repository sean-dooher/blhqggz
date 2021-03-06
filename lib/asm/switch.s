.equ N_CONTEXT_REG, 16
.equ CONTEXT_SIZE, (N_CONTEXT_REG << 3)

.macro DUMP_REG reg, n
sd \reg, (\n << 3)(sp)
.endm

.macro RESTORE_REG reg, n
ld \reg, (\n << 3)(sp)
.endm

.macro DUMP_REGISTERS_INT
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

.macro RESTORE_REGISTERS_INT
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

.macro M_MODE_INTERRUPT_ENTRY
DUMP_REGISTERS_INT

mv      a0, sp
csrr    a1, mcause
csrr    a2, mstatus
csrr    a3, mepc
mv      a4, sp
.endm

.macro M_MODE_INTERRUPT_EXIT
csrr    t0, mepc
addi    t0, t0, 4
csrw    mepc, t0

RESTORE_REGISTERS_INT
.endm
