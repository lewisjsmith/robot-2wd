/*
 * app.h
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdint.h>

#include "motor.h"

#define WHEEL_LEFT_TIM3_CNT ((uint32_t*) 0x40000424)
#define WHEEL_RIGHT_TIM4_CNT ((uint32_t*) 0x40000824)

void speed_callback(void*);

void drive_forwards(void);
void drive_backwards(void);
void drive_left(void);
void drive_right(void);
void drive_stop(void);
void drive_disconnect(void);

void set_wheel_mode(Motor*, uint8_t, MotorMode);

typedef struct {
	volatile float* WHEEL_LEFT_SPEED;
	volatile float* WHEEL_RIGHT_SPEED;
} PrintContext;


#endif /* INC_APP_H_ */
