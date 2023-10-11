
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "gpio.h"
#include "interrupt.h"

// Config interrupt_______________________________________________________________
void interrupt_config(GPIO_TypeDef* GPIOx, uint16_t PINx, irq_config para_irq)	
{
	// Enable GPIO
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
	
	// GPIO no pull up/pull down
	GPIOx->PUPDR = (1 << PINx);
		
	// Enable SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Configure interrupt
	int num_exti = (int)PINx;
	
	if(num_exti <= 3)
	{
		SYSCFG->EXTICR[0] = para_irq.exti_port;		
	}
	
	else if((3 < num_exti) & (num_exti <= 7))
	{
		SYSCFG->EXTICR[1] = para_irq.exti_port;	
	}

	else if((7 < num_exti) & (num_exti <= 11))
	{
		SYSCFG->EXTICR[2] = para_irq.exti_port;	
	}
	
	else if(11 < num_exti)
	{
		SYSCFG->EXTICR[3] = para_irq.exti_port;	
	}	
	
	// Configure mask bit
	EXTI->IMR |= (1 << PINx);
	
	// Config event line mask
	EXTI->EMR &= (1 << PINx);
	
	// Configure trigger
	if(para_irq.trigger_level == rising_trigger)          EXTI->RTSR |= (1 << PINx);
	else if(para_irq.trigger_level == falling_trigger)    EXTI->FTSR |= (1 << PINx);
	
	// Set priority
	NVIC_SetPriority(para_irq.irq_type, para_irq.priority_level_irq);
	
	// Enable IRQ
	NVIC_EnableIRQ(para_irq.irq_type);	
}


