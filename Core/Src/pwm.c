/*
 * pwm.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>

#include "my_types.h"
#include "pwm.h"

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
