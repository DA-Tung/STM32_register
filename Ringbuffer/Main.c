// System***********************************************************
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "usart.h"
#include "Ringbuffer.h"

// Declare variable***********************************************************
uint8_t buffer_data[20];
Ring_Buffer_Data RB_data;
uint8_t data_pop[5];

// Function***********************************************************
void GPIO_ConfigPIN(void)
{	
	// Config Pin
	usart_ConfigPin();	
	
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
	
	usart_init(USART2);
	
	RingBuffer_Init(&RB_data, buffer_data, 6);
	RingBuffer_push(&RB_data, 12);
	RingBuffer_push(&RB_data, 34);
	RingBuffer_push(&RB_data, 56);
	RingBuffer_push(&RB_data, 78);
	RingBuffer_push(&RB_data, 90);	

	RingBuffer_pop(&RB_data, &data_pop[0]);
	RingBuffer_pop(&RB_data, &data_pop[1]);	
	RingBuffer_pop(&RB_data, &data_pop[2]);
	RingBuffer_pop(&RB_data, &data_pop[3]);
	RingBuffer_pop(&RB_data, &data_pop[4]);
	
	while(1)
	{	
		
	}
}




