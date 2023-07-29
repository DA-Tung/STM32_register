
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "enc28j60_spi.h"
#include "NET.h"
#include "ARP.h"

// Declare variable***********************************************************
const uint8_t Ip_Addr[4] = {192, 168, 1, 50};
const uint8_t MAC_Addr[6] = {0x08, 0x0F, 0xC3, 0x63, 0x73, 0x19};

// Function***********************************************************
void GPIO_Config_PIN(void);

// GPIO Config
void GPIO_Config_PIN(void)
{
	gpio_config(GPIOD,PORTD,PIN12,PIN_OUTPUT);
}


// Program main***********************************************************
int main()
{
	SysClock_configure();
	GPIO_Config_PIN();
	spi_init_master(SPI1);
	enc28j60_init();
//	uint8_t *IP_data;
//	memcpy(IP_data,Ip_Addr,4);
	while(1)
	{
		NET_Check();
	}
}




