/* System */
/*-----------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "main.h"

/* Library*/
/*-----------------------------------------------------------------------------------*/
#include "gpio.h"
#include "systick.h"
#include "spi_soft_master.h"

/* Declare */
/*-----------------------------------------------------------------------------------*/
uint8_t transmit_data[4] = {0x0C,0x0D,0x0E,0x0F};
uint8_t receive_data[4];

/* Funcion */
/*-----------------------------------------------------------------------------------*/


/* Main Program */
/*-----------------------------------------------------------------------------------*/
int main()
{
	// Init system clock
	SysClock_configure();
	// Inint Master SPI
	SPI_Master_Init();
	// Config Button
	gpio_config(GPIOA,PORTA,PIN6,PIN_INPUT);
	gpio_config(GPIOD,PORTD,PIN12,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN13,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN14,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN15,PIN_OUTPUT);
	gpio_output(GPIOD,PIN12,PIN_RESET);
	gpio_output(GPIOD,PIN13,PIN_RESET);
	gpio_output(GPIOD,PIN14,PIN_RESET);
	gpio_output(GPIOD,PIN15,PIN_RESET);	
	while(1)
	{	
		SS_ENABLE;												// Reset Pin SS to start transmit data		
		for(int i = 0; i < 4; i++)
		{	
			receive_data[i] = SPI_Master_ReadWrite(transmit_data[i]);		
			gpio_output(GPIOD,receive_data[i],PIN_TOGGLE);			
		}
		SS_DISENABLE;											// Set Pin SS to stop transmit data			
		delay_systick_ms(100);
	}
}





