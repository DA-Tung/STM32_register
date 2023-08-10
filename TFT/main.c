
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
#include "front.h"

// Declare variable***********************************************************

// Function***********************************************************
void GPIO_ConfigPIN(void)
{
	tft_pin_config();
	
//	gpio_config(GPIOD, PORTD,PIN12,PIN_OUTPUT);
//	gpio_config(GPIOD, PORTD,PIN13,PIN_OUTPUT);
//	gpio_config(GPIOD, PORTD,PIN14,PIN_OUTPUT);
//	gpio_config(GPIOD, PORTD,PIN15,PIN_OUTPUT);
}

// Program main***********************************************************
int main()
{
	SysClock_configure();
	
	GPIO_ConfigPIN();

	tft_init();
	
	while(1)
	{	
		tft_fill_screen(cyan_color);			
		delay_systick_ms(500);	
		tft_fill_screen(green_color);			
		delay_systick_ms(500);	
		tft_write_string(0x0011,0x0011,"Toi la toi",Font_16x26,black_color,mode_v);
		delay_systick_ms(1000);	
		tft_drawing_circle(0x0066,0x0066,50,red_color);
		delay_systick_ms(1000);	
		tft_fill_circle(0x00BB,0x00AA,40,red_color);
		delay_systick_ms(1000);			
		tft_drawing_Line(0x0066,0x00E3,3,50,red_color);
		delay_systick_ms(1000);
	}
}






