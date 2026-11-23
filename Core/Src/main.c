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

int main(void) {
	board_init();
	init_systick();
	init_usart2(115200);
	app_init();

	while(1){
		app_step();
	};
}
