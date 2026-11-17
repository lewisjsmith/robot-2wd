/*
 * tim.h
 *
 *  Created on: Nov 17, 2026
 *      Author: lewis
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#include "regs.h"

void tim_pwm_init        (volatile _TIM_struct* tim, uint16_t psc, uint32_t arr);
void tim_pwm_channel_init(volatile _TIM_struct* tim, uint8_t ch);
void tim_pwm_set         (volatile _TIM_struct* tim, uint8_t ch, uint32_t ccr);
void tim_encoder_init    (volatile _TIM_struct* tim);

#endif /* INC_TIM_H_ */
