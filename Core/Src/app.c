/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#include "app.h"

void init_rcc(void) {
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOAEN;
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOBEN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM2EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_USART2EN;
};

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

void usart2_read_char(void) {
	while(!(_USART2->SR & (1u << 5)));
	char c = _USART2->DR;
	usart2_write_char(c);
}

void USART2_IRQHandler(void) { // from core/startup/startup_stm32f446retx.s
	// echo
	usart2_read_char();
}

// PB9 TIM2_CH2 and PB10 TIM2_CH3, driving PWMA and PWMB

// PB4, PB5, PB13, PB14 for AIN1, AIN2, BIN1, BIN2,

// PB8 for STBY

// UART2 for debugging
// Enable NVIC interrupt
// Write interrupt handler, keep it short

// TIM2 for everything
// Enable NVIC interrupt
// Write interrupt handler, keep it short

// SysTick for delays

