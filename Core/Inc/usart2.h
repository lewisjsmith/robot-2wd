/*
 * usart2.h
 *
 *  Created on: Oct 20, 2026
 *      Author: lewis
 */

#ifndef INC_USART2_H_
#define INC_USART2_H_

#include <stdbool.h>

#include "my_types.h"
#include "regs.h"

void init_usart2(uint32_t);
void usart2_write_char(char);
void usart2_write_string(const char*);
bool usart2_try_read(char* out);

#endif /* INC_USART2_H_ */
