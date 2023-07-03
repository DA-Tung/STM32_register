
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "usart.h"
#include "dma.h"

// Declare variable***********************************************************
uint8_t rx_data[10];
dma_para_config data_usart;

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	data_usart.channel_stream = dma_channel_4;
	data_usart.type_data = Peripheral_to_Memory;
	data_usart.size_data = dma_data_byte;
	data_usart.priority_level = 1;
	data_usart.type_irq = DMA1_Stream5_IRQn;
	dma_config(DMA1_Stream5, data_usart); 
	
	dma_get_data(DMA1_Stream5,(uint32_t)&(USART2->DR), (uint32_t)rx_data,5);
		
	while(1)
	{

	}
}



