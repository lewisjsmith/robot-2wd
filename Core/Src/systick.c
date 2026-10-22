/*
 * systick.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>

#include "stm32f446xx.h"

#include "my_types.h"
#include "systick.h"

void init_systick(void){
	// Set to 1ms
	SysTick->LOAD = 16000;
	SysTick->VAL = 0;
	SysTick->CTRL |= (1u << 2);
	SysTick->CTRL |= (1u << 1);
	SysTick->CTRL |= (1u << 0);
}

volatile uint32_t SYSTICK_MILLISECONDS = 0;

void SysTick_Handler(void) {
	SYSTICK_MILLISECONDS += 1;
}

void delay_ms(uint32_t milliseconds) {
	uint32_t start = SYSTICK_MILLISECONDS;
	while((SYSTICK_MILLISECONDS - start) < milliseconds);
}

void scheduler(
		uint32_t* sample_ticker,
		uint32_t sample_period,
		callback_fn fn,
		void* context) {
	if(SYSTICK_MILLISECONDS - *sample_ticker >= sample_period) {
		fn(context);
		*sample_ticker = SYSTICK_MILLISECONDS;
	}
}

