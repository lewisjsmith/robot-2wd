/*
 * gpio.h
 *
 *  Created on: Nov 17, 2026
 *      Author: lewis
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdbool.h>

#include "regs.h"

typedef enum {
	INPUT = 0u,
	OUTPUT = 1u,
	AF = 2u,
	ANALOG = 3u,
} GpioMode;

void gpio_set_mode(volatile _GPIO_struct* port, uint8_t pin, GpioMode mode);
void gpio_set_af(volatile _GPIO_struct* port, uint8_t pin, uint8_t af);
void gpio_write(volatile _GPIO_struct* port, uint8_t pin, bool high);

#endif /* INC_GPIO_H_ */
