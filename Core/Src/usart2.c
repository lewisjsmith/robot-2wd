/*
 * usart2.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>

#include "stm32f446xx.h"

#include "rcc.h"
#include "gpio.h"
#include "usart2.h"

void init_usart2(uint32_t baud) {
	gpio_set_mode(_GPIOA, 2, AF);
	gpio_set_af(_GPIOA, 2, 7);

	gpio_set_mode(_GPIOA, 3, AF);
	gpio_set_af(_GPIOA, 3, 7);

	_USART2->CR1 |= (1u << 3); // TE
	_USART2->CR1 |= (1u << 2); // RE
	_USART2->CR1 |= (1u << 5); // RXNEIE
	NVIC->ISER[1] = (1u << 6);

	// Clock Tree: USART2 from APB1, 16MHz
	_USART2->BRR = (APB1_HZ/baud);

	_USART2->CR1 |= (1u << 13); // UE
}

void usart2_write_char(char c) {
	while(!(_USART2->SR & (1u << 7)))
		;
	_USART2->DR = c;
}

void usart2_write_string(const char* str) {
	while(*str) {
		usart2_write_char(*str);
		str++;
	}
}

#define RX_BUF_SIZE 8u
static volatile char rx_buf[RX_BUF_SIZE];
static volatile uint8_t rx_head, rx_tail;

// from core/startup/startup_stm32f446retx.s
void USART2_IRQHandler(void) {
	if(_USART2->SR & (1u << 5)) {
		char c = _USART2->DR; // Copy out
		uint8_t next = (rx_head + 1) & (RX_BUF_SIZE - 1);
		if(next != rx_tail) {
			rx_buf[rx_head] = c;
			rx_head = next;
		}
	}
}

bool usart2_try_read(char* out) {
	if(rx_tail == rx_head) return false;
	*out = rx_buf[rx_tail];
	rx_tail = (rx_tail + 1) & (RX_BUF_SIZE - 1);
	return true;
}
