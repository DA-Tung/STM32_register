// System***********************************************************
#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"

// Declare variable***********************************************************
int toggle = 0;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();

	while(1)
	{
		gpio_output(GPIOD, PORTD, PIN12, PIN_TOGGLE);
		toggle = 1 - toggle;
		delay_systick_ms(1000);	
	}
}




