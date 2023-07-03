#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "usart_interrupt.h"
#include "string.h"

// USART INIT_______________________________________________________________
void usart_init(USART_TypeDef* USARTx)
{
	// Enable USART
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	// Enable PORT A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		
	// Select alternate funcion mode RX = PA3, TX = PA2
	GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
	
	// Output speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
	
	// AF7 alternate funcion for USART2
	GPIOA->AFR[0] |= (7 << 12) | (7 << 8);
	
	// Clear CR1 register
	USARTx->CR1 = 0;
	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	// Select 8 bit for the word length
	USARTx->CR1 &= ~(USART_CR1_M);
	
	// Select oversampling 16
	USARTx->CR1 &= ~(USART_CR1_OVER8);
	
	// Cofigure bit stop
	USARTx->CR2 &= ~(USART_CR2_STOP);
	
	// Configure baudrate USART : 115200 Kbs
	USARTx->BRR = 0x016D;
	
	// Enable transmitter
	USARTx->CR1 |= USART_CR1_TE;
	
	// Enable receiver
	USARTx->CR1 |= USART_CR1_RE;
	
	// Enable DMA Transmit/Receive
	USARTx->CR3 |= USART_CR3_DMAR;
}

// USART SEND DATA_______________________________________________________________
void usart_send(USART_TypeDef* USARTx, uint8_t data)
{
		// Transmit data
		USARTx->DR = data;	
		
		// After writing the last data anh wait unitl TC = 1
		while(!(USARTx->SR & USART_SR_TC));
}

// USART TRANSMITTER STRING_______________________________________________________________
void usart_send_string(USART_TypeDef* USARTx, char *str)
{
	while(*str) usart_send(USARTx, *str++);

}

// USART RECEIVER DATA_______________________________________________________________
uint8_t usart_receiver(USART_TypeDef* USARTx)
{	
	// Wait RXNE set
	while(!(USARTx->SR & USART_SR_RXNE));
	
	// read value
	return USARTx->DR;
}

// USART RECEIVER STRING_______________________________________________________________
void usart_receiver_string(USART_TypeDef* USARTx, char *str, uint32_t length)
{
	for(int i = 0; i < length; i++)
	{
		*(str + i) = usart_receiver(USARTx);
	}
}

// ENABLE INTERRUPT_______________________________________________________________
void usart_enable_irq(USART_TypeDef *USARTx, IRQn_Type line_irq, uint32_t message_irq, uint32_t level_priority)
{
	USARTx->CR1 |= (1 << message_irq);
	
	NVIC_SetPriority(line_irq, level_priority);
	
	NVIC_EnableIRQ(line_irq);
}

