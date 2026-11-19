/*
 * motor.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "regs.h"
#include "pid.h"

typedef enum {
	DRIVE,
	BRAKE,
	REVERSE
} MotorMode;

typedef struct {
	volatile _TIM_struct* MOTOR_ENCODER_TIMER;
	volatile _GPIO_struct* MOTOR_ENCODER_CHANNEL_1_GPIO;
	uint8_t MOTOR_ENCODER_CHANNEL_1_PIN;
	volatile _GPIO_struct* MOTOR_ENCODER_CHANNEL_2_GPIO;
	uint8_t MOTOR_ENCODER_CHANNEL_2_PIN;
	// ------------------------------------
	volatile uint32_t ENCODER_COUNT_PREV;
	float SPEED_ACTUAL;
} Encoder;

typedef struct {
	volatile _GPIO_struct* MOTOR_OUT_1_GPIO;
	uint8_t MOTOR_OUT_1_GPIO_PIN;
	volatile _GPIO_struct* MOTOR_OUT_2_GPIO;
	uint8_t MOTOR_OUT_2_GPIO_PIN;
	// ------------------------------------
	volatile _TIM_struct* MOTOR_PWM_TIMER;
	volatile _GPIO_struct* MOTOR_PWM_CHANNEL_GPIO;
	uint8_t MOTOR_PWM_CHANNEL;
	uint8_t MOTOR_PWM_CHANNEL_PIN;
	// ------------------------------------
	Encoder ENCODER;
	// ------------------------------------
	Pid PID;
	float SPEED_DESIRED;
	// ------------------------------------
	MotorMode MODE;
} Motor;



// Temporary
typedef struct {
	volatile uint32_t* 	WHEEL_ARR_ADDR;
	volatile uint32_t* 	WHEEL_DUTY_ADDR;

	float* 				SPEED_DESIRED;
	float*				SPEED_ACTUAL;

	uint32_t 			SAMPLE_PERIOD_MS;
	volatile uint32_t	PREV_SAMPLE_TICK_MS;

	Pid* 				PID;
} MotorPidState;

typedef struct {
	Motor				MOTOR_LEFT;
	Motor				MOTOR_RIGHT;

	uint32_t			SPEED_SAMPLE_PERIOD_MS;
	uint32_t			SPEED_SAMPLE_PRINT_PERIOD_MS;
} State;

extern State state;



void init_motor_driver_gpio(Motor* motor);
void init_encoder_gpio_timer(Encoder* encoder);
void init_motor_pwm_gpio(Motor* motor);

//void init_mdriver_gpiob(void);
void drive_forwards(void);
void drive_backwards(void);
void drive_left(void);
void drive_right(void);
void drive_stop(void);
void drive_disconnect(void);
void set_duty(volatile uint32_t* arr_addr, volatile uint32_t* duty_addr, float percentage);
void set_wheel_direction(uint8_t wheel, uint8_t direction);
//void init_encoder_gpio(void);

typedef struct {
	volatile uint32_t* 	PREV_COUNT;
	volatile uint32_t* 	CURR_COUNT;
	volatile float* 	SPEED_ACTUAL;
	// --------------------------
	float* 				SPEED_DESIRED;
	volatile uint32_t* 	WHEEL_ARR_ADDR;
	volatile uint32_t* 	WHEEL_DUTY_ADDR;
	Pid* 				PID;
	MotorMode*		 	MODE;
	// --------------------------
	volatile uint32_t	PREV_SAMPLE_TICK_MS;
} SpeedContext;

float get_speed_from_encoder(volatile uint32_t, volatile uint32_t, uint32_t);
void get_speed_from_encoder_callback(void* context);

void wasd_control(char);

typedef struct {
	volatile float* WHEEL_LEFT_SPEED;
	volatile float* WHEEL_RIGHT_SPEED;
} PrintContext;

void usart2_print_speed(volatile float);
void usart2_print_speed_callback(void* context);

void init_encoder_gpio(Encoder* encoder);

void init_share_state(State*);

#endif /* INC_MOTOR_H_ */
