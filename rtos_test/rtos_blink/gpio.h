#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

// PORT
#define PORTA 		0
#define PORTB 		1
#define PORTC		2
#define PORTD 		3
#define PORTE 		4
#define PORTH 		7

// PIN
#define PIN0 			0
#define PIN1 			1
#define PIN2 			2
#define PIN3	  		3
#define PIN4 			4
#define PIN5 			5
#define PIN6	 		6
#define PIN7	 		7
#define PIN8 			8
#define PIN9	 		9
#define PIN10 			10
#define PIN11 			11
#define PIN12 			12
#define PIN13 			13
#define PIN14 			14
#define PIN15 			15


// STATE
typedef enum
{
  PIN_RESET = 0,
  PIN_SET = 1,
	PIN_TOGGLE	
}GPIO_PinState;

// Bit status
typedef enum
{
	BIT_RESET = 0,
	BIT_SET = 1
}Bit_Status;

// I/O config
typedef enum
{
	PIN_INPUT = 0,
	PIN_OUTPUT = 1
}IO_config;

// Funcion
void gpio_config(GPIO_TypeDef* GPIOx, uint32_t PORTx, uint32_t PINx, IO_config PIN_IO);
void gpio_output(GPIO_TypeDef* GPIOx, uint16_t PINx, GPIO_PinState state);
Bit_Status gpio_input(GPIO_TypeDef* GPIOx, uint16_t PINx);

#endif

