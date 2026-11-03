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
	PidState pid_left;
	pid_left.WHEEL_ENCODER_COUNT_ADDR = WHEEL_LEFT_TIM3_CNT;
	pid_left.WHEEL_ENCODER_COUNT_PREV = 0;
	pid_left.WHEEL_DUTY_ADDR = &_TIM2->CCR2;
	pid_left.WHEEL_SPEED = 0.0f;
	pid_left.SPEED_DESIRED = 0.1f;
	pid_left.SAMPLE_PERIOD_MS = 4;
	pid_left.PREV_SAMPLE_TICK_MS = SYSTICK_MILLISECONDS;
	pid_left.INTEGRAL = 30.0f;
	pid_left.PREV_ERROR = 0.0f;
	pid_left.Kp = 200.0f;
	pid_left.Ki = 80.0f;
	pid_left.Kd = 25.0f;

	PidState pid_right;
	pid_right.WHEEL_ENCODER_COUNT_ADDR = WHEEL_RIGHT_TIM4_CNT;
	pid_right.WHEEL_ENCODER_COUNT_PREV = 0;
	pid_right.WHEEL_DUTY_ADDR = &_TIM2->CCR3;
	pid_right.WHEEL_SPEED = 0.0f;
	pid_right.SPEED_DESIRED = 0.1f;
	pid_right.SAMPLE_PERIOD_MS = 4;
	pid_right.PREV_SAMPLE_TICK_MS = SYSTICK_MILLISECONDS;
	pid_right.INTEGRAL = 30.0f;
	pid_right.PREV_ERROR = 0.0f;
	pid_right.Kp = 200.0f;
	pid_right.Ki = 80.0f;
	pid_right.Kd = 25.0f;

	state->PID_LEFT = pid_left;
	state->PID_RIGHT = pid_right;

	state->SPEED_SAMPLE_PERIOD_MS = 4;
	state->SPEED_SAMPLE_PRINT_PERIOD_MS = 100;
}

// Set desired speed via uart and assign it to an state variable
// Either with a scheduler or on the main loop, run one loop of a PID
// Repeat

void pid_callback(void* context) {
	PidContext* pc = (PidContext*)context;
	pid(pc->state);
}

// Note that PWM below 10 isn't enough to turn the wheel, may need to adjust for this
void pid(volatile PidState* state) {
	uint32_t now = SYSTICK_MILLISECONDS;
	uint32_t prev = state->PREV_SAMPLE_TICK_MS;
	float dt = (now - prev)/1000.0f;
	if(dt == 0.0f) return;

	float error = (state->SPEED_DESIRED - state->WHEEL_SPEED)/0.3f;

	float error_p = error * state->Kp;
	float error_d = ((error - state->PREV_ERROR)/dt) * state->Kd;

	// Integral clamp
	state->INTEGRAL += error * dt * state->Ki;
	if(state->INTEGRAL > 100.0f) state->INTEGRAL = 100.0f;
	else if(state->INTEGRAL < 0.0f) state->INTEGRAL = 0.0f;

	float error_i = state->INTEGRAL;
	float error_sum = error_p + error_d + error_i;

	// PWM Clamp
	if(error_sum > 100.0) error_sum = 100.0;
	else if(error_sum < 0) error_sum = 0.0;

	// TODO: Join get_speed and PID
	set_duty(&_TIM2->ARR, state->WHEEL_DUTY_ADDR, (int)error_sum);

	state->PREV_SAMPLE_TICK_MS = now;
	state->PREV_ERROR = error;
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

