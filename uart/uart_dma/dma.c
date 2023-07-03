
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "dma.h"

// INIT DMA DAC_______________________________________________________________
void dma_config(DMA_Stream_TypeDef * DMAx_Streamx, dma_para_config config_dma)
{
	// Enable DMA1
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN;
	
	// Disable stream
	DMAx_Streamx->CR &= ~DMA_SxCR_EN;	
	
	// Select channel for DMA
	DMAx_Streamx->CR |= (config_dma.channel_stream << 25);

	// Select data transfer direction 
	DMAx_Streamx->CR |= (config_dma.type_data << 6);
	
	// Select low priority
	DMAx_Streamx->CR &= ~DMA_SxCR_PL;
	
	// Incre memory address pointer
	DMAx_Streamx->CR |= DMA_SxCR_MINC;
	
	// don't incre peripheral address pointer
	DMAx_Streamx->CR &= ~DMA_SxCR_PINC;
	
	// Enable circular mode
	DMAx_Streamx->CR |= DMA_SxCR_CIRC;
	
	// No double buffer mode
	DMAx_Streamx->CR &= ~DMA_SxCR_DBM;
	
	// Select size word for memory and peripheral (32 bit)
	DMAx_Streamx->CR |= (config_dma.size_data << 13) | (config_dma.size_data << 11);
	
	//Set priority DMA
	NVIC_SetPriority(config_dma.type_irq, config_dma.priority_level);
	
	// Enable interrupt DMA
	NVIC_EnableIRQ(config_dma.type_irq);
}

void dma_get_data(DMA_Stream_TypeDef * DMAx_Streamx, uint32_t reg_addr, uint32_t m_data, uint16_t length)
{
	// Disable stream
	DMAx_Streamx->CR &= ~DMA_SxCR_EN;	

	// Clear status register
	DMA1->LIFCR = 0xFFFFFFFF;
	DMA1->HIFCR = 0xFFFFFFFF;
	DMA2->LIFCR = 0xFFFFFFFF;
	DMA2->HIFCR = 0xFFFFFFFF;	

	// Configure the total number of data
	DMAx_Streamx->NDTR = length;
	
	// Set the peripheral port register address
	DMAx_Streamx->PAR = reg_addr;
		
	// Set the memory adddress
	DMAx_Streamx->M0AR = m_data;

	// Activate the stream
	DMAx_Streamx->CR |= DMA_SxCR_EN;	
}









