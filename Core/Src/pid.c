/*
 * pid.c
 *
 *  Created on: Nov 18, 2026
 *      Author: lewis
 */

#ifndef SRC_PID_C_
#define SRC_PID_C_

#include "pid.h"

// PWM below 10 isn't sufficient to turn the wheel
// Normalise through constants, 0.3f division required as this is the maximum unloaded speed
// Watch out for encoder quantisation noise amplification
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

#endif /* SRC_PID_C_ */
