/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#include "app.h"

#include "gpio.h"
#include "board.h"
#include "systick.h"

void speed_callback(void* context) {
	Motor* motor = (Motor*)context;

	uint32_t now = SYSTICK_MILLISECONDS;

	uint32_t prev = motor->TICKER;
	float dt = (now - prev);
	if(dt == 0.0f) return;

	// Speed step
	uint32_t prev_count = motor->ENCODER.ENCODER_COUNT_PREV;
	uint32_t curr_count = motor->ENCODER.MOTOR_ENCODER_TIMER->CNT;
	float speed = get_speed_from_encoder(prev_count, curr_count, dt);
	motor->ENCODER.SPEED_ACTUAL = speed;
	motor->ENCODER.ENCODER_COUNT_PREV = curr_count;

	motor->TICKER = now;

	if(motor->MODE == BRAKE) return;

	// PID step
	pid_step(&motor->PID, dt/1000.0f, &motor->SPEED_DESIRED, &motor->ENCODER.SPEED_ACTUAL);
	float output = motor->PID.OUTPUT;
	if(output > motor->MOTOR_PWM_TIMER->ARR) output = motor->MOTOR_PWM_TIMER->ARR;
	else if(output < 0.0f) output = 0.0f;
	motor_set_duty(motor, output);
}


// Hardcode into board.c with reference to direction faced? antic vs clock etc
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



// 	No response reaction
//	uint32_t stale_timer = SYSTICK_MILLISECONDS;
//	uint32_t stale_threshold_ms = 500;
//	uint32_t timed_out = false;
//	// main loop...
//	if(!timed_out && SYSTICK_MILLISECONDS - stale_timer >= stale_threshold_ms) {
//		drive_stop();
//		timed_out = true;
//	}
//	// within the pending_cmd scope
//	stale_timer = SYSTICK_MILLISECONDS;
//	timed_out = false;


