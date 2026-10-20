/*
 * rcc.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>

#include "my_types.h"
#include "rcc.h"

void init_rcc(void) {
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOAEN;
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOBEN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM2EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM3EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_USART2EN;
};
