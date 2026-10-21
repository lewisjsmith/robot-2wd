/*
 * motor.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

// Refactor needed below

#include <stdint.h>

#include "motor.h"

#define PI 3.14159265358979323846
#define WHEEL_DIAMETER 0.064

void init_mdriver_gpiob(void) {
	// PB4 as output
	_GPIOB->MODER &= ~(3u << 8);
	_GPIOB->MODER |= (1u << 8);

	// PB5 as output
	_GPIOB->MODER &= ~(3u << 10);
	_GPIOB->MODER |= (1u << 10);

	// PB13 as output
	_GPIOB->MODER &= ~(3u << 26);
	_GPIOB->MODER |= (1u << 26);

	// PB14 as output
	_GPIOB->MODER &= ~(3u << 28);
	_GPIOB->MODER |= (1u << 28);

	// PB8 as output
	_GPIOB->MODER &= ~(3u << 16);
	_GPIOB->MODER |= (1u << 16);
}

// TI1FP1 and TI2FP2 refer to TI1, TI2 after the input filters and polarity selection
// Counts to/from ARR depending on direction
void init_encoder_gpio(void){
	_TIM3->CCMR1 &= ~(3u << 0);
	_TIM3->CCMR1 |= (1u << 0); // Input IC1, mapped to TI1 (Timer channel 1)
	_TIM3->CCMR1 &= ~(3u << 8);
	_TIM3->CCMR1 |= (1u << 8); // Input IC2, mapped to TI2 (Timer channel 1)

	_TIM3->CCMR1 &= ~(3u << 2); // Capture on each edge
	_TIM3->CCMR1 &= ~(15u << 4);
	_TIM3->CCMR1 |= (3u << 4); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	_TIM3->CCMR1 &= ~(3u << 10); // Capture on each edge
	_TIM3->CCMR1 &= ~(15u << 12);
	_TIM3->CCMR1 |= (3u << 12); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	_TIM3->SMCR &= ~(7u << 0);
	_TIM3->SMCR |= (3u << 0); // Encoder Mode 3, both rising and falling edges trigger counts depending on the others level

	_TIM3->EGR = (1u << 0);
	_TIM3->SR = ~(1u << 0);

	_TIM3->CR1 |= (1u << 0); // CEN

	 // PA6 Channel 1, AF2
	_GPIOA->MODER &= ~(3u << 12);
	_GPIOA->MODER |= (2u << 12);
	_GPIOA->AFRL &= ~(15u << 24);
	_GPIOA->AFRL |= (2u << 24);

	// PA7 Channel 2, AF2
	_GPIOA->MODER &= ~(3u << 14);
	_GPIOA->MODER |= (2u << 14);
	_GPIOA->AFRL &= ~(15u << 28);
	_GPIOA->AFRL |= (2u << 28);
};

// PID may need floating point precision for percentage
// A feedback return value may be required
void set_duty(volatile uint32_t* arr_addr, volatile uint32_t* duty_addr, uint8_t percentage) {
	if(percentage > 100) return;
	if(percentage == 0){
		*duty_addr = 0;
		return;
	}
	uint32_t new_duty = ((*arr_addr + 1) * percentage)/100;
	*duty_addr = new_duty - 1;
}

#define WHEEL_LEFT 		0u
#define WHEEL_RIGHT 	1u
#define DIR_BACKWARD 	0u
#define DIR_FORWARD		1u

void set_wheel_direction(uint8_t wheel, uint8_t direction) {
	if(wheel == WHEEL_LEFT) {
		if(direction == DIR_BACKWARD) {
			_GPIOB->BSRR |= (1u << 4);
			_GPIOB->BSRR |= (1u << 21);
		}
		else if(direction == DIR_FORWARD) {
			_GPIOB->BSRR |= (1u << 20);
			_GPIOB->BSRR |= (1u << 5);
		}
	}

	else if(wheel == WHEEL_RIGHT) {
		if(direction == DIR_BACKWARD) {
			_GPIOB->BSRR |= (1u << 29);
			_GPIOB->BSRR |= (1u << 14);
		}
		else if(direction == DIR_FORWARD) {
			_GPIOB->BSRR |= (1u << 13);
			_GPIOB->BSRR |= (1u << 30);
		}
	}
}

void drive_forwards(void) {
	_GPIOB->BSRR |= (1u << 8); // STBY

	set_wheel_direction(WHEEL_LEFT, DIR_FORWARD);
	set_wheel_direction(WHEEL_RIGHT, DIR_FORWARD);

	set_duty(&_TIM2->ARR, &_TIM2->CCR2, 30);
	set_duty(&_TIM2->ARR, &_TIM2->CCR3, 30);
}

void drive_backwards(void) {
	_GPIOB->BSRR |= (1u << 8); // STBY

	set_wheel_direction(WHEEL_LEFT, DIR_BACKWARD);
	set_wheel_direction(WHEEL_RIGHT, DIR_BACKWARD);

	set_duty(&_TIM2->ARR, &_TIM2->CCR2, 30);
	set_duty(&_TIM2->ARR, &_TIM2->CCR3, 30);
}

void drive_left(void) {
	_GPIOB->BSRR |= (1u << 8); // STBY

	set_wheel_direction(WHEEL_LEFT, DIR_FORWARD);
	set_wheel_direction(WHEEL_RIGHT, DIR_FORWARD);

	set_duty(&_TIM2->ARR, &_TIM2->CCR2, 0);
	set_duty(&_TIM2->ARR, &_TIM2->CCR3, 30);
}

void drive_right(void) {
	_GPIOB->BSRR |= (1u << 8); // STBY

	set_wheel_direction(WHEEL_LEFT, DIR_FORWARD);
	set_wheel_direction(WHEEL_RIGHT, DIR_FORWARD);

	set_duty(&_TIM2->ARR, &_TIM2->CCR2, 30);
	set_duty(&_TIM2->ARR, &_TIM2->CCR3, 0);
}

void drive_stop(void) {
	set_duty(&_TIM2->ARR, &_TIM2->CCR2, 0);
	set_duty(&_TIM2->ARR, &_TIM2->CCR3, 0);
}

void drive_disconnect(void) {
	_GPIOB->BSRR |= (1u << 24); // STBY
}

float get_speed(uint16_t* prev, uint32_t period_ms) {
	uint16_t curr = _TIM3->CNT;
	int16_t diff = curr - *prev;
	float speed = (((WHEEL_DIAMETER * PI)/3840) * diff)/(period_ms / 1000.0);
	*prev = curr;
	return speed;
}

#define WASD_FORWARD 		119
#define WASD_LEFT 			97
#define WASD_RIGHT			100
#define WASD_BACKWARD		115
#define SPACEBAR_STOP		32
#define Q_DISCONNECT		113

void wasd_control(char input) {
	if(input == WASD_FORWARD){
		drive_forwards();
	}
	else if(input == WASD_BACKWARD){
		drive_backwards();
	}
	else if(input == WASD_LEFT){
		drive_left();
	}
	else if(input == WASD_RIGHT){
		drive_right();
	}
	else if(input == SPACEBAR_STOP){
		drive_stop();
	}
	else if(input == Q_DISCONNECT){
		drive_disconnect();
	}
}
