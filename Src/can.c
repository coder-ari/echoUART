/*
 * can.c
 *
 *  Created on: May 26, 2025
 *      Author: ARITRA
 */

#include "stm32f401.h"
#include "spi.h"
#include "can.h"


#define MCP2515_READ          0x03
#define MCP2515_WRITE         0x02
#define MCP2515_BIT_MODIFY    0x05
#define MCP2515_READ_STATUS   0xA0
#define MCP2515_RX_STATUS     0xB0
#define MCP2515_RXB0SIDH      0x61
#define MCP2515_CANINTF       0x2C

#define CS_PIN          4

void cs_low(void) {
    GPIOA_ODR &= ~(1 << CS_PIN);
}

void cs_high(void) {
    GPIOA_ODR |= (1 << CS_PIN);
}
uint8_t mcp2515_read(uint8_t address) {
    cs_low();
    spi1_transfer(MCP2515_READ);
    spi1_transfer(address);
    uint8_t data = spi1_transfer(0xFF);
    cs_high();
    return data;
}

void mcp2515_read_rx_buffer(uint8_t *buffer, uint8_t len) {
    cs_low();
    spi1_transfer(0x90);  // Read RX Buffer 0 (0x90)
    for (uint8_t i = 0; i < len; ++i) {
        buffer[i] = spi1_transfer(0xFF);
    }
    cs_high();
}
