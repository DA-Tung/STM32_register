// System***********************************************************
#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdint.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "iwdg.h"

// Declare variable***********************************************************
IWDG_config 		iwdg_para;

int main()
{
	SysClock_configure();
	
	iwdg_para.pre_divider = divider_8;
	iwdg_para.reload_reg = 79;
	
	iwdg_init(IWDG,iwdg_para);
	iwdg_reload_counter(IWDG);
	
	gpio_output(GPIOB,PORTB,PIN12,PIN_SET);
	
	while(1)
	{
		delay_systick_ms(1800);
		iwdg_reload_counter(IWDG);
	}
}





