#include <exit.h>
#include <stdio.h>

void 
assert(int assertion, char *message)
{
    if (!assertion) {
        print(message);   
        poweroff(-1);
    } else {
        print("PASS");
    }
}