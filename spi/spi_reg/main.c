
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"

// Declare variable***********************************************************
uint8_t send_data = 5;
uint8_t receive_data = 0;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	spi_init_master(SPI1);
	spi_init_slave(SPI2);	
	
	while(1)
	{
		gpio_output(GPIOA,PORTA,PIN4,PIN_RESET);
		spi_transmit(SPI1,&send_data,1);		
		spi_receive(SPI2,&receive_data,1);		
		gpio_output(GPIOA,PORTA,PIN4,PIN_SET);	
		delay_systick_ms(500);
		send_data +=2;	
	}
}

	

