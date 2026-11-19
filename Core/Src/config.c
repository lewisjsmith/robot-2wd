/*
 * config.c
 *
 *  Created on: Nov 9, 2026
 *      Author: lewis
 */

#include "config.h"
#include "math.h"
#include "stdint.h"

State state = {

	.SPEED_SAMPLE_PRINT_PERIOD_MS = 200,
	.SPEED_SAMPLE_PERIOD_MS = 4,

	.MOTOR_LEFT = {
		.MODE 					= BRAKE,

		.MOTOR_OUT_1_GPIO       = _GPIOB,
		.MOTOR_OUT_1_GPIO_PIN   = 4,
		.MOTOR_OUT_2_GPIO       = _GPIOB,
		.MOTOR_OUT_2_GPIO_PIN   = 5,

		.MOTOR_PWM_TIMER        = _TIM2,
		.MOTOR_PWM_CHANNEL      = 2,
		.MOTOR_PWM_CHANNEL_GPIO = _GPIOB,
		.MOTOR_PWM_CHANNEL_PIN  = 9,

		.ENCODER = {
			.MOTOR_ENCODER_TIMER          = _TIM3,
			.MOTOR_ENCODER_CHANNEL_1_GPIO = _GPIOA,
			.MOTOR_ENCODER_CHANNEL_1_PIN  = 6,
			.MOTOR_ENCODER_CHANNEL_2_GPIO = _GPIOA,
			.MOTOR_ENCODER_CHANNEL_2_PIN  = 7,

			.ENCODER_COUNT_PREV = 0,
			.SPEED_ACTUAL = 0.0f,
		},

		.SPEED_DESIRED = 0.157080f,

		.PID = {
			.INTEGRAL 	= 0.0f,
			.PREV_ERROR = 0.0f,
			.Kp 		= 1500.0f,
			.Ki 		= 1000.0f,
			.Kd 		= 0.0f,
			.OUT_MIN	= 0,
			.OUT_MAX	= 800,
		},
	},

	.MOTOR_RIGHT = {
		.MODE 					= BRAKE,

		.MOTOR_OUT_1_GPIO       = _GPIOB,
		.MOTOR_OUT_1_GPIO_PIN   = 13,
		.MOTOR_OUT_2_GPIO       = _GPIOB,
		.MOTOR_OUT_2_GPIO_PIN   = 14,

		.MOTOR_PWM_TIMER        = _TIM2,
		.MOTOR_PWM_CHANNEL      = 3,
		.MOTOR_PWM_CHANNEL_GPIO = _GPIOB,
		.MOTOR_PWM_CHANNEL_PIN  = 10,

		.ENCODER = {
			.MOTOR_ENCODER_TIMER          = _TIM4,
			.MOTOR_ENCODER_CHANNEL_1_GPIO = _GPIOB,
			.MOTOR_ENCODER_CHANNEL_1_PIN  = 6,
			.MOTOR_ENCODER_CHANNEL_2_GPIO = _GPIOB,
			.MOTOR_ENCODER_CHANNEL_2_PIN  = 7,

			.ENCODER_COUNT_PREV = 0,
			.SPEED_ACTUAL = 0.0f,
		},

		.SPEED_DESIRED = 0.157080f,

		.PID = {
			.INTEGRAL 	= 0.0f,
			.PREV_ERROR = 0.0f,
			.Kp 		= 1500.0f,
			.Ki 		= 1000.0f,
			.Kd 		= 0.0f,
			.OUT_MIN	= 0,
			.OUT_MAX	= 800,
		},

	},
};




















