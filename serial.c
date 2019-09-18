#include "serial.h"

#define RX_EMPTY (1 << 31)
#define TX_FULL (1 << 31)
#define N_UART_BASES 2

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
    (struct uart_info *) UART1_BASE
};

static void uart_init(struct uart_info *);

void serial_init() { 
    for (int i = 0; i < N_UART_BASES; i++) {
        uart_init(UART_BASES[i]);
    }
}

static void uart_init(struct uart_info *uart) {
    uart->rxctrl |= 1;
    uart->txctrl |= 1;
}

void uart_putc(char c, UART_N uart) {
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

char uart_getc(UART_N uart) {
    uint32_t rx_data;
    while (((rx_data = UART_BASES[uart]->rxdata) & RX_EMPTY) != 0);
    return (char) rx_data & 0xF;
}

void uart_puts(char* str, uint32_t len, UART_N uart) {
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        uart_putc(str[i], uart);
    }
}

void uart_gets(char* str, uint32_t len, UART_N uart) {
    for (int i = 0; i < len; i++) {
        if((str[i] = uart_getc(uart)) == '\0')
            break;
    }
}
