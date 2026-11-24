/*
 * tim.c
 *
 *  Created on: Nov 17, 2026
 *      Author: lewis
 */

#include "tim.h"

void tim_pwm_init(volatile _TIM_struct* tim, uint16_t psc, uint32_t arr) {
	tim->PSC = psc;
	tim->ARR = arr;
	tim->CR1 |= (1u << 7);
	tim->EGR = (1u << 0);
	tim->SR = ~(1u << 0);
	tim->CR1 |= (1u << 0);
}

void tim_pwm_set(volatile _TIM_struct* tim, uint8_t ch, uint32_t ccr) {
	if(ch > 4 || ch == 0) return;

	if(ch == 1) {
		tim->CCR1 = ccr;
	}
	else if(ch == 2) {
		tim->CCR2 = ccr;
	}
	else if(ch == 3) {
		tim->CCR3 = ccr;
	}
	else if(ch == 4) {
		tim->CCR4 = ccr;
	}
}

void tim_pwm_channel_init(volatile _TIM_struct* tim, uint8_t ch) {
	if(ch > 4 || ch == 0) return;

	if(ch == 1) {
		tim->CCMR1 &= ~(3u << 0); // Set as output
		tim->CCMR1 &= ~(7u << 4);
		tim->CCMR1 |= (6u << 4);
		tim->CCMR1 |= (1u << 3);
		tim->CCER |= (1u << 0);
		tim_pwm_set(tim, 1, 0);
	}
	else if (ch == 2) {
		tim->CCMR1 &= ~(3u << 8);
		tim->CCMR1 &= ~(7u << 12);
		tim->CCMR1 |= (6u << 12);
		tim->CCMR1 |= (1u << 11);
		tim->CCER |= (1u << 4);
		tim_pwm_set(tim, 2, 0);
	}
	else if (ch == 3) {
		tim->CCMR2 &= ~(3u << 0);
		tim->CCMR2 &= ~(7u << 4);
		tim->CCMR2 |= (6u << 4);
		tim->CCMR2 |= (1u << 3);
		tim->CCER |= (1u << 8);
		tim_pwm_set(tim, 3, 0);
	}
	else if (ch == 4) {
		tim->CCMR2 &= ~(3u << 8);
		tim->CCMR2 &= ~(7u << 12);
		tim->CCMR2 |= (6u << 12);
		tim->CCMR2 |= (1u << 11);
		tim->CCER |= (1u << 12);
		tim_pwm_set(tim, 4, 0);
	}
}

void tim_encoder_init(volatile _TIM_struct* tim) {
	tim->CCMR1 &= ~(3u << 0);
	tim->CCMR1 |= (1u << 0); // Input IC1, mapped to TI1 (Timer channel 1)
	tim->CCMR1 &= ~(3u << 8);
	tim->CCMR1 |= (1u << 8); // Input IC2, mapped to TI2 (Timer channel 1)

	tim->CCMR1 &= ~(3u << 2); // Capture on each edge
	tim->CCMR1 &= ~(15u << 4);
	tim->CCMR1 |= (3u << 4); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	tim->CCMR1 &= ~(3u << 10); // Capture on each edge
	tim->CCMR1 &= ~(15u << 12);
	tim->CCMR1 |= (3u << 12); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	tim->SMCR &= ~(7u << 0);
	tim->SMCR |= (3u << 0); // Encoder Mode 3, both rising and falling edges trigger counts depending on the others level

	tim->EGR = (1u << 0);
	tim->SR = ~(1u << 0);

	tim->CR1 |= (1u << 0); // CEN
}















