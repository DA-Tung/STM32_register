
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "NRF24L01.h"
#include "gpio.h"
#include "spi.h"
#include "systick.h"

#define CSN_Enable		     	gpio_output(GPIOA,PORTA,PIN4,PIN_RESET)
#define CSN_DisEnable 			gpio_output(GPIOA,PORTA,PIN4,PIN_SET)

#define CE_DisEnable		     	gpio_output(GPIOA,PORTA,PIN10,PIN_RESET)
#define CE_Enable 			      gpio_output(GPIOA,PORTA,PIN10,PIN_SET)
					

static uint8_t NRF24L01_readwrite(uint8_t rw_addr)
{
	uint8_t rw_data = 0;
	
	spi_readwrite(SPI1, (uint8_t *) &rw_addr, (uint8_t *) &rw_data, 1);
	
	return rw_data;
}

void NRF24L01_write(uint8_t reg, uint8_t data_write)
{
	uint8_t buffer_write[2];
	
	buffer_write[0] = reg | (1 << 5);
	
	buffer_write[1] = data_write;
	
	CSN_Enable;
	
	spi_transmit(SPI1,buffer_write,2);
	
	CSN_DisEnable;
}

uint8_t NRF24L01_read(uint8_t reg)
{
	uint8_t data_read = 0;
		
	CSN_Enable;
	
	spi_transmit(SPI1, &reg,1);
	
	spi_receive(SPI1, &data_read,1);
	
	CSN_DisEnable;
	
	return data_read;
}

void NRF24L01_Flush_Tx(void)
{
	uint8_t data = Flush_Tx;
	
	CSN_Enable;
	
	spi_transmit(SPI1, &data, 1);
	
	CSN_DisEnable;
}

void NRF24L01_Flush_Rx(void)
{	
	uint8_t data = Flush_Rx;
	
	CSN_Enable;
	
	spi_transmit(SPI1, &data, 1);
	
	CSN_DisEnable;
}

void NRF24L01_Init(void)
{
	// Disenable chip to configure 
	CE_DisEnable;
	
	// Select CRC = 1, PWR_UP = 1
	NRF24L01_write(CONFIG, 0x0A);
	
	// Select channel
	NRF24L01_write(EN_AA, 0);
	
	// Enable use channel
	NRF24L01_write(EN_RxAddr, 0);
	
	// Setup address width = 3 byte
	NRF24L01_write(Setup_AW, 0x01);
	
	// Setup auto retransmit 1000us, count = 15
	NRF24L01_write(Setup_Retr, 0x3F);
	
	// Set frequency of the transmit ~ 2400 + 
	NRF24L01_write(RF_CH,50);
	
	// Setup RF	: 1 Mbps, -18dBm
	NRF24L01_write(RF_setup, 0x06);
	
	// Enable dynamic payload length
	NRF24L01_write(DYNPD, 0);
	
	// Feature Register
	NRF24L01_write(FEATURE, 0);
	
	// Enable Chip
	CE_Enable;
}

void NRF24L01_Rx_mode(void)
{
	CE_DisEnable;
	
	
	
}


void NRF24L01_Tx_mode(void)
{
	
}








