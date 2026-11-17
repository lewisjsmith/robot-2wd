/*
 * gpio.c
 *
 *  Created on: Nov 17, 2026
 *      Author: lewis
 */

#include "gpio.h"

void gpio_set_mode(volatile _GPIO_struct* port, uint8_t pin, GpioMode mode) {
	port->MODER &= ~(3u << pin*2);
	port->MODER |= (mode << pin*2);
}

void gpio_set_af(volatile _GPIO_struct* port, uint8_t pin, uint8_t af) {
	if(pin > 15) return;
	if(af > 15) return;

	volatile uint32_t* AF_reg;
	uint8_t pin_shift = pin;

	if(pin < 8) AF_reg = &port->AFRL;
	else {
		AF_reg = &port->AFRH;
		pin_shift = pin - 8;
	}

	*AF_reg &= ~(15u << (4 * pin_shift));
	*AF_reg |= (af << (4 * pin_shift));
}

void gpio_write(volatile _GPIO_struct* port, uint8_t pin, bool high) {
	if(pin > 15) return;

	if(high) {
		port->BSRR = 1u << pin;
	} else {
		port->BSRR = 1u << pin + 16u;
	}


}

