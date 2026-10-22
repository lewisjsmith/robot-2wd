/*
 * motor.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "my_types.h"

void init_mdriver_gpiob(void);
void drive_forwards(void);
void drive_backwards(void);
void drive_left(void);
void drive_right(void);
void drive_stop(void);
void drive_disconnect(void);
void set_duty(volatile uint32_t* arr_addr, volatile uint32_t* duty_addr, uint8_t percentage);
void set_wheel_direction(uint8_t wheel, uint8_t direction);
void init_encoder_gpio(void);

typedef struct {
	volatile uint32_t* prev_cnt;
	volatile uint32_t** curr_cnt;
	uint32_t* period_ms;
	volatile float *speed;
} SpeedContext;

float get_speed_from_encoder(volatile uint32_t*, volatile uint32_t*, uint32_t);
void get_speed_from_encoder_callback(void* context);

void wasd_control(char);

typedef struct {
	volatile float* speed;
} PrintContext;

void usart2_print_speed(volatile float);
void usart2_print_speed_callback(void* context);

#endif /* INC_MOTOR_H_ */
