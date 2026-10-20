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
float get_speed(uint16_t*, uint32_t);
void wasd_control(char);

#endif /* INC_MOTOR_H_ */
