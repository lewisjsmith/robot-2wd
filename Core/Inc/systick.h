/*
 * systick.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include <stdint.h>

uint32_t systick_ms(void);

void init_systick(void);
void delay_ms(uint32_t);

#endif /* INC_SYSTICK_H_ */
