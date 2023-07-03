// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "timer_input_capture.h"

// Declare variable***********************************************************

uint32_t rise_data = 0, false_data = 0;
uint32_t cycle = 0;
float freq = 0;
int detect_capture = 0;

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	while(1)
	{
		
	}
}




