
// System***********************************************************
#include "string.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "NRF24L01.h"

// Declare variable***********************************************************
#define NRF24L01_SPI			SPI1	

uint8_t TxAddr[] = {0x11,0x22,0x33,0x44,0x55};
uint8_t TxData[] = "Hello\n";

// Function***********************************************************
void GPIO_ConfigPin(void);


void GPIO_ConfigPin(void)
{
	gpio_config(GPIOD, PORTD, PIN12, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN13, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN14, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN15, PIN_OUTPUT);
}


// Program main***********************************************************
int main()
{
	SysClock_configure();
	GPIO_ConfigPin();
	spi_init_master(NRF24L01_SPI);
	NRF24L01_Init();
	
	NRF24L01_Tx_mode(TxAddr, 10);
	
	while(1)
	{	
		// Test transmit data
		if(NRF24L01_Transmit(TxData) == 1)
		{
			
			gpio_output(GPIOD, PIN12, PIN_TOGGLE);
			gpio_output(GPIOD, PIN13, PIN_RESET);
		}
		else 
		{
			gpio_output(GPIOD, PIN13, PIN_SET);
		}
		delay_systick_ms(500);		
		
	}
}

