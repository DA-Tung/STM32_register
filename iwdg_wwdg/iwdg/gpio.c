
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "gpio.h"

// GPIO_OUTPUT
//-----------------------------------------------------------------------------------------------------------------------------------------------
void gpio_output(GPIO_TypeDef* GPIOx, uint16_t PINx, GPIO_PinState state)
{	
		// Set bit
		if(state == PIN_SET) GPIOx->ODR |= (1 << PINx);
	
		// Reset bit
		else if(state == PIN_RESET ) GPIOx->ODR &= ~(1 << PINx);  
	
		// Toggle bit
		else if(state == PIN_TOGGLE) GPIOx->ODR ^= (1 << PINx);	
}

// GPIO_INPUT_______________________________________________________________
bit_state gpio_input(GPIO_TypeDef* GPIOx, uint16_t PINx)
{	
		bit_state state;
		
	  // Check status bit
		if((GPIOx->IDR & (1 << PINx)) == 1) state = BIT_SET;
		else state = BIT_RESET;
	
	return state;	
}

// GPIO_CONFIG_______________________________________________________________
void gpio_config(GPIO_TypeDef* GPIOx, uint32_t PORTx, uint32_t PINx, IO_config PIN_IO)
{
		// Enable GPIO
		RCC->AHB1ENR |= (1 << PORTx);
		
		// No pull-up and pull-down		
		GPIOx->PUPDR |= 0;		
	
		// Output Pin
		if(PIN_IO == PIN_OUTPUT)
		{
			// Select PIN and Mode I/O
			GPIOx->MODER |= (1 << (PINx*2));		
			
			// Configure I/O output type
			GPIOx->OTYPER |= 0;
	
			// Configure output speed
			GPIOx->OSPEEDR |= (uint32_t)(3 << (PINx*2));				
		}
		// Input Pin
		else if(PIN_IO == PIN_INPUT)
		{
			// Select PIN and Mode I/O
			GPIOx->MODER &= ~(1 << (PINx*2));		
				
			// Configure output speed
			GPIOx->OSPEEDR &= ~((uint32_t)(3 << (PINx*2)));				
		}
}




