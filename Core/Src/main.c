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
//#include "config.h"
#include "app.h"
#include "usart2.h"
#include "teleop.h"
#include "board.h"
#include "systick.h"

#include "tim.h"

typedef struct {
	Motor 	motor;
	Encoder	encoder;
	bool	polarity;
	Pid 	pid;
} Wheel;

int main(void) {
	board_init();
	init_systick();
	init_usart2(115200);

	PrintContext pc;
	pc.WHEEL_LEFT_SPEED = &state.MOTOR_LEFT.ENCODER.SPEED_ACTUAL;
	pc.WHEEL_RIGHT_SPEED = &state.MOTOR_RIGHT.ENCODER.SPEED_ACTUAL;

	uint32_t speed_print_ticker = 0;


	while(1){
		char next_cmd = 0;
		if(usart2_try_read(&next_cmd)) {
			wasd_control(next_cmd);
		}

		scheduler(
					&state.MOTOR_LEFT.TICKER,
					state.SPEED_SAMPLE_PERIOD_MS,
					speed_callback,
					&state.MOTOR_LEFT
		);

		scheduler(
					&state.MOTOR_RIGHT.TICKER,
					state.SPEED_SAMPLE_PERIOD_MS,
					speed_callback,
					&state.MOTOR_RIGHT
		);

		scheduler(
					&speed_print_ticker,
					state.SPEED_SAMPLE_PRINT_PERIOD_MS,
					usart2_print_speed_callback,
					&pc
		);

	};
}
