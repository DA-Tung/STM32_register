#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// channel timer
#define timer_channel_1		1
#define timer_channel_2		2
#define timer_channel_3		3
#define timer_channel_4		4

void timer_base_init(TIM_TypeDef* TIMx);
void delay_timer(TIM_TypeDef* TIMx, uint32_t time);



