#include "serial.h"

#define RX_EMPTY (1 << 31)
#define TX_FULL (1 << 31)

#ifdef SIFIVE_U
#define N_UART_BASES 2
#else
#define N_UART_BASES 1
#endif

struct uart_info {
    uint32_t txdata;
    uint32_t rxdata;
    uint32_t txctrl;
    uint32_t rxctrl;
    uint32_t ie;
    uint32_t ip;
    uint32_t div;
};

struct uart_info *UART_BASES[] = {
    (struct uart_info *) UART0_BASE,
    #if N_UART_BASES > 1
    (struct uart_info *) UART1_BASE
    #endif
};


void
uart_init(UART_N uart_n) 
{
    #ifdef SIFIVE_U
    struct uart_info *uart = UART_BASES[uart_n];
    uart->rxctrl |= 1;
    uart->txctrl |= 1;
    #endif
}

void
uart_putc(char c, UART_N uart)
{
    register uint32_t read_tx_data = (uint32_t) c;
    register uint32_t tx_data = (uint32_t) c;
    register struct uart_info *uart_base = UART_BASES[uart];

    do {
        __asm__ volatile (
            "amoswap.w %0, %1, (%2)"
            : "=r" (read_tx_data)
            : "r" (tx_data),
              "r" (uart_base)
        );
    } while ((read_tx_data & TX_FULL) != 0);
}

char
uart_getc(UART_N uart)
{
    uint32_t rx_data;
    while (((rx_data = UART_BASES[uart]->rxdata) & RX_EMPTY) != 0);

    char ret = (char) rx_data & 0xFF;
    return ret == '\r' ? '\n' : ret;
}

void
uart_puts(const char* str, UART_N uart)
{
    while (*str) {
        uart_putc(*str++, uart);
    }
}

void
uart_gets(char* str, uint32_t len, UART_N uart)
{
    int i;
    for (i = 0; i < len; i++) {
        if((str[i] = uart_getc(uart)) == '\0' || str[i] == '\r' || str[i] == '\n')
            break;
    }

    if (i + 1 < len - 1)
        str[i + 1] = '\0';
    else
        str[len - 1] = '\0';
}
