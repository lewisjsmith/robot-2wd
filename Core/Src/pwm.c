/*
 * pwm.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdint.h>

#include "gpio.h"

#include "my_types.h"
#include "pwm.h"

void init_motor_pwm_timer(Motor* motor) {
	if(motor->MOTOR_PWM_CHANNEL == 1){
		motor->MOTOR_PWM_TIMER->CCMR1 &= ~(3u << 0); // Set as output
		motor->MOTOR_PWM_TIMER->CCMR1 &= ~(7u << 4); // Mode
		motor->MOTOR_PWM_TIMER->CCMR1 |= (6u << 4); // Mode
	}
	else if(motor->MOTOR_PWM_CHANNEL == 2){
		motor->MOTOR_PWM_TIMER->CCMR1 &= ~(3u << 8); // Set as output
		motor->MOTOR_PWM_TIMER->CCMR1 &= ~(7u << 12); // Mode
		motor->MOTOR_PWM_TIMER->CCMR1 |= (6u << 12); // Mode
	}
	else if(motor->MOTOR_PWM_CHANNEL == 3){
		motor->MOTOR_PWM_TIMER->CCMR2 &= ~(3u << 0); // "
		motor->MOTOR_PWM_TIMER->CCMR2 &= ~(7u << 4); // "
		motor->MOTOR_PWM_TIMER->CCMR2 |= (6u << 4); // "
	}
	else if(motor->MOTOR_PWM_CHANNEL == 4){
		motor->MOTOR_PWM_TIMER->CCMR2 &= ~(3u << 8); // "
		motor->MOTOR_PWM_TIMER->CCMR2 &= ~(7u << 12); // "
		motor->MOTOR_PWM_TIMER->CCMR2 |= (6u << 12); // "
	}

	motor->MOTOR_PWM_TIMER->CR1 |= (1u << 7); // ARPE

	if(motor->MOTOR_PWM_CHANNEL == 1){
		motor->MOTOR_PWM_TIMER->CCER |= (1u << 0);
	}
	else if(motor->MOTOR_PWM_CHANNEL == 2){
		motor->MOTOR_PWM_TIMER->CCER |= (1u << 4);
	}
	else if(motor->MOTOR_PWM_CHANNEL == 3){
		motor->MOTOR_PWM_TIMER->CCER |= (1u << 8);
	}
	else if(motor->MOTOR_PWM_CHANNEL == 4){
		motor->MOTOR_PWM_TIMER->CCER |= (1u << 12);
	}

	// 20kHz aim, 16MHz source, maintain resolution with ARR > PSC
	motor->MOTOR_PWM_TIMER->PSC = 0;
	motor->MOTOR_PWM_TIMER->ARR = 800 - 1;

	// Duty cycle
	motor->MOTOR_PWM_TIMER->CCR1 = 0;
	motor->MOTOR_PWM_TIMER->CCR2 = 0;
	motor->MOTOR_PWM_TIMER->CCR3 = 0;
	motor->MOTOR_PWM_TIMER->CCR4 = 0;

	motor->MOTOR_PWM_TIMER->EGR = (1u << 0);
	motor->MOTOR_PWM_TIMER->SR &= ~(1u << 0);

	motor->MOTOR_PWM_TIMER->CR1 |= (1u << 0); // CEN
}

void init_motor_pwm_gpio(Motor* motor) {
	gpio_set_mode(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, AF);
	gpio_set_af(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, 1);
}
