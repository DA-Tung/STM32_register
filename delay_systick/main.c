// System***********************************************************
#define MAIN_H__
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"


// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	while(1)
	{
		delay_systick_ms(500);
	}
}


