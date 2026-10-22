/*
 * usart2.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "stm32f446xx.h"

#include "my_types.h"
#include "usart2.h"
#include "motor.h"

void init_usart2(void) {
	_GPIOA->MODER &= ~(3u << 4);
	_GPIOA->MODER &= ~(3u << 6);
	_GPIOA->MODER |= (2u << 4);
	_GPIOA->MODER |= (2u << 6);
	_GPIOA->AFRL &= ~(15u << 8);
	_GPIOA->AFRL &= ~(15u << 12);
	_GPIOA->AFRL |= (7u << 8);
	_GPIOA->AFRL |= (7u << 12);

	_USART2->CR1 |= (1u << 3); // TE
	_USART2->CR1 |= (1u << 2); // RE
	_USART2->CR1 |= (1u << 5); // RXNEIE
	NVIC->ISER[1] = (1u << 6);

	// Clock Tree: USART2 from APB1, 16MHz
	// Baud rate of 115200, f / BRR register value, 16000000/115200
	_USART2->BRR = (16000000/115200);

	_USART2->CR1 |= (1u << 13); // UE
};

void usart2_write_char(char c) {
	while(!(_USART2->SR & (1u << 7)))
		;
	_USART2->DR = c;
}

void usart2_write_string(char* str) {
	while(*str) {
		usart2_write_char(*str);
		str++;
	}
}

// Extend to strings when required
void usart2_read_char(void) {
	while(!(_USART2->SR & (1u << 5)));
	char c = _USART2->DR;

//	// echo
//	char buffer[100];
//	snprintf(buffer, sizeof(buffer), "Received: %u \r\n", c);
// 	usart2_write_string(buffer);

 	wasd_control(c);
}

// Extend functionality when required
void USART2_IRQHandler(void) { // from core/startup/startup_stm32f446retx.s
	// echo
	usart2_read_char();
}
