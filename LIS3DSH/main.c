
// System***********************************************************
#include "string.h"
#include "math.h"
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "Lis3dsh_spi.h"
#include "MY_LIS3DSH.h"

// Declare variable***********************************************************
Value_Axis_G value_axis;
int16_t data_axis[3];

// Function***********************************************************
void GPIO_ConfigPin(void);

// GPIO Config
void GPIO_ConfigPin(void)
{
	spi_master_configPIN();
	
	gpio_config(GPIOE, PORTE, PIN3, PIN_OUTPUT);
	
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
	
	spi_init_master(SPI1);	
	
	Lis3dsh_init(FULL_SCALE_8);
	
	while(1)
	{
		if(Lis3dsh_get_status() == 1)
		{
			value_axis = Lis3dsh_get_value(SENSITIVITY_8G);
			
			if(value_axis.Axis_X > 0 && value_axis.Axis_Y > 0)
			{
				gpio_output(GPIOD,PIN12, PIN_SET);
				gpio_output(GPIOD,PIN13, PIN_SET);
				gpio_output(GPIOD,PIN14, PIN_RESET);
				gpio_output(GPIOD,PIN15, PIN_RESET);
			}
			else if(value_axis.Axis_X > 0 && value_axis.Axis_Y < 0)
			{
				gpio_output(GPIOD,PIN12, PIN_SET);
				gpio_output(GPIOD,PIN13, PIN_RESET);
				gpio_output(GPIOD,PIN14, PIN_RESET);
				gpio_output(GPIOD,PIN15, PIN_SET);
			}		
			else if(value_axis.Axis_X < 0 && value_axis.Axis_Y > 0)
			{
				gpio_output(GPIOD,PIN12, PIN_RESET);
				gpio_output(GPIOD,PIN13, PIN_SET);
				gpio_output(GPIOD,PIN14, PIN_SET);
				gpio_output(GPIOD,PIN15, PIN_RESET);
			}
			else if(value_axis.Axis_X < 0 && value_axis.Axis_Y < 0)
			{
				gpio_output(GPIOD,PIN12, PIN_RESET);
				gpio_output(GPIOD,PIN13, PIN_RESET);
				gpio_output(GPIOD,PIN14, PIN_SET);
				gpio_output(GPIOD,PIN15, PIN_SET);
			}				
		}
	}
}






