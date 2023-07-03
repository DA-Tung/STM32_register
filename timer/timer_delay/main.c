
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "timer_delay.h"

// Declare variable***********************************************************
int count = 0;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
  timer_base_init(TIM2);
	
	while(1)
	{
		gpio_output(GPIOD, PORTD, PIN13, PIN_TOGGLE);
		count = 1 - count;
		delay_timer(TIM2, 1000);
	}
}

