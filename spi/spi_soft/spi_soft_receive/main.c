/* System */
/*-----------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "main.h"

/* Library*/
/*-----------------------------------------------------------------------------------*/
#include "gpio.h"
#include "systick.h"
#include "spi_soft.h"

/* Declare */
/*-----------------------------------------------------------------------------------*/
uint8_t receive_data[4];
uint8_t transmit_data[4] = {0x5C,0x5D,0x5E,0x5F};	
uint8_t i = 0;
/* Funcion */
/*-----------------------------------------------------------------------------------*/

/* Main Program */
/*-----------------------------------------------------------------------------------*/
int main()
{
	// Init system clock
	SysClock_configure();
	// Inint Slave SPI
	SPI_Slave_Init();
	// Config Button
	//BUTTON_CONFIG;
	gpio_config(GPIOD,PORTD,PIN12,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN13,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN14,PIN_OUTPUT);
	gpio_config(GPIOD,PORTD,PIN15,PIN_OUTPUT);
	while(1)
	{
		receive_data[i] = SPI_Slave_ReadWrite(transmit_data[i]);
		gpio_output(GPIOD,receive_data[i],PIN_TOGGLE);
		i++;
		if(i == 4) i = 0;
	}
}






