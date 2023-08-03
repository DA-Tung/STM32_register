
// System***********************************************************
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "tft.h"

// Declare variable***********************************************************

// Function***********************************************************
void GPIO_ConfigPIN(void)
{
	tft_pin_config();
}

// Program main***********************************************************
int main()
{
	SysClock_configure();
	
	GPIO_ConfigPIN();
	
	tft_init();
	
	while(1)
	{
		tft_fill_screen(0, 0x013E, 0, 0x0055, red_color);	
		delay_systick_ms(2000);
		tft_fill_screen(0, 0x013E, 0, 0x00EE, green_color);			
		delay_systick_ms(2000);		
	}
}






