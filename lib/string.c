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

char *
reverse(char *str, size_t len)
{
    int half = len/2;
    for (int i = 0; i < half; i++) {
        char temp = str[i];
        str[i] = str[half - 1 - i];
        str[half - 1 - i] = temp;
    }
    return str;
}

char *
itoa(int val, char *str, int base)
{
    int i = 0;

    int neg = 0;

    if (val == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (val < 0 && base == 0) {
        neg = 1;
        val *= -1;
    }

    while (val > 0) {
        int rem = val % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        val /= base;
    }

    if (neg)
        str[i++] = '-';
    str[i] = '\0';

    reverse(str, i);

    return str;
}