/*
 * board.h
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include <stdint.h>

#include "motor.h"

#define SHARED_PWM_TIMER 		_TIM2
#define SHARED_PWM_PSC			0
#define SHARED_PWM_ARR			(800 - 1)

typedef struct {
	Motor				MOTOR_LEFT;
	Motor				MOTOR_RIGHT;
	Encoder				ENCODER_LEFT;
	Encoder				ENCODER_RIGHT;
	uint32_t			SPEED_SAMPLE_PERIOD_MS;
	uint32_t			SPEED_SAMPLE_PRINT_PERIOD_MS;
} State;
extern State state;

void board_init(void);

#endif /* INC_BOARD_H_ */
