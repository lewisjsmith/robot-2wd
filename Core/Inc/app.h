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
#include "encoder.h"
#include "pid.h"

typedef struct {
	Motor* 			motor;
	Encoder*		encoder;
	Pid 			pid;
	float 			SPEED_DESIRED;
	uint32_t		ticker;
} Wheel;

void speed_callback(void*);

void app_init(void);
void app_step(void);

void drive_forwards(void);
void drive_backwards(void);
void drive_left(void);
void drive_right(void);
void drive_stop(void);
void drive_disconnect(void);

void set_wheel_mode(Wheel*, MotorMode);

typedef void (* callback_fn)(void* context);

#endif /* INC_APP_H_ */
