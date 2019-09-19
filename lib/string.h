#pragma once

#include "stdint.h"

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t n);

size_t strlen(const char *str);
size_t strnlen(const char *str, size_t maxlen);

int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);

void *memcpy(void *dst, const void *src, size_t n);

char *reverse(char *str, size_t len);
char *itoa(int val, char *str, int base);