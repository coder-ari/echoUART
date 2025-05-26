#include "stm32f401.h"


void uart_init(void) {
    // 1. Enable GPIOA clock
    RCC_AHB1ENR |= (1 << 0);   // GPIOAEN

    // 2. Set PA9 (TX) and PA10 (RX) to Alternate Function mode (AF7)
    GPIOA_MODER &= ~((0x3 << (9 * 2)) | (0x3 << (10 * 2))); // Clear mode bits
    GPIOA_MODER |=  ((0x2 << (9 * 2)) | (0x2 << (10 * 2))); // AF mode = 10

    // 3. Set AF7 (USART1) on PA9 and PA10
    GPIOA_AFRH &= ~((0xF << ((9 - 8) * 4)) | (0xF << ((10 - 8) * 4)));
    GPIOA_AFRH |=  ((0x7 << ((9 - 8) * 4)) | (0x7 << ((10 - 8) * 4)));

    // 4. No pull-up, no pull-down for PA9 and PA10 (optional)
    GPIOA_PUPDR &= ~((0x3 << (9 * 2)) | (0x3 << (10 * 2)));

    // 5. Enable USART1 clock (APB2ENR bit 4)
    RCC_APB2ENR |= (1 << 4);

    // 6. Configure USART1 baud rate using mantissa and fraction

    uint32_t mantissa = 0x8;
    uint32_t fraction = 0xB;
    USART1_BRR = (mantissa << 4) | fraction;

    // 7. Enable USART1, transmitter and receiver
    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE
}


void uart_send_char(char c) {
    while (!(USART1_SR & (1 << 7)));  // Wait until TXE is set (data register empty)
    USART1_DR = (uint8_t)c;            // Write only 8 bits
}

void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}
