/*
 * app.c
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#include <stdio.h>

#include "app.h"

#include "gpio.h"
#include "board.h"
#include "usart2.h"
#include "systick.h"
#include "teleop.h"

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
			.ticker = SYSTICK_MILLISECONDS,
	};

	wheel_right = (Wheel){
			.motor = &state.MOTOR_RIGHT,
			.encoder = &state.ENCODER_RIGHT,
			.pid = INITIAL_PID_DEFAULT,
			.SPEED_DESIRED = 0.157080f,
			.ticker = SYSTICK_MILLISECONDS,
	};

};

static uint32_t speed_print_ticker = 0;

static void scheduler(
		uint32_t* sample_ticker,
		uint32_t sample_period,
		callback_fn fn,
		void* context) {
	if(SYSTICK_MILLISECONDS - *sample_ticker >= sample_period) {
		fn(context);
		*sample_ticker = SYSTICK_MILLISECONDS;
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

void speed_callback(void* context) {
	Wheel* wheel = (Wheel*)context;

	uint32_t now = SYSTICK_MILLISECONDS;

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

void set_wheel_mode(Wheel* const wheel, const MotorMode direction) {
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


