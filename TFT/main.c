
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
	
}

// Program main***********************************************************
int main()
{
	SysClock_configure();
	GPIO_ConfigPIN();
	tft_init();
	
	tft_write_pixel(0x0055,0x0066,green_color);
	delay_systick_ms(5000);
	tft_fill_screen(0, 0x00EF, 0, 0x013F, blue_color);	
	
	while(1)
	{
		
	}
}






