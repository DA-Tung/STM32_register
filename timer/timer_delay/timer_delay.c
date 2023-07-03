#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "timer_delay.h"

// TIMER BASE INIT_______________________________________________________________
void timer_base_init(TIM_TypeDef* TIMx)
{
	// Enable timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN |RCC_APB1ENR_TIM4EN |RCC_APB1ENR_TIM5EN;
	
	// Setup parameter Auto Reload Register
	TIMx->ARR = 1000;
	
	// Setup parameter Prescaler Register
	TIMx->PSC = 83;
	
	// Counting mode
	TIMx->CR1 &= ~TIM_CR1_DIR;
	
	// Counter enable
	TIMx->CR1 |= TIM_CR1_CEN;
	
	// Enable auto reload
	TIMx->CR1 |= TIM_CR1_ARPE;
	
	// Update event generation
	TIMx->EGR |= TIM_EGR_UG;
}

// FUNCION DELAY TIMER_______________________________________________________________
void delay_timer(TIM_TypeDef* TIMx, uint32_t time)
{
	while(time)
	{
		// Setup value timer counter 
		TIMx->CNT = 0;
		
		// Waiting CNT count 1000
		while(TIMx->CNT < 1000);
		//   
		time--;
	}
}




