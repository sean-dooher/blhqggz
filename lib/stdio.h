#pragma once

#include <stdarg.h>
#include <printf.h>

#ifdef DEBUG
    #define debug(...) printf(__VA_ARGS__)
#else
    #define debug()
#endif

void print(const char *msg);
