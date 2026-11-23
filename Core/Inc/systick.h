/*
 * systick.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include <stdint.h>

extern volatile uint32_t SYSTICK_MILLISECONDS;

void init_systick(void);
void delay_ms(uint32_t);

#endif /* INC_SYSTICK_H_ */
