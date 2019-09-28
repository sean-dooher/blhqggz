#pragma once

#include <stdint.h>

typedef enum UART_N {
    UART0 = 0,
    UART1
} UART_N;

#define serial_putc(c) uart_putc(c, UART0)
#define serial_getc(c) uart_getc(UART0)
#define serial_puts(str) uart_puts(str, UART0)
#define serial_gets(str, len) uart_gets(str, len, UART0)

#define serial_init() uart_init(UART0);

void uart_init(UART_N);

void uart_putc(char, UART_N);
char uart_getc(UART_N);

void uart_puts(const char*, UART_N);
void uart_gets(char*, uint32_t, UART_N);
