#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "string.h"
#include "enc28j60_spi.h"
#include "spi.h"
#include "systick.h"
#include "gpio.h"
#include "ARP.h"
#include "NET.h"

#define Buffer_Length  512

uint8_t	eth_buffer[Buffer_Length];

// NET Send Frame_______________________________________________________________
void NET_SendFrame(uint8_t *EthFrame, uint16_t length)
{
	enc28j60_write_packet(ERX_Start, length, EthFrame);
}

// NET Read Frame_______________________________________________________________
void NET_Read(uint8_t *Net_Buffer, uint16_t length)
{
	if(length > 41)
	{
		if(Net_Buffer[12] == 0x08 && Net_Buffer[13] == 0x06)
		{
			ARP_read_request(Net_Buffer,length);
			gpio_output(GPIOD,PIN12,PIN_TOGGLE);
		}
	}
}

// NET Check_______________________________________________________________
void NET_Check(void)
{
	uint16_t length;
	while(1)
	{
		length = enc28j60_read_packet(eth_buffer);
		if(length == 0) return;
		else NET_Read(eth_buffer,length);
	}
}




