/*
 * board.c
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

// See F446RE data sheets for available wiring options; alternate-function table

#include "board.h"

#include "regs.h"
#include "tim.h"

State state = {

	.SPEED_SAMPLE_PRINT_PERIOD_MS = 200,
	.SPEED_SAMPLE_PERIOD_MS = 4,

	.MOTOR_LEFT = {
		.MODE 					= BRAKE,

		.MOTOR_OUT_1_GPIO       = _GPIOB,
		.MOTOR_OUT_1_GPIO_PIN   = 4,
		.MOTOR_OUT_2_GPIO       = _GPIOB,
		.MOTOR_OUT_2_GPIO_PIN   = 5,

		.MOTOR_PWM_TIMER        = SHARED_PWM_TIMER,
		.MOTOR_PWM_CHANNEL      = 2,
		.MOTOR_PWM_CHANNEL_GPIO = _GPIOB,
		.MOTOR_PWM_CHANNEL_PIN  = 9,

		.POLARITY 				= 0,
	},

	.ENCODER_LEFT = {
		.MOTOR_ENCODER_TIMER          = _TIM3,
		.MOTOR_ENCODER_CHANNEL_1_GPIO = _GPIOA,
		.MOTOR_ENCODER_CHANNEL_1_PIN  = 6,
		.MOTOR_ENCODER_CHANNEL_2_GPIO = _GPIOA,
		.MOTOR_ENCODER_CHANNEL_2_PIN  = 7,

		.ENCODER_COUNT_PREV = 0,
		.SPEED_ACTUAL = 0.0f,
	},

	.MOTOR_RIGHT = {
		.MODE 					= BRAKE,

		.MOTOR_OUT_1_GPIO       = _GPIOB,
		.MOTOR_OUT_1_GPIO_PIN   = 13,
		.MOTOR_OUT_2_GPIO       = _GPIOB,
		.MOTOR_OUT_2_GPIO_PIN   = 14,

		.MOTOR_PWM_TIMER        = SHARED_PWM_TIMER,
		.MOTOR_PWM_CHANNEL      = 3,
		.MOTOR_PWM_CHANNEL_GPIO = _GPIOB,
		.MOTOR_PWM_CHANNEL_PIN  = 10,

		.POLARITY 				= 1,
	},

	.ENCODER_RIGHT = {
		.MOTOR_ENCODER_TIMER          = _TIM4,
		.MOTOR_ENCODER_CHANNEL_1_GPIO = _GPIOB,
		.MOTOR_ENCODER_CHANNEL_1_PIN  = 6,
		.MOTOR_ENCODER_CHANNEL_2_GPIO = _GPIOB,
		.MOTOR_ENCODER_CHANNEL_2_PIN  = 7,

		.ENCODER_COUNT_PREV = 0,
		.SPEED_ACTUAL = 0.0f,
	},
};

void board_init(void) {
	// 1. Clocks
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOAEN;
	_RCC->AHB1ENR |= _RCC_AHB1ENR_GPIOBEN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM2EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM3EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_TIM4EN;
	_RCC->APB1ENR |= _RCC_APB1ENR_USART2EN;

	// 2. Shared resources
	tim_pwm_init(SHARED_PWM_TIMER, SHARED_PWM_PSC, SHARED_PWM_ARR);
	init_motor_driver_stby();

	// 3. Motors
	motor_init(&state.MOTOR_LEFT);
	motor_init(&state.MOTOR_RIGHT);

	// 4. Sensors
	encoder_init(&state.ENCODER_LEFT);
	encoder_init(&state.ENCODER_RIGHT);
}
