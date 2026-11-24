/*
 * motor.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdbool.h>

#include "regs.h"

typedef enum {
	DRIVE,
	BRAKE,
	REVERSE
} MotorMode;

typedef struct {
	volatile _GPIO_struct* 	MOTOR_OUT_1_GPIO;
	uint8_t 				MOTOR_OUT_1_GPIO_PIN;
	volatile _GPIO_struct* 	MOTOR_OUT_2_GPIO;
	uint8_t 				MOTOR_OUT_2_GPIO_PIN;

	volatile _TIM_struct* 	MOTOR_PWM_TIMER;
	volatile _GPIO_struct* 	MOTOR_PWM_CHANNEL_GPIO;
	uint8_t 				MOTOR_PWM_CHANNEL;
	uint8_t 				MOTOR_PWM_CHANNEL_PIN;

	MotorMode MODE;
	bool 	POLARITY;
} Motor;

void init_motor_driver_gpio(Motor* motor);
void init_motor_pwm_gpio(Motor* motor);
void init_motor_driver_stby(void);
void motor_init(Motor* motor);
void motor_set_duty(Motor* m, float ccr);
void set_wheel_direction(Motor* motor, MotorMode direction);

#endif /* INC_MOTOR_H_ */
