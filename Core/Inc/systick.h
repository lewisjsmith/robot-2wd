/*
 * systick.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include "my_types.h"

extern volatile uint32_t SYSTICK_MILLISECONDS;

void init_systick(void);
void SysTick_Handler(void);
void delay_ms(uint32_t);

typedef void (* callback_fn)(void* context);

void scheduler(
		uint32_t* sample_ticker,
		uint32_t sample_rate,
		callback_fn fn,
		void* context);

#endif /* INC_SYSTICK_H_ */
