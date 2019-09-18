#pragma once
#include "stdint.h"

#define UART0_BASE 0x10010000
#define UART1_BASE 0x10011000

typedef enum UART_N {
    UART0 = 0,
    UART1
} UART_N;

#define serial_putc(c) uart_putc(c, UART0)
#define serial_getc(c) uart_getc(UART0)
#define serial_puts(str, len) uart_puts(str, len, UART0)
#define serial_gets(str, len) uart_gets(str, len, UART0)

void serial_init();

void uart_putc(char c, UART_N uart);
char uart_getc(UART_N uart);

void uart_puts(char* str, uint32_t len, UART_N uart);
void uart_gets(char* str, uint32_t len, UART_N uart);
