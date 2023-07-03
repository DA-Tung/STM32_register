#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "timer_pwm.h"

// INIT TIMER PWM_______________________________________________________________
void timer_pwm_init(TIM_TypeDef* TIMx)
{
	// Enable timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN |RCC_APB1ENR_TIM4EN |RCC_APB1ENR_TIM5EN;

	// Enable GPIO
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
	
	// GPIO mode
	GPIOD->MODER |= GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1;
		
	// AF2 Alternate function for Timer
	GPIOD->AFR[1] |= GPIO_AFRH_AFSEL12_1 | GPIO_AFRH_AFSEL13_1 | GPIO_AFRH_AFSEL14_1 | GPIO_AFRH_AFSEL15_1;
		
	// Enable PWM mode
	TIMx->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
	
	// PWM mode 1
  TIMx->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;	
	TIMx->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;	
  TIMx->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;	
	TIMx->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;

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
	
// INIT TIMER PWM_______________________________________________________________
void timer_pwm_channel(TIM_TypeDef* TIMx, char channel_timer, uint32_t duty_cycle)
{
	// PWM channel 1
	if(channel_timer == timer_channel_1)
	{
		// Enable auto reload preload register
		TIMx->CCMR1 |= TIM_CCMR1_OC1PE;		
		
		// Duty cycle channel 1
		TIMx->CCR1 = duty_cycle; 				
	}
	
	// PWM channel 2
	else if(channel_timer == timer_channel_2)
	{
		// Enable auto reload preload register
		TIMx->CCMR1 |= TIM_CCMR1_OC2PE;	

		// Duty cycle channel 2
		TIMx->CCR2 = duty_cycle;					
	}
	
	// PWM channel 3
	else if(channel_timer == timer_channel_3)
	{
		// Enable auto reload preload register
		TIMx->CCMR2 |= TIM_CCMR2_OC3PE;	
		
		// Duty cycle channel 3
		TIMx->CCR3 = duty_cycle;							
	}
	
	// PWM channel 4
	else if(channel_timer == timer_channel_4)
	{
		// Enable auto reload preload register
		TIMx->CCMR2 |= TIM_CCMR2_OC4PE;		
		
		// Duty cycle channel 1
		TIMx->CCR4 = duty_cycle; 				
	}	
}





