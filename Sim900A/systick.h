#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

void SysClock_configure(void);
void delay_systick_us(uint32_t time);
void delay_systick_ms(uint32_t time);

#endif //_SYSTICK_H_


