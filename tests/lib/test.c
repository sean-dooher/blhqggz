#include <exit.h>
#include <stdio.h>

void 
assert(int assertion, const char *format, ...)
{
    va_list va;
    va_start(va, format);

    if (!assertion) {
        vprintf(format, va);
        printf("\n");      
        poweroff(-1);
    } else {
        printf("PASS\n");
    }

    va_end(va);
}