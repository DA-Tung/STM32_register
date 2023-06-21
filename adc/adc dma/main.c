
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "adc.h"
#include "dma.h"

// Declare variable***********************************************************
uint16_t adc_val[2];

dma_para_config data_dma;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	adc_init(ADC1,2);

	data_dma.channel_stream = dma_channel_0;
	data_dma.type_data = Peripheral_to_Memory;
	data_dma.size_data = dma_data_half_word;
	data_dma.priority_level = 0;
	data_dma.type_irq = DMA2_Stream0_IRQn;
	dma_config(DMA2_Stream0, data_dma); 

	dma_get_data(DMA2_Stream0, (uint32_t)&(ADC1->DR),(uint32_t)adc_val,2);	
	adc_start(ADC1);	
	
	while(1)
	{

	}
}

