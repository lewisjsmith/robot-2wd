/*
 * encoder.c
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

#include "encoder.h"

#include "gpio.h"
#include "tim.h"
#include "robot_params.h"

// AF2 hardcoded
void init_encoder_gpio(Encoder* encoder) {
	 // PA6 Channel 1, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, encoder->MOTOR_ENCODER_CHANNEL_1_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, encoder->MOTOR_ENCODER_CHANNEL_1_PIN, 2);

	// PA7 Channel 2, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, encoder->MOTOR_ENCODER_CHANNEL_2_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, encoder->MOTOR_ENCODER_CHANNEL_2_PIN, 2);
}

void encoder_init(Encoder* encoder) {
	tim_encoder_init(encoder->MOTOR_ENCODER_TIMER);
	init_encoder_gpio(encoder);
}

float get_speed_from_encoder(volatile uint32_t prev_cnt, volatile uint32_t curr_cnt, uint32_t period_ms) {
	uint16_t prev = prev_cnt;
	uint16_t curr = curr_cnt;
	int16_t diff = curr - prev;

	if(diff < 0) diff *= -1;

	float speed = (((WHEEL_DIAMETER * PI)/COUNTS_PER_REV) * diff)/(period_ms / 1000.0f);
	return speed;
}
