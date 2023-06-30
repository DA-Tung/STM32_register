
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "dac.h"
#include "dma.h"

// INIT DAC_______________________________________________________________
void dac_config(DAC_TypeDef *DACx, uint8_t channel, uint8_t trigger, uint32_t trigger_para)
{
	// Enable GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// Select ananlog mode
	GPIOA->MODER |= GPIO_MODER_MODE4;
	
	// Select no pull up/pull down
	GPIOA->PUPDR = 0;
	
	// Enable DAC
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	// Clear Control register
	DACx->CR = 0;
	
	if(channel == dac_channel_1)
	{
		// Check TRIGGER bit
		if(trigger == trigger_en)
		{
			// Enable trigger channel 1
			DACx->CR |= DAC_CR_TEN1;
			
			// Select event trigger
			DACx->CR |= 1 << trigger_para;
			
			// Disable wave generation
			DACx->CR &= ~DAC_CR_WAVE1;
		}
		else
		{
			trigger_para = not_trigger_dac;
		}
		// Enable buffer
		DACx->CR &= ~DAC_CR_BOFF1;		
		
		// Enable DAC
		DACx->CR |= DAC_CR_EN1;
	}
	if(channel == dac_channel_2)
	{
		// Check TRIGGER bit
		if(trigger == trigger_en)
		{
			// Enable trigger channel 2
			DACx->CR |= DAC_CR_TEN2;
			
			// Select event trigger
			DACx->CR |= 1 << trigger_para;
			
			// Disable wave generation
			DACx->CR &= ~DAC_CR_WAVE2;
		}
		else
		{
			trigger_para = not_trigger_dac;
		}		
		
		// Enable buffer
		DACx->CR &= ~DAC_CR_BOFF2;		
		
		// Enable DAC
		DACx->CR |= DAC_CR_EN2;		
	}	
}

// SET VALUE DAC_______________________________________________________________
void dac_set_value(DAC_TypeDef *DACx, uint8_t channel, uint8_t alignment, uint32_t value_data)
{
	if(channel == dac_channel_1)
	{
		// 8 bit align right
		if(alignment == dac_align_8b_R1)
		{
			DAC->DHR8R1 = value_data;
		}
		
		// 12 bit align right
		else if(alignment == dac_align_12b_R1)
		{
			DAC->DHR12R1 = value_data;
		}		
		
		// 12 bit align left
		else if(alignment == dac_align_12b_L1)
		{
			DAC->DHR12L1 = value_data << 4;
		}	
	}
	
	else if(channel == dac_channel_2)
	{
		// 8 bit align right
		if(alignment == dac_align_8b_R2)
		{
			DAC->DHR8R2 = value_data;
		}
		
		// 12 bit align right
		else if(alignment == dac_align_12b_R2)
		{
			DAC->DHR12R2 = value_data;
		}		
		
		// 12 bit align left
		else if(alignment == dac_align_12b_L2)
		{
			DAC->DHR12L2 = value_data;
		}	
	}
}

// SET MULTI VALUE DAC_______________________________________________________________
void dac_set_multi_value(DAC_TypeDef *DACx, uint8_t channel, uint8_t alignment, uint32_t *value_data, uint32_t length)
{
	for(int i = 0; i < length; i++)
	{
		dac_set_value(DACx, channel, alignment, *(value_data + i));
	}
}



