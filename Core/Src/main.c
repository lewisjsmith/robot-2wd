/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "config.h"
#include "app.h"

int main(void) {
	init_rcc();
	init_systick();

	init_usart2();

	init_motor_driver_gpio(&state.MOTOR_LEFT);
	init_motor_driver_gpio(&state.MOTOR_RIGHT);

	init_motor_pwm_gpio(&state.MOTOR_LEFT);
	init_motor_pwm_timer(&state.MOTOR_LEFT);

	init_motor_pwm_gpio(&state.MOTOR_RIGHT);
	init_motor_pwm_timer(&state.MOTOR_RIGHT);

	// TODO: add timer to name
	init_encoder_gpio_timer(&state.MOTOR_LEFT.ENCODER);
	init_encoder_gpio_timer(&state.MOTOR_RIGHT.ENCODER);

	PrintContext pc;
	pc.WHEEL_LEFT_SPEED = &state.MOTOR_LEFT.ENCODER.SPEED_ACTUAL;
	pc.WHEEL_RIGHT_SPEED = &state.MOTOR_RIGHT.ENCODER.SPEED_ACTUAL;

	SpeedContext MOTOR_LEFT_CONTEXT;
	MOTOR_LEFT_CONTEXT.PREV_COUNT = &state.MOTOR_LEFT.ENCODER.ENCODER_COUNT_PREV;
	MOTOR_LEFT_CONTEXT.CURR_COUNT = &state.MOTOR_LEFT.ENCODER.MOTOR_ENCODER_TIMER->CNT;
	MOTOR_LEFT_CONTEXT.SPEED_ACTUAL = &state.MOTOR_LEFT.ENCODER.SPEED_ACTUAL;
	MOTOR_LEFT_CONTEXT.SPEED_DESIRED = &state.MOTOR_LEFT.SPEED_DESIRED;
	MOTOR_LEFT_CONTEXT.PID = &state.MOTOR_LEFT.PID;
	MOTOR_LEFT_CONTEXT.PREV_SAMPLE_TICK_MS = SYSTICK_MILLISECONDS;
	MOTOR_LEFT_CONTEXT.WHEEL_ARR_ADDR = &state.MOTOR_LEFT.MOTOR_PWM_TIMER->ARR;
	MOTOR_LEFT_CONTEXT.WHEEL_DUTY_ADDR = &state.MOTOR_LEFT.MOTOR_PWM_TIMER->CCR2;
	MOTOR_LEFT_CONTEXT.MODE = &state.MOTOR_LEFT.MODE;

	SpeedContext MOTOR_RIGHT_CONTEXT;
	MOTOR_RIGHT_CONTEXT.PREV_COUNT = &state.MOTOR_RIGHT.ENCODER.ENCODER_COUNT_PREV;
	MOTOR_RIGHT_CONTEXT.CURR_COUNT = &state.MOTOR_RIGHT.ENCODER.MOTOR_ENCODER_TIMER->CNT;
	MOTOR_RIGHT_CONTEXT.SPEED_ACTUAL = &state.MOTOR_RIGHT.ENCODER.SPEED_ACTUAL;
	MOTOR_RIGHT_CONTEXT.SPEED_DESIRED = &state.MOTOR_RIGHT.SPEED_DESIRED;
	MOTOR_RIGHT_CONTEXT.PID = &state.MOTOR_RIGHT.PID;
	MOTOR_RIGHT_CONTEXT.PREV_SAMPLE_TICK_MS = SYSTICK_MILLISECONDS;
	MOTOR_RIGHT_CONTEXT.WHEEL_ARR_ADDR = &state.MOTOR_RIGHT.MOTOR_PWM_TIMER->ARR;
	MOTOR_RIGHT_CONTEXT.WHEEL_DUTY_ADDR = &state.MOTOR_RIGHT.MOTOR_PWM_TIMER->CCR3;
	MOTOR_RIGHT_CONTEXT.MODE = &state.MOTOR_RIGHT.MODE;

	uint32_t speed_ticker_left = 0;
	uint32_t speed_ticker_right = 0;
	uint32_t speed_print_ticker = 0;

// 	No response reaction
//	uint32_t stale_timer = SYSTICK_MILLISECONDS;
//	uint32_t stale_threshold_ms = 500;
//	uint32_t timed_out = false;
//	// main loop...
//	if(!timed_out && SYSTICK_MILLISECONDS - stale_timer >= stale_threshold_ms) {
//		drive_stop();
//		timed_out = true;
//	}
//	// within the pending_cmd scope
//	stale_timer = SYSTICK_MILLISECONDS;
//	timed_out = false;

	while(1){
		if(pending_cmd) {
			wasd_control(pending_cmd);

			pending_cmd = 0;
		}

		scheduler(
					&speed_ticker_left,
					4,
					speed_callback,
					&MOTOR_LEFT_CONTEXT
		);

		scheduler(
					&speed_ticker_right,
					4,
					speed_callback,
					&MOTOR_RIGHT_CONTEXT
		);

		scheduler(
					&speed_print_ticker,
					state.SPEED_SAMPLE_PRINT_PERIOD_MS,
					usart2_print_speed_callback,
					&pc
		);

	};
}
