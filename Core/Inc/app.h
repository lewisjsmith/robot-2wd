/*
 * app.h
 *
 *  Created on: Oct 15, 2026
 *      Author: lewis
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdint.h>
#include "stm32f446xx.h"

#define RCC_addr 	0x40023800
#define GPIOA_addr 	0x40020000
#define GPIOB_addr 	0x40020400
#define USART2_addr	0x40004400
#define TIM2_addr	0x40000000

#define _RCC 	((volatile _RCC_struct*) RCC_addr)
#define _GPIOA 	((volatile _GPIO_struct*) GPIOA_addr)
#define _GPIOB 	((volatile _GPIO_struct*) GPIOB_addr)
#define _USART2 ((volatile _USART_struct*) USART2_addr)
#define _TIM2 	((volatile _TIM_struct*) TIM2_addr)

#define _RCC_AHB1ENR_GPIOAEN (1u << 0)
#define _RCC_AHB1ENR_GPIOBEN (1u << 1)
#define _RCC_APB1ENR_TIM2EN (1u << 0)
#define _RCC_APB1ENR_USART2EN (1u << 17)

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1;
	volatile uint32_t AHB2;
	volatile uint32_t AHB3;
	volatile uint32_t reserved0;
	volatile uint32_t APB1;
	volatile uint32_t APB2;
	volatile uint32_t reserved1;
	volatile uint32_t reserved2;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t reserved3;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
} _RCC_struct;

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDER;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
} _GPIO_struct;

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t reserved0;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t reserved1;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;
} _TIM_struct;

typedef struct {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
} _USART_struct;

void init_rcc(void);
void init_pwm_gpiob(void);
void init_pwm_tim2(void);
void init_usart2(void);
void init_systick(void);

void init_mdriver_gpiob(void);
void test_motor_CCW(void);
void test_motor_CW(void);
void test_motor_BRAKE(void);
void test_motor_OFF(void);
void test_motor_FW(void);
void test_motor_BW(void);
void test_motor_LEFT(void);
void test_motor_RIGHT(void);

void USART2_IRQHandler(void);
void usart2_write_char(char);
void usart2_write_string(char*);
void usart2_read_char(void);

void SysTick_Handler(void);
void delay_ms(uint32_t);

#endif /* INC_APP_H_ */
