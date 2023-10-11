// System***********************************************************
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "rfid_rc522.h"
//#include "lcd_i2c.h"
#include "i2c.h"

// Declare variable***********************************************************
uint8_t status;
uint8_t key;
uint8_t test;
uint8_t serNum[5];
uint8_t str[MAX_LEN]; // Max_LEN = 16
uint8_t Key_Card1[4] = {0x93, 0x33, 0x07, 0x50};
uint8_t Key_Card2[4] = {0x53, 0x77, 0x64, 0x50};
uint8_t Key_Card3[4] = {0xCE, 0xDE, 0x5F, 0x1D};
uint8_t Key_Card4[4] = {0xCE, 0xD8, 0x2D, 0x1D};

uint8_t  KEY[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t  KEY2[]={1,2,3,4,5,6};

char str_display[16];
uint8_t pos = 0;

// Function***********************************************************
void GPIO_ConfigPIN(void)
{	
	// Config Pin
	RC522_ConfigPin();
	
	// Config PIN LED
	gpio_config(GPIOD, PORTD,PIN12,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN13,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN14,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN15,PIN_OUTPUT);
}


// Program main***********************************************************
int main()
{
	SysClock_configure();
	
	GPIO_ConfigPIN();
	
	i2c_init(I2C1);
	
	//lcd_init();
	
	spi_init_master(SPI1);
	
	RC522_init();
	
	//lcd_send_cmd(lcd_clear_display);	
	
	while(1)
	{	
		status = RC522_Request(PICC_REQIDL, str);
		status = RC522_Anticoll(str);
		memcpy(serNum,str,5);
				
//		lcd_set_pos(1,0);
//		lcd_send_string(" RFID KEY : ");
//		for(int i = 0; i < 5; i++)
//		{
//			sprintf(str_display, "%d ", serNum[i]);
//			lcd_set_pos(3,pos);
//			lcd_send_string(str_display);	
//			pos += strlen(str_display);
//			delay_systick_ms(500);
//			if(i == 4)
//			{
//				pos = 0;
//				sprintf(str_display, "%s","");				
//			}
//		}			
	}
}




