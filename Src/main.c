#include "stm32f401.h"
#include "uart.h"

void TIM2_IRQHandler(void) {
    if (TIM2_SR & 0x01) {
        TIM2_SR &= ~0x01;           // Clear update interrupt flag
        GPIOC_ODR ^= (1 << 13);     // Toggle PC13;
        uart_send_string("hi");
    }
}

void gpio_init(void) {
    RCC_AHB1ENR |= (1 << 2);        // Enable GPIOC clock
    GPIOC_MODER &= ~(0x3 << (13 * 2)); // Clear PC13 mode bits
    GPIOC_MODER |= (0x1 << (13 * 2));  // Set PC13 to output
}

void tim2_init(void) {
    RCC_APB1ENR |= (1 << 0);        // Enable TIM2 clock
    TIM2_PSC = 16000 - 1;           // Prescaler: 16MHz / 16000 = 1kHz
    TIM2_ARR = 500 - 1;             // 500ms interval (500 counts at 1kHz)
    TIM2_DIER |= 0x01;              // Enable update interrupt
    TIM2_CR1 |= 0x01;               // Enable counter
    NVIC_ISER0 |= (1 << TIM2_IRQ_NUMBER); // Enable TIM2 IRQ
}

int main(void) {
    gpio_init();
    uart_init();
    tim2_init();

    while (1) {
        // Main loop does nothing
    }
}
