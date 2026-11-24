/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

// To-do:
// Apply initial motor mode(s) stated in the board.c config, needs to be written
// Replaced hardcoded STBY
// Extract out PID gains, default desired speeds/setpoints, initial integral to robot_params
// Teleop keycodes, list as chars, enum or other
// Consider stale-command timeout, naive implementation at the bottom of app.c
// Remove rcc (.c/.h), move clock constant
//

#include <stdio.h>

#include "app.h"

#include "gpio.h"
#include "board.h"
#include "usart2.h"
#include "systick.h"
#include "teleop.h"
#include "motor.h"
#include "pid.h"
#include "encoder.h"

typedef struct {
	Motor* 			motor;
	Encoder*		encoder;
	Pid 			pid;
	float 			SPEED_DESIRED;
	uint32_t		ticker;
} Wheel;

typedef void (* callback_fn)(void* context);

static const Pid INITIAL_PID_DEFAULT = {
	.INTEGRAL 	= 0.0f,
	.PREV_ERROR = 0.0f,
	.Kp 		= 1500.0f,
	.Ki 		= 1000.0f,
	.Kd 		= 0.0f,
	.OUT_MIN	= 0,
	.OUT_MAX	= SHARED_PWM_ARR,
};

static Wheel wheel_left;
static Wheel wheel_right;

void app_init(void) {

	wheel_left = (Wheel){
			.motor = &state.MOTOR_LEFT,
			.encoder = &state.ENCODER_LEFT,
			.pid = INITIAL_PID_DEFAULT,
			.SPEED_DESIRED = 0.157080f, // Move
			.ticker = 0,
	};

	wheel_right = (Wheel){
			.motor = &state.MOTOR_RIGHT,
			.encoder = &state.ENCODER_RIGHT,
			.pid = INITIAL_PID_DEFAULT,
			.SPEED_DESIRED = 0.157080f,
			.ticker = 0,
	};

};

static uint32_t speed_print_ticker = 0;

static void scheduler(
		uint32_t* sample_ticker,
		uint32_t sample_period,
		callback_fn fn,
		void* context) {
	uint32_t now = systick_ms();
	if(now - *sample_ticker >= sample_period) {
		fn(context);
		*sample_ticker = now;
	}
}

static void print_telemetry(void* unused) {
    (void)unused;
    char buf[64];
    snprintf(buf, sizeof(buf), "L %.3f  R %.3f m/s\r\n",
             wheel_left.encoder->SPEED_ACTUAL,
             wheel_right.encoder->SPEED_ACTUAL);
    usart2_write_string(buf);
}

static void speed_callback(void* context) {
	Wheel* wheel = (Wheel*)context;

	uint32_t now = systick_ms();

	uint32_t prev = wheel->ticker;
	float dt = (now - prev);
	if(dt == 0.0f) return;

	// Speed step
	uint32_t prev_count = wheel->encoder->ENCODER_COUNT_PREV;
	uint32_t curr_count = wheel->encoder->MOTOR_ENCODER_TIMER->CNT;
	float speed = get_speed_from_encoder(prev_count, curr_count, dt);
	wheel->encoder->SPEED_ACTUAL = speed;
	wheel->encoder->ENCODER_COUNT_PREV = curr_count;

	wheel->ticker = now;

	if(wheel->motor->MODE == BRAKE) return;

	// PID step
	pid_step(&wheel->pid, dt/1000.0f, &wheel->SPEED_DESIRED, &wheel->encoder->SPEED_ACTUAL);
	float output = wheel->pid.OUTPUT;
	motor_set_duty(wheel->motor, output);
}

void app_step(void) {

	char next_cmd = 0;
	if(usart2_try_read(&next_cmd)) {
		wasd_control(next_cmd);
	}

	scheduler(
				&wheel_left.ticker,
				state.SPEED_SAMPLE_PERIOD_MS,
				speed_callback,
				&wheel_left
	);

	scheduler(
				&wheel_right.ticker,
				state.SPEED_SAMPLE_PERIOD_MS,
				speed_callback,
				&wheel_right
	);

	scheduler(
				&speed_print_ticker,
				state.SPEED_SAMPLE_PRINT_PERIOD_MS,
				print_telemetry,
				NULL
	);
};

static void set_wheel_mode(Wheel* wheel, MotorMode direction) {
	if(wheel->motor->MODE != direction) {
		set_wheel_direction(wheel->motor, direction);
		wheel->pid.PREV_ERROR = 0.0f;
		wheel->motor->MODE = direction;
		if(direction != BRAKE) wheel->pid.INTEGRAL = 30.0f;
		else wheel->pid.INTEGRAL = 0.0f;
	}
}

void drive_forwards(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&wheel_left, DRIVE);
	set_wheel_mode(&wheel_right, DRIVE);
}

void drive_backwards(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&wheel_left, REVERSE);
	set_wheel_mode(&wheel_right, REVERSE);
}

void drive_left(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&wheel_left, BRAKE);
	set_wheel_mode(&wheel_right, DRIVE);
}

void drive_right(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&wheel_left, DRIVE);
	set_wheel_mode(&wheel_right, BRAKE);
}

void drive_stop(void) {
	gpio_write(_GPIOB, 8, 1); // STBY
	set_wheel_mode(&wheel_left, BRAKE);
	set_wheel_mode(&wheel_right, BRAKE);
}

void drive_disconnect(void) {
	gpio_write(_GPIOB, 8, 0); // STBY
	set_wheel_mode(&wheel_left, BRAKE);
	set_wheel_mode(&wheel_right, BRAKE);
}



// 	No response reaction
//	uint32_t stale_timer = systick_ms();
//	uint32_t stale_threshold_ms = 500;
//	uint32_t timed_out = false;
//	// main loop...
//	if(!timed_out && systick_ms() - stale_timer >= stale_threshold_ms) {
//		drive_stop();
//		timed_out = true;
//	}
//	// within the pending_cmd scope
//	stale_timer = systick_ms();
//	timed_out = false;


