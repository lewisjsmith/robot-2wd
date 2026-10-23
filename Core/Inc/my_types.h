/*
 * my_structs.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_MY_TYPES_H_
#define INC_MY_TYPES_H_

#define RCC_addr 	0x40023800
#define GPIOA_addr 	0x40020000
#define GPIOB_addr 	0x40020400
#define USART2_addr	0x40004400
#define TIM2_addr	0x40000000
#define TIM3_addr 	0x40000400

#define _RCC 	((volatile _RCC_struct*) RCC_addr)
#define _GPIOA 	((volatile _GPIO_struct*) GPIOA_addr)
#define _GPIOB 	((volatile _GPIO_struct*) GPIOB_addr)
#define _USART2 ((volatile _USART_struct*) USART2_addr)
#define _TIM2 	((volatile _TIM_struct*) TIM2_addr)
#define _TIM3 	((volatile _TIM_struct*) TIM3_addr)

#define _RCC_AHB1ENR_GPIOAEN (1u << 0)
#define _RCC_AHB1ENR_GPIOBEN (1u << 1)

#define _RCC_APB1ENR_TIM2EN (1u << 0)
#define _RCC_APB1ENR_TIM3EN (1u << 1)

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

typedef struct {
	volatile uint32_t* 	WHEEL_LEFT_ENCODER_COUNT_ADDR;
	volatile uint32_t 	WHEEL_LEFT_ENCODER_COUNT_PREV;
	uint32_t			SPEED_SAMPLE_PERIOD_MS;
	uint32_t			SPEED_SAMPLE_PRINT_PERIOD_MS;
	// Later untangle into speed per wheel and body
	volatile float		SPEED;

	// PID
	float 				SPEED_DESIRED;
	volatile uint32_t	PID_PREV_MS;
	uint32_t 			PID_SAMPLE_PERIOD_MS;
	volatile float 		PID_INTEGRAL;
	float 				PID_PREV_ERROR;
	float 				Kp;
	float 				Kd;
	float				Ki;
} State;


#endif /* INC_MY_TYPES_H_ */
