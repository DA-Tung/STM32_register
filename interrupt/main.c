
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "interrupt.h"

// Declare variable***********************************************************
int count_irq = 0;
irq_config para_irq_handler;

// Function***********************************************************
void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR1)
	{
		EXTI->PR |= EXTI_PR_PR1;
		count_irq++;		
	}
}

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	para_irq_handler.exti_port = SYSCFG_EXTICR1_EXTI0_PA;
	para_irq_handler.trigger_level = rising_trigger;
	para_irq_handler.irq_type = EXTI0_IRQn;
	para_irq_handler.priority_level_irq = 0;
	interrupt_config(GPIOC, PIN14, para_irq_handler);	
//	interrupt_config(GPIOA, PIN0, SYSCFG_EXTICR1_EXTI0_PA ,rising_trigger);
	
	while(1)
	{

	}
}

