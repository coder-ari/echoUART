#include "stm32f401.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include "stdint.h"

uint8_t rx_data[8];
uint8_t id = 0;
uint8_t len = 0;
uint32_t tx_counter = 0;

void gpio_init(void) {
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;

    // PC13 = output (LED)
    GPIOC_MODER &= ~(0x3 << (13 * 2));
    GPIOC_MODER |=  (0x1 << (13 * 2));

    // PA4 = output (MCP2515 CS)
    GPIOA_MODER &= ~(0x3 << (4 * 2));
    GPIOA_MODER |=  (0x1 << (4 * 2));

    cs_high();
}

void tim2_init(void) {
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2_PSC = 16000 - 1;     // 1ms tick
    TIM2_ARR = 1000 - 1;      // 1000ms = 1 sec
    TIM2_DIER |= TIM_DIER_UIE;
    TIM2_CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQ_NUMBER);
}

void TIM2_IRQHandler(void) {
    if (TIM2_SR & TIM_SR_UIF) {
        TIM2_SR &= ~TIM_SR_UIF;

        // Toggle PC13
        GPIOC_ODR ^= (1 << 13);

    }
}

int main(void) {
    gpio_init();
    uart_init();
    spi1_init();
    can_init();     // Init CAN (normal mode)
    tim2_init();    // Enable periodic sending

    uart_send_async_string("System Initialized in NORMAL MODE\r\n");

    while (1) {
    	uart_try_send_from_fifo();
    	if (can_receive(&id, rx_data, &len)) {
    	    for (int i = 0; i < len; i++) {
    	        uart_send_async(rx_data[i]);
    	    }
    	}
    }
}
