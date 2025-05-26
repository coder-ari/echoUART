#include "stm32f401.h"
#include "uart.h"
#include "spi.h"
#include "can.h"


#define MCP2515_READ_RXB0SIDH 0x90



void TIM2_IRQHandler(void) {
    if (TIM2_SR & 0x01) {
        TIM2_SR &= ~0x01;
        GPIOC_ODR ^= (1 << 13);

        uint8_t can_data[13]; // MCP2515 returns 13 bytes per CAN frame

        // Start SPI transaction
        // (assert MCP2515 CS low, depends on your CS pin)
        GPIOA_ODR &= ~(1 << 4); // Assuming PA4 is CS

        spi1_transfer(MCP2515_READ_RXB0SIDH); // Read RXB0

        for (int i = 0; i < 13; ++i) {
        	can_data[i] = spi1_transfer(0xFF); // Dummy byte to read
        }

        // End SPI transaction
        GPIOA_ODR |= (1 << 4); // CS high


        for (int i = 0; i < 13; ++i) {
        	uart_send_hex(can_data[i]);

        }

    }
}

void gpio_init(void) {
    RCC_AHB1ENR |= (1 << 2);
    GPIOC_MODER &= ~(0x3 << (13 * 2));
    GPIOC_MODER |= (0x1 << (13 * 2));

    GPIOA_MODER &= ~(3 << (2 * 4));
    GPIOA_MODER |=  (1 << (2 * 4));

    cs_high();
}

void tim2_init(void) {
    RCC_APB1ENR |= (1 << 0);
    TIM2_PSC = 16000 - 1;
    TIM2_ARR = 500 - 1;
    TIM2_DIER |= 0x01;
    TIM2_CR1 |= 0x01;
    NVIC_ISER0 |= (1 << TIM2_IRQ_NUMBER);
}

int main(void) {
    gpio_init();
    uart_init();
    spi1_init();
    tim2_init();

    while (1) {
        }
    }

