#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "NRF24L01.h"
#include "gpio.h"
#include "spi.h"
#include "systick.h"

#define CSN_Enable		     	gpio_output(GPIOA,PIN4,PIN_RESET)
#define CSN_DisEnable 			gpio_output(GPIOA,PIN4,PIN_SET)

#define CE_DisEnable		    gpio_output(GPIOA,PIN3,PIN_RESET)
#define CE_Enable 			    gpio_output(GPIOA,PIN3,PIN_SET)
		
#define NRF24L01_SPI			SPI1	
		
// NRF24L01 Readwrite data_______________________________________________________________
static uint8_t NRF24L01_readwrite(uint8_t rw_addr)
{
	uint8_t rw_data = 0;
	
	spi_readwrite(NRF24L01_SPI, (uint8_t *) &rw_addr, (uint8_t *) &rw_data, 1);
	
	return rw_data;
}

// NRF24L01 Write data_______________________________________________________________
void NRF24L01_write_data(uint8_t cmd, uint8_t data_write)
{
	uint8_t buffer_write[2];
	
	buffer_write[0] = cmd | (1 << 5);
	
	buffer_write[1] = data_write;
	
	CSN_Enable;
	
	spi_transmit(NRF24L01_SPI,buffer_write,2);
	
	CSN_DisEnable;
}

// NRF24L01 Write multi data_______________________________________________________________
void NRF24L01_write_multi_data(uint8_t cmd, uint8_t *buffer_data, uint16_t length)
{
	uint8_t addr_cmd;
	
	addr_cmd = cmd | (1 << 5);
		
	CSN_Enable;
	
	spi_transmit(NRF24L01_SPI,&addr_cmd,1);	
	
	spi_transmit(NRF24L01_SPI,buffer_data,length);
	
	CSN_DisEnable;	
}

// NRF24L01 Read data_______________________________________________________________
uint8_t NRF24L01_read_data(uint8_t cmd)
{
	uint8_t data_read = 0;
		
	CSN_Enable;
	
	spi_transmit(NRF24L01_SPI, &cmd,1);
	
	spi_receive(NRF24L01_SPI, &data_read,1);
	
	CSN_DisEnable;
	
	return data_read;
}

// NRF24L01 Read multi data_______________________________________________________________
void NRF24L01_read_multi_data(uint8_t cmd, uint8_t *buffer_data, uint16_t length)
{
	uint8_t data_read = 0;
		
	CSN_Enable;
	
	spi_transmit(NRF24L01_SPI, &cmd,1);
	
	spi_receive(NRF24L01_SPI, buffer_data, length);
	
	CSN_DisEnable;
}

// NRF24L01 Flush Tx_______________________________________________________________
void NRF24L01_Flush_Tx(void)
{
	uint8_t data = Flush_Tx;
	
	CSN_Enable; 
	
	spi_transmit(NRF24L01_SPI, &data, 1);
	
	CSN_DisEnable;
}

// NRF24L01 Flush Rx_______________________________________________________________
void NRF24L01_Flush_Rx(void)
{	
	uint8_t data = Flush_Rx;
	
	CSN_Enable;
	
	spi_transmit(NRF24L01_SPI, &data, 1);
	
	CSN_DisEnable;
}

// NRF24L01 Init_______________________________________________________________
void NRF24L01_Init(void)
{
	// Disenable chip to configure 
	CE_DisEnable;
	
	// Select CRC = 1, PWR_UP = 1
	NRF24L01_write_data(CONFIG, 0x0A);
	
	// Select channel
	NRF24L01_write_data(EN_AA, 0);
	
	// Enable use channel
	NRF24L01_write_data(EN_RXADDR, 0);
	
	// Setup address width = 3 byte
	NRF24L01_write_data(SETUP_AW, 0x01);
	
	// Setup auto retransmit 1000us, count = 15
	NRF24L01_write_data(SETUP_RETR, 0x3F);
	
	// Set frequency of the transmit ~ 2400 + 
	NRF24L01_write_data(RF_CH,50);
	
	// Setup RF	: 1 Mbps, -18dBm
	NRF24L01_write_data(RF_SETUP, 0x06);
	
	// Enable dynamic payload length
	NRF24L01_write_data(DYNPD, 0);
	
	// Feature Register
	NRF24L01_write_data(FEATURE, 0);
	
	// Enable Chip
	CE_Enable;
}

