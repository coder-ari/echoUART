/*
 * spi.c
 *
 *  Created on: May 26, 2025
 *      Author: ARITRA
 */

#include "stm32f401.h"
#include "spi.h"

void spi1_init(void) {
    RCC_AHB1ENR |= (1 << 0);    // Enable GPIOA
    RCC_APB2ENR |= (1 << 12);   // Enable SPI1

    // PA5 = SCK, PA6 = MISO, PA7 = MOSI
    GPIOA_MODER &= ~(0xFF << (5 * 2));
    GPIOA_MODER |=  (0xAA << (5 * 2));  // Alternate function

    GPIOA_AFRL &= ~(0xFFF << (5 * 4));
    GPIOA_AFRL |=  (0x555 << (5 * 4));  // AF5

    // Configure SPI
    SPI1_CR1 = (1 << 2)  | // Master
               (1 << 1)  | // CPOL=1
               (1 << 0)  | // CPHA=1
               (3 << 3)  | // Baud rate (fPCLK/16)
               (1 << 6);   // SPI enable
}

uint8_t spi1_transfer(uint8_t data) {
    while (!(SPI1_SR & (1 << 1)));  // TXE
    SPI1_DR = data;
    while (!(SPI1_SR & (1 << 0)));  // RXNE
    return SPI1_DR;
}
