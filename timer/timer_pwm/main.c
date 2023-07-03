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
#include "timer_pwm.h"

// Declare variable***********************************************************
int t = 1, i = 50;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
  timer_pwm_init(TIM4);
	
	while(1)
	{
		t = t + i;
		timer_pwm_channel(TIM4,timer_channel_1,t);
		timer_pwm_channel(TIM4,timer_channel_2,t);
		timer_pwm_channel(TIM4,timer_channel_3,t);	
		timer_pwm_channel(TIM4,timer_channel_4,t);
		delay_systick_ms(100);
		if( t > 999 || t < 50)
		{
			i = ~i;
		}
	}
}

