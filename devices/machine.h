// INIT
#define STACK_SIZE 4096

// PMP
#define PMP_R 0x01
#define PMP_W 0x02
#define PMP_X 0x04

#define PMP_NAPOT 0x18

// PRIVILEGE MODES
#define PRIV_U 0x00
#define PRIV_S 0x01
#define PRIV_M 0x11


// MSTATUS OFFSETS
#define MPP_OFFSET 11
#define SPP_OFFSET 8
#define MIE_OFFSET 3
#define SPIE_OFFSET 5
#define SIE_OFFSET 1


// IRQ OFFSETS
#define ME_IRQ_OFFSET 11
#define ME_IRQ (1 << ME_IRQ_OFFSET)

#define MS_IRQ_OFFSET 3
#define MS_IRQ (1 << MS_IRQ_OFFSET)

#define MT_IRQ_OFFSET 7
#define MT_IRQ (1 << MT_IRQ_OFFSET)

#define M_IRQS (ME_IRQ | MS_IRQ | MT_IRQ)

// S_MODE IRQ VALUES
#define ST_IRQ_OFFSET 5
#define ST_IRQ (1 << ST_IRQ_OFFSET)

#define SE_IRQ_OFFSET 9
#define SE_IRQ (1 << SE_IRQ_OFFSET)

#define SS_IRQ_OFFSET 1
#define SS_IRQ (1 << SS_IRQ_OFFSET)

#define S_IRQS (SE_IRQ | SS_IRQ | ST_IRQ)

// U_MODE IRQ VALUES
#define UE_IRQ_OFFSET 8
#define UE_IRQ (1 << UE_IRQ_OFFSET)

#define UT_IRQ_OFFSET 4
#define UT_IRQ (1 << UT_IRQ_OFFSET)

#define US_IRQ_OFFSET 0
#define US_IRQ (1 << US_IRQ_OFFSET)

#define U_IRQS (UE_IRQ | US_IRQ | UT_IRQ)


#ifndef __ASSEMBLER__
#define WFI() asm volatile("wfi");
#endif