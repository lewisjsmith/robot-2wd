/*
 * usart2.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_USART2_H_
#define INC_USART2_H_

#include "my_types.h"

void init_usart2(void);
void USART2_IRQHandler(void);
void usart2_write_char(char);
void usart2_write_string(char*);
void usart2_read_char(void);

#endif /* INC_USART2_H_ */
