
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
void EXTI1_IRQHandler(void)
{
	if((EXTI->PR & EXTI_PR_PR1) != 0)
	{
		// Clear bit
		EXTI->PR |= EXTI_PR_PR1;
		
		if(gpio_input(GPIOA, PIN1) == 1)
		{
			count_irq++;			
			gpio_output(GPIOD, PIN13, PIN_TOGGLE);
		}
	}
}

void GPIO_ConfigPIN(void)
{	
	// Config Pin
//	gpio_config(GPIOA, PINA
	
	// Config PIN LED
	gpio_config(GPIOD, PORTD,PIN12,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN13,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN14,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN15,PIN_OUTPUT);
}

// Program main***********************************************************
int main()
{	
	SysClock_configure();
	
	GPIO_ConfigPIN();
	
	para_irq_handler.exti_port = SYSCFG_EXTICR1_EXTI1_PA;
	para_irq_handler.trigger_level = rising_trigger;
	para_irq_handler.irq_type = EXTI1_IRQn;
	para_irq_handler.priority_level_irq = 1;
	interrupt_config(GPIOA, PIN1, para_irq_handler);	
	
	while(1)
	{
		gpio_output(GPIOD, PIN12, PIN_TOGGLE);
		delay_systick_ms(1000);
	}
}



