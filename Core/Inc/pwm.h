/*
 * pwm.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "my_types.h"
#include "motor.h"

void init_pwm_gpiob(void);
void init_pwm_tim2(void);

void init_motor_pwm_timer(Motor* motor);
void init_motor_pwm_gpio(Motor* motor);

#endif /* INC_PWM_H_ */
