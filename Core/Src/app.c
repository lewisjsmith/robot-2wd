/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#include <stdio.h>

#include <stdbool.h>
#include <math.h>

#include "app.h"

State init_state(State* state) {
	state->WHEEL_LEFT_ENCODER_COUNT_ADDR = WHEEL_LEFT_TIM3_CNT;
	state->WHEEL_LEFT_ENCODER_COUNT_PREV = 0;
	state->SPEED = 0.0;
	state->SPEED_SAMPLE_PERIOD_MS = 10;
	state->SPEED_SAMPLE_PRINT_PERIOD_MS = 1000;
}

float get_error(float want, float* got) {
	return want - *got;
}

float pid(float error) {
	float Kp = 1;
	float force = Kp * error;
	if(force > 100.0) force = 100.0;
	else if(force < 0.0) force = 0;
	return force;
}


























// PB9 TIM2_CH2 and PB10 TIM2_CH3, driving PWMA and PWMB

// PB4, PB5, PB13, PB14 for AIN1, AIN2, BIN1, BIN2,

// PB8 for STBY

// UART2 for debugging
// Enable NVIC interrupt
// Write interrupt handler, keep it short

// TIM2 for everything
// Enable NVIC interrupt
// Write interrupt handler, keep it short

// SysTick for delays

