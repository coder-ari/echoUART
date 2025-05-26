/*
 * can.h
 *
 *  Created on: May 26, 2025
 *      Author: ARITRA
 */

#ifndef CAN_H
#define CAN_H

#include <stdint.h>

uint8_t mcp2515_read(uint8_t address);
void mcp2515_read_rx_buffer(uint8_t *buffer, uint8_t len);
void cs_low(void);
void cs_high(void);

#endif
