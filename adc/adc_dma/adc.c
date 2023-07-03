#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "adc.h"

// ADC INIT_______________________________________________________________
void adc_init(ADC_TypeDef* ADCx, uint32_t num_channel)
{
//	// Enable GPIOA
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//	
//	// Select analog mode
//	GPIOA->MODER |= GPIO_MODER_MODE0 | GPIO_MODER_MODE1;
//	
//	// No pull up and pull down
//	GPIOA->PUPDR = 0;
	
	// Enable ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	// Select PCLK2 clock
	ADC->CCR |= ADC_CCR_ADCPRE_0;
	
	// 12 bit Resolution
	ADCx->CR1 &= ~(ADC_CR1_RES); 
	
	// Scan mode
	ADCx->CR1 |= ADC_CR1_SCAN;
	
	// Enable continuous conversion mode
	ADCx->CR2 |= ADC_CR2_CONT;
	
	// End of conversion
	ADCx->CR2 |= ADC_CR2_EOCS;
	
	// Right alignment
	ADCx->CR2 &= ~(ADC_CR2_ALIGN);
	
	// Enable start a new transfer
//	ADCx->CR2 |= ADC_CR2_DDS;
	
	// Select sampling time 84 cycle
//	ADCx->SMPR2 |= ADC_SMPR2_SMP0_2| ADC_SMPR2_SMP1_2;
	
	// Select number channels
	ADCx->SQR1 |= ((num_channel - 1U) << 20);

	// Enable DMA ADC
	ADCx->CR2 |= ADC_CR2_DMA;
	
	// Turn on ADC
	ADCx->CR2 |= ADC_CR2_ADON;
}

// ADC channel convert_______________________________________________________________
void adc_channel_conv(ADC_TypeDef* ADCx, char channel)
{
	// Clear SQR3
	ADCx->SQR3 = 0;
	
	// Conversion in regular sequence
	ADCx->SQR3 |= (uint32_t)(channel);	
	
	// Start the conversion
	ADCx->CR2 |= ADC_CR2_SWSTART;	
	
	// Wait EOC flag to set
	while(!(ADCx->SR & ADC_SR_EOC));	
}

// READ DATA ADC_______________________________________________________________
uint16_t adc_get_value(ADC_TypeDef* ADCx)
{
	return ADCx->DR;
}

// ADC START_______________________________________________________________
void adc_start(ADC_TypeDef* ADCx)
{
	// Clear status register
	ADCx->SR &= ~(ADC_SR_OVR | ADC_SR_STRT | ADC_SR_EOC | ADC_SR_AWD);
		
	// DMA Disanable selection
	ADCx->CR2 |= ADC_CR2_DDS;	
	
	// Start the conversion
	ADCx->CR2 |= ADC_CR2_SWSTART;	
}

// CONFIG CHANNEL ADC_______________________________________________________________
void adc_channel_config(ADC_TypeDef* ADCx, uint32_t channel_dma, uint32_t pos, uint32_t sample_time)
{
	// Enable GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// Select analog mode
	GPIOA->MODER |= (3 << (channel_dma*2));
	
	// No pull up and pull down
	GPIOA->PUPDR = 0;	
	
	if(channel_dma < 7U)
	{
		// Clear value before
		ADCx->SQR3 &= ~(ADC_SQR3_SQ1 << (pos * 5));
		
		// Set now value
		ADCx->SQR3 |= channel_dma << (pos * 5);
	}
	
	if((channel_dma >= 7U) && (channel_dma < 13U))
	{
		// Clear value before
		ADCx->SQR2 &= ~(ADC_SQR3_SQ1 << ((pos - 7U) * 5));
		
		// Set now value
		ADCx->SQR2 |= channel_dma << ((pos - 7U) * 5);		
	}
	
	if(channel_dma >= 13U)
	{
		// Clear value before
		ADCx->SQR1 &= ~(ADC_SQR3_SQ1 << ((pos - 13U) * 5));
		
		// Set now value
		ADCx->SQR1 |= channel_dma << ((pos - 13U) * 5);		
	}	
		
	if(channel_dma < 10U)
	{
		// Clear value before
		ADCx->SMPR2 &= ~(ADC_SMPR2_SMP0 << (channel_dma * 3));
		
		// Set now value
		ADCx->SMPR2 |= sample_time << (channel_dma * 3);
	}
	if(channel_dma >= 10U)
	{
		// Clear value before
		ADCx->SMPR1 &= ~(ADC_SMPR2_SMP0 << ((channel_dma - 10U) * 3));
		
		// Set now value
		ADCx->SMPR1 |= sample_time << ((channel_dma - 10U) * 3);
	}	
}



