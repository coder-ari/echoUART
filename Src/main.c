#include "stm32f401.h"
#include "uart.h"

void TIM2_IRQHandler(void) {
    if (TIM2_SR & 0x01) {
        TIM2_SR &= ~0x01;
        GPIOC_ODR ^= (1 << 13);
        uint32_t counter = TIM2_CNT;
        //uart_send_async_string("Hi from TIM2\r\n");
        uart_send_hex(counter);  // Sends: 0x000001F4 if counter is 500
        uart_send_char('\n');
    }
}

void gpio_init(void) {
    RCC_AHB1ENR |= (1 << 2);
    GPIOC_MODER &= ~(0x3 << (13 * 2));
    GPIOC_MODER |= (0x1 << (13 * 2));
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
    tim2_init();

    while (1) {
        }
    }

