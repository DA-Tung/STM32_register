
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "spi.h"

// SPI MASTER Config Pin_______________________________________________________________
void spi_ConfigPin(void)
{
	// Enable GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// Select alternate funcion mode PA5 = SCK, PA6 = MISO, PA7 = MOSI
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	
	// Out push-pull
	GPIOA->OTYPER |= 0;
	
	// No pull up and pull down
	GPIOA->PUPDR |= 0;
	
	// output maximum speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	
	// Alternate funcion AF5
	GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28);
}

// SPI MASTER INIT_______________________________________________________________
void spi_init_master(SPI_TypeDef* SPIx)
{	
	spi_ConfigPin();
	
//	// Disenable SPI
//	SPIx->CR1 &= ~SPI_CR1_SPE;
	
	// Enable SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	// Select master mode
	SPIx->CR1 |= SPI_CR1_MSTR;
	
	// Select serial clock baudrate
	SPIx->CR1 |= SPI_CR1_BR_2;
	
	// Clock polarity low
	SPIx->CR1 &= ~(SPI_CR1_CPOL);
	
	// Clock phase high
	SPIx->CR1 |= SPI_CR1_CPHA;
	
	// Data size 8 bit
	SPIx->CR1 &= ~(SPI_CR1_DFF);
	
	// Select MSB format
	SPIx->CR1 &= ~(SPI_CR1_LSBFIRST);
	
	// Select 2-line unidirectional data
	SPIx->CR1 &= ~SPI_CR1_BIDIMODE;

	// Select full duplex (transmit and receive)
	SPIx->CR1 &= ~SPI_CR1_RXONLY;

	// Software slave management
	SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	
	// Set bit SSOE
	SPIx->CR2 |= SPI_CR2_SSOE;
	
	// Frame format : Motorola mode
	SPIx->CR2 &= ~SPI_CR2_FRF;

	// Enable SPI
	SPIx->CR1 |= SPI_CR1_SPE;
	
	// CRC calculation
	SPIx->CRCPR = 7; 
}

// SPI SLAVE INIT_______________________________________________________________
void spi_init_slave(SPI_TypeDef* SPIx)
{
	// Enable GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	// Select alternate funcion mode PB13 = SCK, PB14 = MISO, PB15 = MOSI and input mode PB12 = NSS
	GPIOB->MODER |= GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1;
	
	// Out push-pull
	GPIOB->OTYPER |= 0;
	
	// No pull up and pull down
	GPIOB->PUPDR |= 0;
	
	// output maximum speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12 | GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR14 | GPIO_OSPEEDER_OSPEEDR15;
	
	// Alternate funcion AF5
	GPIOB->AFR[1] |= (5 << 16) |(5 << 20) |(5 << 24) |(5 << 28);
	
	// Enable SPI2
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	// Clock polarity low
	SPIx->CR1 &= ~SPI_CR1_CPOL;
	
	// Clock phase high
	SPIx->CR1 |= SPI_CR1_CPHA;
	
	// Data size 8 bit
	SPIx->CR1 &= ~(SPI_CR1_DFF);
	
	// Select MSB format
	SPIx->CR1 &= ~SPI_CR1_LSBFIRST;
	
	// Select 2-line unidirectional data
	SPIx->CR1 &= ~SPI_CR1_BIDIMODE;

	// Select full duplex (transmit and receive)
	SPIx->CR1 &= ~SPI_CR1_RXONLY;

	// Software slave management
	SPIx->CR1 &= ~SPI_CR1_SSM;
	
	// Internal slave select
	SPIx->CR1 &= ~SPI_CR1_SSI;
	
	// Frame format : Motorola mode
	SPIx->CR2 &= ~SPI_CR2_FRF;

	// Enable master slave
	SPIx->CR1 &= ~SPI_CR1_MSTR;
	
	// Enable SPI
	SPIx->CR1 |= SPI_CR1_SPE;	
	
//	// CRC calculation
//	SPIx->CRCPR = 7; 	
}

// SPI TRANSMIT_______________________________________________________________
void spi_transmit(SPI_TypeDef* SPIx, uint8_t* data, uint8_t length)
{	
	for(int i = 0; i < length; i++)
	{		
		// Wait until Bsy to set
		while(!(SPIx->SR & SPI_SR_TXE)) {};
			
		// write data
		SPIx->DR = *(data + i);		
	}
}

// SPI RECEIVER_______________________________________________________________
void spi_receive(SPI_TypeDef* SPIx, uint8_t* data, uint8_t length)
{	
	for(int i = 0; i < length; i++)
	{											
		// Set dummy data
		SPIx->DR = 0;
			
		// Wait until RXNE to set			
		while(!(SPIx->SR & SPI_SR_RXNE)){};		
			
		// read data			
		*(data + i) = SPIx->DR;	
	}
}



// SPI READ_WRITE_______________________________________________________________
void spi_readwrite(SPI_TypeDef* SPIx, uint8_t* write_data, uint8_t* read_data, uint8_t length)
{
	uint32_t rw_allow = 1U;
	uint8_t read_count = length;
	uint8_t write_count = length;
	
	// Check count read_write
	while(read_count || write_count)
	{
		// Check if interface is transmit
		if((SPIx->SR & SPI_SR_TXE) && (write_count > 0) && (rw_allow == 1))
		{
			SPIx->DR = *write_data;
			write_data += sizeof(uint8_t);
			write_count--;
			rw_allow = 0;
		}
		
		// Check if interface is receive
		if((SPIx->SR & SPI_SR_RXNE) && (read_count > 0))
		{
			*read_data = SPIx->DR;
			read_data += sizeof(uint8_t);
			read_count--;
			rw_allow = 1;
		}
	}
}




