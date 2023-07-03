
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "adc.h"

// Declare variable***********************************************************
uint16_t adc_val[2];

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	adc_init(ADC1,2);
	adc_channel_config(ADC1, adc_channel_0, 0, sample_time_84cyc);
	adc_channel_config(ADC1, adc_channel_2, 1, sample_time_84cyc);	
	
	while(1)
	{
		adc_channel_conv(ADC1, adc_channel_0);
		adc_val[0] = adc_get_value(ADC1);
		adc_channel_conv(ADC1, adc_channel_1);
		adc_val[1] = adc_get_value(ADC1);	
	}
}

