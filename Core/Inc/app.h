/*
 * app.h
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdint.h>

#include "my_types.h"

#include "rcc.h"
#include "usart2.h"
#include "pwm.h"
#include "systick.h"
#include "motor.h"

#define WHEEL_LEFT_TIM3_CNT ((uint32_t*) 0x40000424)
#define WHEEL_RIGHT_TIM4_CNT ((uint32_t*) 0x40000824)

void init_state(State*);

typedef struct {
	PidState* state;
} PidContext;

void pid(volatile PidState* state);
void pid_callback(void* context);

// To-do:
// Split pid fn parameters, split separate pid state from overall state
// Set desired speeds for turning to be enacted by PID instead of manual PWM
// Better printing telemetry
// Remove constants to separate file named constants

#endif /* INC_APP_H_ */
