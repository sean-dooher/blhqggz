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
#define SE_IRQ_OFFSET 9
#define UE_IRQ_OFFSET 8

#define MT_IRQ_OFFSET 7
#define ST_IRQ_OFFSET 5
#define UT_IRQ_OFFSET 4

#define MS_IRQ_OFFSET 3
#define SS_IRQ_OFFSET 1
#define US_IRQ_OFFSET 0

#ifndef ASSEMBLY
#define WFI() asm volatile("wfi");
#endif