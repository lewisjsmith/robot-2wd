/*
 * motor.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdio.h>
#include <stdint.h>

#include "motor.h"

#include "gpio.h"
#include "tim.h"

void init_motor_pwm_gpio(Motor* motor) {
	gpio_set_mode(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, AF);
	gpio_set_af(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, 1);
}

void init_motor_driver_gpio(Motor* motor) {
	gpio_set_mode(motor->MOTOR_OUT_1_GPIO, motor->MOTOR_OUT_1_GPIO_PIN, OUTPUT);
	gpio_set_mode(motor->MOTOR_OUT_2_GPIO, motor->MOTOR_OUT_2_GPIO_PIN, OUTPUT);
}

void init_motor_driver_stby(void) {
	gpio_set_mode(_GPIOB, 8, OUTPUT);
}

void motor_init(Motor* motor) {
	init_motor_driver_gpio(motor);
	tim_pwm_channel_init(motor->MOTOR_PWM_TIMER, motor->MOTOR_PWM_CHANNEL);
	init_motor_pwm_gpio(motor);
}

void motor_set_duty(Motor* m, float ccr) {
    uint32_t arr = m->MOTOR_PWM_TIMER->ARR;        // or MOTOR_PWM_ARR
    if (ccr < 0.0f) ccr = 0.0f;
    if (ccr > arr)  ccr = arr;
    tim_pwm_set(m->MOTOR_PWM_TIMER, m->MOTOR_PWM_CHANNEL, (uint32_t)(ccr + 0.5f));
}

void set_wheel_direction(Motor* motor, MotorMode direction) {
	if(direction == DRIVE) {
		gpio_write(
				motor->MOTOR_OUT_1_GPIO,
				motor->MOTOR_OUT_1_GPIO_PIN,
				motor->POLARITY
		);

		gpio_write(
				motor->MOTOR_OUT_2_GPIO,
				motor->MOTOR_OUT_2_GPIO_PIN,
				!motor->POLARITY
		);
	}
	else if(direction == REVERSE) {
		gpio_write(
				motor->MOTOR_OUT_1_GPIO,
				motor->MOTOR_OUT_1_GPIO_PIN,
				!motor->POLARITY
		);

		gpio_write(
				motor->MOTOR_OUT_2_GPIO,
				motor->MOTOR_OUT_2_GPIO_PIN,
				motor->POLARITY
		);
	}
	else if(direction == BRAKE) {
		gpio_write(
				motor->MOTOR_OUT_1_GPIO,
				motor->MOTOR_OUT_1_GPIO_PIN,
				1
		);

		gpio_write(
				motor->MOTOR_OUT_2_GPIO,
				motor->MOTOR_OUT_2_GPIO_PIN,
				1
		);
	}
}


