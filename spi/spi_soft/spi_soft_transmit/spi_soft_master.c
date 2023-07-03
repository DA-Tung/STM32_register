#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "gpio.h"
#include "systick.h"
#include "spi_soft_master.h"

// SPI funcion clock
void SPI_Master_Clock(void)
{
	// Set SCK
	SCK_SET;
	delay_systick_us(5);
	// Reset SCK
	SCK_RST;
	delay_systick_us(5);
}

// SPI Master Init
void SPI_Master_Init(void)
{
	// Config SS PIN
	SS_CONFIG;
	SS_DISENABLE;
	// Config SCK
	SCK_CONFIG;
	SCK_RST;
	// Config MOSI
	MOSI_CONFIG;
	MOSI_RST;
	// Config MISO	
	MISO_CONFIG;
}

// SPI Master transmit
/*
	Funcion : SPI Master transmit
	Purpure : transmit 1 byte form MOSI Pin
*/
void SPI_Master_Transmit(uint8_t data)
{
	/* program implementation transmit each bit at MOSI Pin	*/
	for(uint8_t i = 0; i < 8; i++)
	{
		if((data&0x80) == 0)	MOSI_RST;		// Check status each bit to transmit data
		else					MOSI_SET; 
		SPI_Master_Clock();						// Emit 1 pulse clock to transmit 1 bit
		data = data << 1;						// shift bit to the left to continue transmit next bit 
	}
}	

// SPI Master receive
/*
	Funcion : SPI Master Receive
	Purpure : Receive 1 byte form MISO Pin
*/
uint8_t SPI_Master_Receive(void)
{
	uint8_t data;
	/* program implementation receive 1 byte at MISO Pin	*/	
	for(uint8_t i = 0; i < 8; i++)
	{
		SPI_Master_Clock();				// Emit 1 pulse clock to receive next bit
		data = data << 1;
		data = data | MISO_PIN;			// Read status of MISO Pin 
	}
	return data;						// Reteurn value of data after end receive
}

// SPI Master transmit and receive
/*
	Funcion : SPI Master transmit and receive
	Purpure : transmit and receive 1 byte 
*/
uint8_t SPI_Master_ReadWrite(uint8_t w_data)           
{
	uint8_t r_data;
	for(uint8_t s_bit = 0x80; s_bit > 0; s_bit = s_bit >> 1)		// Program implementation transmit each bit at MISO Pin
	{
		if((w_data&s_bit) == 0)	MOSI_RST;							// Check status each bit to transmit data
		else					MOSI_SET; 	 

		delay_systick_us(10);										
		SCK_SET;													// Set SCK is high level to read data
		if(MISO_PIN == BIT_SET)			r_data = r_data | s_bit;	// Read status of MISO Pin 
		delay_systick_us(10);										
		SCK_RST;													// Set SCK is low level to end of 1 clock cycle
	}
	return r_data;
}