// NRF24L01 Tx mode_______________________________________________________________
void NRF24L01_Tx_mode(uint8_t *addr, uint8_t channel)
{
	// Disable chip before configure Tx Mode
	CE_DisEnable;
	
	// Select channel
	NRF24L01_write_data(RF_CH, channel);	
	
	// Transmit data Tx address
	NRF24L01_write_multi_data(TX_ADDR, addr, 5);
	
	// Config power mode
	uint8_t config_power = NRF24L01_read_data(CONFIG);
	config_power |= (1 << 1);
	NRF24L01_write_data(CONFIG,config_power);
	
	// Enable chip after configure Tx mode
	CE_Enable;
}

// NRF24L01 Transmit_______________________________________________________________
uint8_t NRF24L01_Transmit(uint8_t *buffer_data)
{
	// Enable chip select
	CSN_Enable;
	
	// Transmit command
	uint8_t cmd = W_Tx_Payload;	
	spi_transmit(NRF24L01_SPI,&cmd,1);
	
	// Transmit data
	spi_transmit(NRF24L01_SPI,buffer_data,32);
	
	// Disenable chip select
	CSN_DisEnable;
	
	// delay
	delay_systick_us(1);
	
	// Read data fifo
	uint8_t fifo_data = NRF24L01_read_data(FIFO_STATUS);
	
	// Check data fifo
	if((fifo_data & (1<<4)) && (!(fifo_data & (1 << 3))))
	{
		NRF24L01_Flush_Tx();
		
		return 1;
	}
	
	return 0;
}

// NRF24L01 Rx mode_______________________________________________________________
void NRF24L01_Rx_mode(uint8_t *addr, uint8_t channel, Rx_Mode_Data Rx_Mode)
{
	uint16_t length_pipe;
	
	// Disable chip before configure Tx Mode
	CE_DisEnable;
	
	// Select channel
	NRF24L01_write_data(RF_CH, channel);	
	
	// Select data pipe
	uint8_t En_RxAddr = NRF24L01_read_data(EN_RXADDR);
	En_RxAddr |= (1 << Rx_Mode.ERX_Pipe);
	NRF24L01_write_data(EN_RXADDR, En_RxAddr);
	
	// Write data Rx address
	if(Rx_Mode.ERX_Addr == RX_ADDR_P0 || Rx_Mode.ERX_Addr == RX_ADDR_P1)
	{
		length_pipe = 5;
	}
	else
	{
		length_pipe = 1;
	}
	NRF24L01_write_multi_data(Rx_Mode.ERX_Addr, addr, length_pipe);
	
	//32 bit Payload size for pipe
	NRF24L01_write_data(Rx_Mode.Rx_PW_Pipe,32);
	
	// Config power mode : Power ON, PRX = 1
	uint8_t config_power = NRF24L01_read_data(CONFIG);
	config_power |= (1 << 1) | (1 << 0);
	NRF24L01_write_data(CONFIG,config_power);
	
	// Enable chip after configure Tx mode
	CE_Enable;
}

// NRF24L01 Receive data_______________________________________________________________
void NRF24L01_Receive(uint8_t *buffer_data)
{
	// Enable chip select
	CSN_Enable;
	
	// Transmit command
	uint8_t cmd = R_Rx_Payload;	
	spi_transmit(NRF24L01_SPI, &cmd, 1);
	
	// Receive data payload
	spi_receive(NRF24L01_SPI, buffer_data, 32);
	
	// Disenable chip select
	CSN_DisEnable;
	
	// delay
	delay_systick_us(1);	
	
	NRF24L01_Flush_Rx();
}

// NRF24L01 Check Pipe Num_______________________________________________________________
uint8_t NRF24L01_Check_PipeNum(uint8_t PipeNum)
{
	uint8_t status = NRF24L01_read_data(STATUS);
	if((status & (1<<6)) && (status & (PipeNum << 1)))
	{
		NRF24L01_write_data(STATUS, ( 1<<6));
		
		return 1;
	}
	return 0;
}




