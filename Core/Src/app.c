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
#include <stdint.h>

#include "motor.h"

void speed_callback(void* context) {
	SpeedContext* sc = (SpeedContext*)context;

	uint32_t now = SYSTICK_MILLISECONDS;
	uint32_t prev = sc->PREV_SAMPLE_TICK_MS;
	float dt = (now - prev);
	if(dt == 0.0f) return;

	// Speed step
	uint32_t prev_count = *(sc->PREV_COUNT);
	uint32_t curr_count = *(sc->CURR_COUNT);
	float speed = get_speed_from_encoder(prev_count, curr_count, dt);
	*(sc->SPEED_ACTUAL) = speed;
	*(sc->PREV_COUNT) = curr_count;

	sc->PREV_SAMPLE_TICK_MS = now;

	if(*(sc->MODE) == BREAK) return;

	// PID step)
	pid_step(sc->PID, dt/1000.0f, sc->SPEED_DESIRED, sc->SPEED_ACTUAL);
	float output = sc->PID->OUTPUT;
	if(output > 100.0f) output = 100.0f;
	else if(output < 0.0f) output = 0.0f;
	set_duty(sc->WHEEL_ARR_ADDR, sc->WHEEL_DUTY_ADDR, (int)output);


}

// PWM below 10 isn't sufficient to turn the wheel
// Normalise through constants, 0.3f division required as this is the maximum unloaded speed
void pid_step(Pid* pid, float dt, float* desired, float* actual) {
	float error = *desired - *actual;
	float error_p = error * pid->Kp;
	float error_d = ((error - pid->PREV_ERROR)/dt) * pid->Kd;

	pid->INTEGRAL += error * dt * pid->Ki;
	if(pid->INTEGRAL > pid->OUT_MAX) pid->INTEGRAL = pid->OUT_MAX;
	else if(pid->INTEGRAL < pid->OUT_MIN) pid->INTEGRAL = pid->OUT_MIN;
	float error_i = pid->INTEGRAL;

	pid->PREV_ERROR = error;
	pid->OUTPUT = (error_p + error_d + error_i);
}



