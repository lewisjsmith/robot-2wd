/*
 * motor.c
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#include <stdio.h>
#include <stdint.h>

#include "motor.h"
#include "usart2.h"
#include "constants.h"

#include "gpio.h"

void init_motor_pwm_gpio(Motor* motor) {
	gpio_set_mode(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, AF);
	gpio_set_af(motor->MOTOR_PWM_CHANNEL_GPIO, motor->MOTOR_PWM_CHANNEL_PIN, 1);
}

void init_motor_driver_gpio(Motor* motor) {
	gpio_set_mode(motor->MOTOR_OUT_1_GPIO, motor->MOTOR_OUT_1_GPIO_PIN, OUTPUT);
	gpio_set_mode(motor->MOTOR_OUT_2_GPIO, motor->MOTOR_OUT_2_GPIO_PIN, OUTPUT);
	gpio_set_mode(_GPIOB, 8, OUTPUT); //PB8 STBY
}

void init_encoder_gpio(Encoder* encoder) {
	 // PA6 Channel 1, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, encoder->MOTOR_ENCODER_CHANNEL_1_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, encoder->MOTOR_ENCODER_CHANNEL_1_PIN, 2);

	// PA7 Channel 2, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, encoder->MOTOR_ENCODER_CHANNEL_2_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, encoder->MOTOR_ENCODER_CHANNEL_2_PIN, 2);
}

void init_encoder_gpio_timer(Encoder* encoder){
	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(3u << 0);
	encoder->MOTOR_ENCODER_TIMER->CCMR1 |= (1u << 0); // Input IC1, mapped to TI1 (Timer channel 1)
	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(3u << 8);
	encoder->MOTOR_ENCODER_TIMER->CCMR1 |= (1u << 8); // Input IC2, mapped to TI2 (Timer channel 1)

	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(3u << 2); // Capture on each edge
	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(15u << 4);
	encoder->MOTOR_ENCODER_TIMER->CCMR1 |= (3u << 4); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(3u << 10); // Capture on each edge
	encoder->MOTOR_ENCODER_TIMER->CCMR1 &= ~(15u << 12);
	encoder->MOTOR_ENCODER_TIMER->CCMR1 |= (3u << 12); // Input capture filter, 8 consecutive events to validate a transition, taken at source frequency

	encoder->MOTOR_ENCODER_TIMER->SMCR &= ~(7u << 0);
	encoder->MOTOR_ENCODER_TIMER->SMCR |= (3u << 0); // Encoder Mode 3, both rising and falling edges trigger counts depending on the others level

	encoder->MOTOR_ENCODER_TIMER->EGR = (1u << 0);
	encoder->MOTOR_ENCODER_TIMER->SR = ~(1u << 0);

	encoder->MOTOR_ENCODER_TIMER->CR1 |= (1u << 0); // CEN

	 // PA6 Channel 1, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, encoder->MOTOR_ENCODER_CHANNEL_1_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_1_GPIO, 6, 2);

	// PA7 Channel 2, AF2
	gpio_set_mode(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, encoder->MOTOR_ENCODER_CHANNEL_2_PIN, AF);
	gpio_set_af(encoder->MOTOR_ENCODER_CHANNEL_2_GPIO, 7, 2);
};

void set_duty(volatile uint32_t* arr_addr, volatile uint32_t* duty_addr, float new_duty_ccr) {
	if(new_duty_ccr > *arr_addr) return;
	if(new_duty_ccr == 0){
		*duty_addr = 0;
		return;
	}
	*duty_addr = new_duty_ccr;
}

#define WHEEL_LEFT 		0u
#define WHEEL_RIGHT 	1u


// TODO: abstract out the _GPIOB field
void set_wheel_direction(uint8_t wheel, MotorMode direction) {
	if(wheel == WHEEL_LEFT) {
		if(direction == REVERSE) {
			gpio_write(_GPIOB, 4, 1);
			gpio_write(_GPIOB, 5, 0);
		}
		else if(direction == DRIVE) {
			gpio_write(_GPIOB, 4, 0);
			gpio_write(_GPIOB, 5, 1);
		}
		else if(direction == BRAKE) {
			gpio_write(_GPIOB, 4, 1);
			gpio_write(_GPIOB, 5, 1);
		}
	}

	else if(wheel == WHEEL_RIGHT) {
		if(direction == REVERSE) {
			gpio_write(_GPIOB, 13, 0);
			gpio_write(_GPIOB, 14, 1);
		}
		else if(direction == DRIVE) {
			gpio_write(_GPIOB, 13, 1);
			gpio_write(_GPIOB, 14, 0);
		}
		else if(direction == BRAKE) {
			gpio_write(_GPIOB, 13, 1);
			gpio_write(_GPIOB, 14, 1);
		}
	}
}

void set_wheel_mode(Motor* motor, uint8_t wheel, MotorMode direction) {
	if(motor->MODE != direction) {
		set_wheel_direction(wheel, direction);
		motor->PID.PREV_ERROR = 0.0f;
		motor->MODE = direction;
		if(direction != BRAKE) motor->PID.INTEGRAL = 30.0f;
		else motor->PID.INTEGRAL = 0.0f;
	}
}

void drive_forwards(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, DRIVE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, DRIVE);
}

void drive_backwards(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, REVERSE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, REVERSE);
}

void drive_left(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, BRAKE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, DRIVE);
}

void drive_right(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, DRIVE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, BRAKE);
}

void drive_stop(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, BRAKE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, BRAKE);
}

void drive_disconnect(void) {
	gpio_write(_GPIOB, 8, 0); // STBY
	set_wheel_mode(&state.MOTOR_LEFT, WHEEL_LEFT, BRAKE);
	set_wheel_mode(&state.MOTOR_RIGHT, WHEEL_RIGHT, BRAKE);
}

float get_speed_from_encoder(volatile uint32_t prev_cnt, volatile uint32_t curr_cnt, uint32_t period_ms) {
	uint16_t prev = prev_cnt;
	uint16_t curr = curr_cnt;
	int16_t diff = curr - prev;

	if(diff < 0) diff *= -1;

	float speed = (((WHEEL_DIAMETER * PI)/3840) * diff)/(period_ms / 1000.0f);
	return speed;
}

// Needs pairing with a suitable delay
void usart2_print_speed(volatile float speed) {
	char buffer[100];
	snprintf(
		buffer,
		sizeof(buffer),
		"Speed: %f m/s \r\n",
		speed);
	usart2_write_string(buffer);
}

void usart2_print_speed_callback(void* context) {
    PrintContext* pc = (PrintContext*)context;

	char buffer_l[100];
	snprintf(
		buffer_l,
		sizeof(buffer_l),
		"Left  ");
	usart2_write_string(buffer_l);
    usart2_print_speed(*(pc->WHEEL_LEFT_SPEED));

	char buffer_r[100];
	snprintf(
		buffer_r,
		sizeof(buffer_r),
		"Right ");
	usart2_write_string(buffer_r);
    usart2_print_speed(*(pc->WHEEL_RIGHT_SPEED));
}

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
