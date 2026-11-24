/*
 * pid.h
 *
 *  Created on: Nov 18, 2026
 *      Author: lewis
 */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct {
	float 				INTEGRAL;
	float 				PREV_ERROR;
	float				OUTPUT;
	float 				Kp;
	float			 	Ki;
	float 				Kd;
	float				OUT_MIN;
	float				OUT_MAX;
} Pid;

void pid_step(Pid* pid, float dt, const float* desired, const float* actual);

#endif /* INC_PID_H_ */
