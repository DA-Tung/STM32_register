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
#include "modbus_rtu.h"

// Declare variable***********************************************************
modbus_para_write mb_data_write;
uint8_t data_mb[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

// Function***********************************************************
void GPIO_ConfigPIN(void)
{	
	// Config Pin
	usart_ConfigPin();	
	
	// Config PIN DE/RE
	gpio_config(GPIOA, PORTA, PIN4, PIN_OUTPUT);
	
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
	
	usart_init(USART1);
	
	mb_data_write.addr_slave = 0x0A;
	mb_data_write.funcion_mb = 0x04;
	mb_data_write.start_addr = 0x0002;
	mb_data_write.num_reg = 0x04;
	mb_data_write.length_data = 0x08;
	for(int i = 0; i < 8; i++)
	{
		mb_data_write.value_data[i] = data_mb[i];	
	}

	while(1)
	{	
		modbus_write_frame(mb_data_write); 
	}
}




