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

void init_state(State* state) {
	state->WHEEL_LEFT_ENCODER_COUNT_ADDR = WHEEL_LEFT_TIM3_CNT;
	state->WHEEL_LEFT_ENCODER_COUNT_PREV = 0;
	state->SPEED = 0.0f;
	state->SPEED_SAMPLE_PERIOD_MS = 4;
	state->SPEED_SAMPLE_PRINT_PERIOD_MS = 100;

	// PID, split into another object?
	state->SPEED_DESIRED = 0.17f;
	state->PID_PREV_MS = SYSTICK_MILLISECONDS;
	state->PID_PREV_ERROR = 0.0f;
	state->PID_SAMPLE_PERIOD_MS = 4;
	state->PID_INTEGRAL = 30.0f;
	state->Kp = 200.0f;
	state->Kd = 25.0f;
	state->Ki = 80.0f;
}

// Set desired speed via uart and assign it to an state variable
// Either with a scheduler or on the main loop, run one loop of a PID
// Repeat

void pid_callback(void* context) {
	PidContext* pc = (PidContext*)context;
	pid(pc->state);
}

// Note that PWM below 10 isn't enough to turn the wheel, may need to adjust for this
void pid(volatile State* state) {
	uint32_t now = SYSTICK_MILLISECONDS;
	uint32_t prev = state->PID_PREV_MS;
	float dt = (now - prev)/1000.0f;
	if(dt == 0.0f) return;

	float error = (state->SPEED_DESIRED - state->SPEED)/0.3f;

	float error_p = error * state->Kp;
	float error_d = ((error - state->PID_PREV_ERROR)/dt) * state->Kd;

	// Integral clamp
	state->PID_INTEGRAL += error * dt * state->Ki;
	if(state->PID_INTEGRAL > 100.0f) state->PID_INTEGRAL = 100.0f;
	else if(state->PID_INTEGRAL < 0.0f) state->PID_INTEGRAL = 0.0f;

	float error_i = state->PID_INTEGRAL;
	float error_sum = error_p + error_d + error_i;

	// PWM Clamp
	if(error_sum > 100.0) error_sum = 100.0;
	else if(error_sum < 0) error_sum = 0.0;

	// TODO: Join get_speed and PID
	set_duty(&_TIM2->ARR, &_TIM2->CCR2, (int)error_sum);

	state->PID_PREV_MS = now;
	state->PID_PREV_ERROR = error;
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

