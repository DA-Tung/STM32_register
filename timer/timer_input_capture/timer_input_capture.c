#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "timer_input_capture.h"

// INIT INPUT CAPTURE MODE_______________________________________________________________
void timer_capture_init(TIM_TypeDef* TIMx, char channel_timer)
{
	// Enable timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN |RCC_APB1ENR_TIM4EN |RCC_APB1ENR_TIM5EN;

	// Enable GPIO
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
	
	// GPIO mode
	GPIOA->MODER |= GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1 | GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
		
	// AF2 Alternate function for Timer
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_0 | GPIO_AFRL_AFSEL1_0 | GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL3_0;
	
	// Setup parameter Auto Reload Register
	TIMx->ARR = 1000;
	
	// Setup parameter Prescaler Register
	TIMx->PSC = 83;	
	
	if(channel_timer == timer_channel_1)
	{
		// Select active input
		TIMx->CCMR1 |= TIM_CCMR1_CC1S_0;
		
		// Select the edge
		TIMx->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
	
		// Capture is done each time
		TIMx->CCMR1 &= ~TIM_CCMR1_IC1PSC;	

		// Enable Capture
		TIMx->CCER |= TIM_CCER_CC1E;		
	}

	else if(channel_timer == timer_channel_2)
	{
		// Select active input
		TIMx->CCMR1 |= TIM_CCMR1_CC2S_0;
		
		// Select the edge
		TIMx->CCER |= TIM_CCER_CC2P | TIM_CCER_CC2NP;
		
		// Capture is done each time
		TIMx->CCMR1 &= ~TIM_CCMR1_IC2PSC;	

		// Enable Capture
		TIMx->CCER |= TIM_CCER_CC2E;		
	}		
	
	else if(channel_timer == timer_channel_3)
	{
		// Select active input
		TIMx->CCMR2 |= TIM_CCMR2_CC3S_0;
		
		// Select the edge
		TIMx->CCER |= TIM_CCER_CC3P | TIM_CCER_CC3NP;
		
		// Capture is done each time
		TIMx->CCMR2 &= ~TIM_CCMR2_IC3PSC;	

		// Enable Capture
		TIMx->CCER |= TIM_CCER_CC3E;		
	}

	else if(channel_timer == timer_channel_4)
	{
		// Select active input
		TIMx->CCMR2 |= TIM_CCMR2_CC4S_0;
		
		// Select the edge
		TIMx->CCER |= TIM_CCER_CC4P | TIM_CCER_CC4NP;
		
		// Capture is done each time
		TIMx->CCMR2 &= ~TIM_CCMR2_IC4PSC;	

		// Enable Capture
		TIMx->CCER |= TIM_CCER_CC4E;		
	}			
	
	// Counting mode
	TIMx->CR1 &= ~TIM_CR1_DIR;
	
	// Counter enable
	TIMx->CR1 |= TIM_CR1_CEN;		
	
//	TIMx->DIER |= (1 << channel_timer);	
}

//Get value timer capture_______________________________________________________________
uint32_t timer_capture_getvalue(TIM_TypeDef* TIMx, char channel_timer)
{
	uint32_t timer_value = 0;
	
		// Capture channel 1
		if(channel_timer == timer_channel_1)
		{
			// Wait CC1IF set
			while(!(TIMx->SR & TIM_SR_CC1IF));
			
			// get value			
			timer_value = TIMx->CCR1;		
		}	
		
		// Capture channel 2
		if(channel_timer == timer_channel_2)
		{				
			// Wait CC1IF set
  		while(!(TIMx->SR & TIM_SR_CC2IF));
				
			// get value
			timer_value = TIMx->CCR2;
		}	
		
		// Capture channel 3
		if(channel_timer == timer_channel_3)
		{
			// Wait CC1IF set
  		while(!(TIMx->SR & TIM_SR_CC3IF));
				
			// get value			
			timer_value = TIMx->CCR3;
		}	
		
		// Capture channel 4
		if(channel_timer == timer_channel_4)
		{
			// Wait CC1IF set
  		while(!(TIMx->SR & TIM_SR_CC4IF));
				
			// get value			
			timer_value = TIMx->CCR4;
		}	

	return timer_value;
}






