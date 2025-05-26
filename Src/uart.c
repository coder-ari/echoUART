#include "stm32f401.h"
#include "uart.h"

#define UART_BUFFER_SIZE 128

// RX Ring Buffer
volatile char uart_rx_buffer[UART_BUFFER_SIZE];
volatile uint16_t uart_rx_head = 0;
volatile uint16_t uart_rx_tail = 0;

// TX Ring Buffer
volatile char uart_tx_buffer[UART_BUFFER_SIZE];
volatile uint16_t uart_tx_head = 0;
volatile uint16_t uart_tx_tail = 0;

void uart_init(void) {
    RCC_AHB1ENR |= (1 << 0); // GPIOA clock

    GPIOA_MODER &= ~((0x3 << (9 * 2)) | (0x3 << (10 * 2)));
    GPIOA_MODER |=  ((0x2 << (9 * 2)) | (0x2 << (10 * 2)));

    GPIOA_AFRH &= ~((0xF << ((9 - 8) * 4)) | (0xF << ((10 - 8) * 4)));
    GPIOA_AFRH |=  ((0x7 << ((9 - 8) * 4)) | (0x7 << ((10 - 8) * 4)));

    GPIOA_PUPDR &= ~((0x3 << (9 * 2)) | (0x3 << (10 * 2)));

    RCC_APB2ENR |= (1 << 4); // USART1 clock

    // Baud rate (for 9600 assuming 16MHz)
    //USART1_BRR = (8 << 4) | 11;

    // Baud rate (for 57600 assuming 16MHz)
    USART1_BRR = (17 << 4) | 6;

    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE
    USART1_CR1 |= (1 << 5); // RXNEIE

    NVIC_ISER1 |= (1 << (USART1_IRQ_NUMBER - 32)); // USART1 IRQ
}

void uart_send_async(char c) {
    uint16_t next_head = (uart_tx_head + 1) % UART_BUFFER_SIZE;

    if (next_head == uart_tx_tail) {
        // Buffer full, drop character or handle overflow
        return;
    }

    uart_tx_buffer[uart_tx_head] = c;
    uart_tx_head = next_head;

    // Enable TXE interrupt
    USART1_CR1 |= (1 << 7);
}

void uart_send_async_string(const char *str) {
    while (*str) {
        uart_send_async(*str++);
    }
}

int uart_get_char(char *c) {
    if (uart_rx_head == uart_rx_tail) return 0;
    *c = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % UART_BUFFER_SIZE;
    return 1;
}

void USART1_IRQHandler(void) {
    if (USART1_SR & (1 << 5)) { // RXNE
        char c = USART1_DR;
        uint16_t next_head = (uart_rx_head + 1) % UART_BUFFER_SIZE;
        if (next_head != uart_rx_tail) {
            uart_rx_buffer[uart_rx_head] = c;
            uart_rx_head = next_head;
        }
    }

    if (USART1_SR & (1 << 7)) { // TXE
        if (uart_tx_head == uart_tx_tail) {
            USART1_CR1 &= ~(1 << 7); // Disable TXE interrupt
        } else {
            USART1_DR = uart_tx_buffer[uart_tx_tail];
            uart_tx_tail = (uart_tx_tail + 1) % UART_BUFFER_SIZE;
        }
    }
}

char uart_receive_char(void) {
    // Wait until RXNE (Receive Data Register Not Empty) is set
    while (!(USART1_SR & (1 << 5)));

    // Read the received byte (only the lowest 8 bits are valid)
    return (char)(USART1_DR & 0xFF);
}

void uart_send_hex(uint32_t value) {
    uart_send_async((value >> 24) & 0xFF); // MSB first
    uart_send_async((value >> 16) & 0xFF);
    uart_send_async((value >> 8) & 0xFF);
    uart_send_async(value & 0xFF);         // LSB
}

