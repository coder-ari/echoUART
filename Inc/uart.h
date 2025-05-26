/*
 * uart.h
 *
 *  Created on: May 23, 2025
 *      Author: ARITRA
 */

#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send_async(char c);
void uart_send_async_string(const char *str);
int uart_get_char(char *c);

#define USART1_IRQ_NUMBER 37

#endif
