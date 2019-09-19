#include "string.h"

char *strcpy(char *dst, const char *src)
{
    char *ret = dst;
    while (*src)
    {
        *dst++ = *src++;
    }
    *dst = '\0';

    return ret;
}

char *
strncpy(char *dst, const char *src, size_t n)
{
    char *ret = dst;
    while (*src && n--)
    {
        *dst++ = *src++;
    }
    *dst = '\0';

    return ret;
}

size_t
strlen(const char *str)
{
    return 0;
}

size_t
strnlen(const char *str, size_t maxlen)
{
    return 0;
}

int
strcmp(const char *str1, const char *str2)
{
    while (*str1)
    {
        if (*str1 != *str2)
            break;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int
strncmp(const char *str1, const char *str2, size_t n)
{
    while (*str1 && n--)
    {
        if (*str1 != *str2)
            break;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void *
memcpy(void *dst, const void *src, size_t n)
{
    char *cdst = (char *) dst;
    char *csrc = (char *) src;
    for (int i = 0; i < n; i++)
        *cdst++ = *csrc++;

    return dst;
}
