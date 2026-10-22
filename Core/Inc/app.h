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

State init_state(State*);
float pid(float);

#endif /* INC_APP_H_ */
