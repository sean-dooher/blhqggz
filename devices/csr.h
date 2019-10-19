#define CSRR(reg, var) do { asm volatile("csrr %0, " #reg ";":"=r"(var)); } while(0);
#define CSRW(reg, var) do { asm volatile("csrw " #reg", %0;"::"r"(var)); } while(0);
#define CSRRC(old, reg, var) do { asm volatile("csrrc %0, " #reg", %1;":"=r"(old):"r"(var)); } while(0);