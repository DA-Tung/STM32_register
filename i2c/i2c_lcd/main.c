
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "i2c.h"
#include "lcd_i2c.h"

// Declare variable***********************************************************

// Function***********************************************************

// Program main***********************************************************
int main()
{	
	SysClock_configure();
	
	i2c_init(I2C1);

	lcd_init();	
	
	while(1)
	{
		lcd_send_cmd(0x01);
		
		delay_systick_ms(1000);	
		lcd_set_pos(1,2);
		lcd_send_string("stm32f4__");
		
		delay_systick_ms(1000);	
		lcd_set_pos(2,1);
		lcd_send_string("i2c_lcd++");	
		
		delay_systick_ms(1000);	
		lcd_set_pos(3,4);
		lcd_send_string("hello__");

		delay_systick_ms(1000);	
		lcd_set_pos(4,3);
		lcd_send_string("good_bye++");
	}
}

