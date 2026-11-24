/*
 * encoder.h
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "regs.h"

typedef struct {
	volatile _TIM_struct* 	MOTOR_ENCODER_TIMER;
	volatile _GPIO_struct* 	MOTOR_ENCODER_CHANNEL_1_GPIO;
	uint8_t 				MOTOR_ENCODER_CHANNEL_1_PIN;
	volatile _GPIO_struct* 	MOTOR_ENCODER_CHANNEL_2_GPIO;
	uint8_t 				MOTOR_ENCODER_CHANNEL_2_PIN;

	uint32_t 				ENCODER_COUNT_PREV;
	float 					SPEED_ACTUAL;
} Encoder;

void encoder_init(const Encoder* encoder);
float get_speed_from_encoder(uint32_t prev_cnt, volatile uint32_t curr_cnt, uint32_t period_ms);

#endif /* INC_ENCODER_H_ */
