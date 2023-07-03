#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "spi_soft.h"
#include "gpio.h"
#include "systick.h"

// SPI Slave Init
void SPI_Slave_Init(void)
{
	// SS config
	SS_CONFIG;
	// SCK config
	SCK_CONFIG;
	// Mosi config
	MOSI_CONFIG;
	// Miso config
	MISO_CONFIG;
	MISO_RST;
}

// SPI Slave transmit
/*
	Funcion : SPI Slave transmit
	Purpure : transmit 1 byte form MISO Pin
*/
void SPI_Slave_Transmit(uint8_t data)
{
	while(SS_PIN == PIN_SET);
	//program implementation transmit each bit at MISO Pin
	for(uint8_t i = 0; i < 8; i++)
	{
		if((data&0x80) == 0)	MISO_RST;		// Check status each bit to transmit data
		else					MISO_SET;		
		while(SCK_PIN == PIN_RESET);			// Waiting until SCK is High to read data
		while(SCK_PIN == PIN_SET);				// Waiting until SCK is Low to end of 1 clock cycle					
		data = data << 1;						// shift bit to the left to continue transmit next bit
	}
}

// SPI Slave reveice
/*
	Funcion : SPI Slave Receive
	Purpure : Receive 1 byte form MOSI Pin
*/
uint8_t SPI_Slave_Receive(void)
{
	uint8_t data;
	// Wait SS Pin
	while(SS_PIN == PIN_SET);
	// program implementation receive 1 byte at MOSI Pin
	for(uint8_t i = 0; i < 8; i++)
	{	
		while(SCK_PIN == BIT_RESET);	// Waiting until SCK is High to read data
		data = data << 1;				// shift bit to the left to continue transmit next bit
		data = data | MOSI_PIN;			// Get value of each bit		
		while(SCK_PIN == BIT_SET);		// Waiting until SCK is Low to end of 1 clock cycle	
	}
	return data;						// Reteurn value of data after end receive
}

// SPI Slave transmit and receive 1 byte
/*
	Funcion : SPI Slave transmit and receive
	Purpure : transmit and receive 1 byte form master
*/
uint8_t SPI_Slave_ReadWrite(uint8_t w_data)
{
	uint8_t r_data;
	while(SS_PIN == BIT_SET);										// Waiting SPI Master Enable
	for(uint8_t s_bit = 0x80; s_bit > 0; s_bit = s_bit >> 1)		//program implementation transmit each bit at MISO Pin
	{
		if((w_data&s_bit) == 0)			MISO_RST;					// Check status each bit to transmit data
		else							MISO_SET;
		
		while(SCK_PIN == BIT_RESET);								// Waiting until SCK is High to read data				
		if(MOSI_PIN == PIN_SET)			r_data = r_data | s_bit;	// Get value of each bit
		while(SCK_PIN == BIT_SET);									// Waiting until SCK is Low to end of 1 clock cycle			
	}
	return r_data;													// Return value of r_data
}


