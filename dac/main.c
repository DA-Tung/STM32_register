
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "dac.h"

// Declare variable***********************************************************
uint32_t value_sin_wave[100];
float PI_value = 3.14159;

uint32_t wave_value[4] = {500, 1500, 2500, 3500};
int i = 0;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
  dac_config(DAC,dac_channel_1, trigger_disen, not_trigger_dac);	
  
	while(1)
	{
		dac_set_value(DAC, dac_channel_1, dac_align_12b_R1, wave_value[i]);
		i++;	
		delay_systick_ms(1000);		
		if(i == 4) i = 0;
	}
}

