/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#include <stdio.h>

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

// Extend to strings when required
void usart2_read_char(void) {
	while(!(_USART2->SR & (1u << 5)));
	char c = _USART2->DR;

	// echo
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Received: %c \r\n", c);
 	usart2_write_string(buffer);
}

// Extend functionality when required
void USART2_IRQHandler(void) { // from core/startup/startup_stm32f446retx.s
	// echo
	usart2_read_char();
}

void init_systick(void){
	// Set to 1ms
	SysTick->LOAD = 16000;
	SysTick->VAL = 0;
	SysTick->CTRL |= (1u << 2);
	SysTick->CTRL |= (1u << 1);
	SysTick->CTRL |= (1u << 0);
}

uint32_t SYSTICK_MILLISECONDS = 0;

void SysTick_Handler(void) {
	SYSTICK_MILLISECONDS += 1;
}

void delay_ms(uint32_t milliseconds) {
	uint32_t start = SYSTICK_MILLISECONDS;
	while((SYSTICK_MILLISECONDS - start) < milliseconds);
}

void init_pwm_tim2(void) {
//	_TIM2->DIER |= (1u << 6); // TIE
//	_TIM2->DIER |= (1u << 0); // UIE
	// Alternative is CC1E, the capture-compare register interrupt enables

	// Channel 2, PB9
	_TIM2->CCMR1 &= ~(3u << 8); // Set as output
	_TIM2->CCMR1 &= ~(7u << 12); // Mode
	_TIM2->CCMR1 |= (6u << 12); // Mode

	// Channel 3, PB10
	_TIM2->CCMR2 &= ~(3u << 0); // "
	_TIM2->CCMR2 &= ~(7u << 4); // "
	_TIM2->CCMR2 |= (6u << 4); // "

	_TIM2->CR1 |= (1u << 7); // ARPE

	_TIM2->CCER |= (1u << 4);
	_TIM2->CCER |= (1u << 8);

	// 20kHz aim, 16MHz source, maintain resolution with ARR > PSC
	_TIM2->PSC = 0;
	_TIM2->ARR = 800 - 1;

	// Wheel speed differential comes from duty cycles on CCR2 and CCR3
	_TIM2->CCR2 = 400 - 1;
	_TIM2->CCR3 = 400 - 1;

	_TIM2->EGR = (1u << 0);
	_TIM2->SR &= ~(1u << 0);

	_TIM2->CR1 |= (1u << 0); // CEN
}

void init_pwm_gpiob(void) {
	_GPIOB->MODER &= ~(3u << 18);
	_GPIOB->MODER &= ~(3u << 20);
	_GPIOB->MODER |= (2u << 18);
	_GPIOB->MODER |= (2u << 20);
	_GPIOB->AFRH &= ~(15u << 4);
	_GPIOB->AFRH &= ~(15u << 8);
	_GPIOB->AFRH |= (1u << 4);
	_GPIOB->AFRH |= (1u << 8);
}

void TIM2_IRQHandler(void){
	// _TIM2->SR may need clearing on interrupt
	// Do something
};

// Refactor needed below

void init_mdriver_gpiob(void) {
	// PB4
	_GPIOB->MODER &= ~(3u << 8);
	_GPIOB->MODER |= (1u << 8);

	// PB5
	_GPIOB->MODER &= ~(3u << 10);
	_GPIOB->MODER |= (1u << 10);

	// PB13
	_GPIOB->MODER &= ~(3u << 26);
	_GPIOB->MODER |= (1u << 26);

	// PB14
	_GPIOB->MODER &= ~(3u << 28);
	_GPIOB->MODER |= (1u << 28);

	// PB8
	_GPIOB->MODER &= ~(3u << 16);
	_GPIOB->MODER |= (1u << 16);
}

// Encoder mode for timers, increment or decrement a count, poll CNT at a set time period to derive speed
void init_encoder_gpio(void){

};

// 2x CCW for forwards
void test_motor_FW(void) {
	_TIM2->CCR2 = 400 - 1;
	_TIM2->CCR3 = 400 - 1;

	_GPIOB->BSRR |= (1u << 8);

	_GPIOB->BSRR |= (1u << 20);
	_GPIOB->BSRR |= (1u << 5);

	_GPIOB->BSRR |= (1u << 13);
	_GPIOB->BSRR |= (1u << 30);
}

void test_motor_BW(void) {
	_TIM2->CCR2 = 400 - 1;
	_TIM2->CCR3 = 400 - 1;

	_GPIOB->BSRR |= (1u << 8);

	_GPIOB->BSRR |= (1u << 4);
	_GPIOB->BSRR |= (1u << 21);

	_GPIOB->BSRR |= (1u << 29);
	_GPIOB->BSRR |= (1u << 14);
}

// Requires memory of the previous speed
void test_motor_LEFT(void) {
	_TIM2->CCR2 = 0;
	_TIM2->CCR3 = 400 - 1;

	_GPIOB->BSRR |= (1u << 8);

	_GPIOB->BSRR |= (1u << 4);
	_GPIOB->BSRR |= (1u << 21);

	_GPIOB->BSRR |= (1u << 29);
	_GPIOB->BSRR |= (1u << 14);
}

void test_motor_RIGHT(void) {
	_TIM2->CCR2 = 400 - 1;
	_TIM2->CCR3 = 0;

	_GPIOB->BSRR |= (1u << 8);

	_GPIOB->BSRR |= (1u << 4);
	_GPIOB->BSRR |= (1u << 21);

	_GPIOB->BSRR |= (1u << 29);
	_GPIOB->BSRR |= (1u << 14);
}

void test_motor_CCW(void) {
	_GPIOB->BSRR |= (1u << 8); // STBY

	_GPIOB->BSRR |= (1u << 4); // AIN1 high
	_GPIOB->BSRR |= (1u << 21); // AIN2 low
}

void test_motor_CW(void) {
	_GPIOB->BSRR |= (1u << 8);
	_GPIOB->BSRR |= (1u << 20);
	_GPIOB->BSRR |= (1u << 5);
}

void test_motor_BRAKE(void) {
	_GPIOB->BSRR |= (1u << 8);
	_GPIOB->BSRR |= (1u << 20);
	_GPIOB->BSRR |= (1u << 21);
}

void test_motor_OFF(void) {
	_GPIOB->BSRR |= (1u << 24);
	_GPIOB->BSRR |= (1u << 20);
	_GPIOB->BSRR |= (1u << 21);
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

