/*
 * pid.h
 *
 *  Created on: Nov 18, 2026
 *      Author: lewis
 */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct {
	volatile float 		INTEGRAL;
	float 				PREV_ERROR;
	float				OUTPUT;
	float 				Kp;
	float			 	Ki;
	float 				Kd;
	float				OUT_MIN;
	float				OUT_MAX;
} Pid;

void pid_step(Pid*, float, float*, float*);

#endif /* INC_PID_H_ */
