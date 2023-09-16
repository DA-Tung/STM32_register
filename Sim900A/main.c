
// System***********************************************************
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Library***********************************************************
#include "main.h"
#include "usart.h"
#include "systick.h"
#include "gpio.h"
#include "Sim900A.h"

// Declare variable***********************************************************

char GsmTest[10] = "AT\r\n";
char ActivateTxMode[20] = "AT+CMGF=1\r\n";
char PhoneNum[30] = "AT+CMGS=\"+84359932752\"\r\n";
char Message[100] = "Hello!!!";
int CtrlZ = 26;
char ch[1];
char reply[20];

extern Message_Typedef Message_Details;

// Function***********************************************************
void GPIO_ConfigPin(void);

void GPIO_ConfigPin(void)
{
	// Config Pin
	usart_ConfigPin();
	
	gpio_config(GPIOD, PORTD, PIN13, PIN_OUTPUT);
}


// Program main***********************************************************
int main()
{
	SysClock_configure();
	
	GPIO_ConfigPin();
	
	usart_init(USART2);
	
	//SIM900Init();
	ch[0] = CtrlZ;
	
	usart_transmit_multi_data(USART2, (uint8_t *)GsmTest, strlen(GsmTest));
	usart_receiver_string(USART2, reply, 10);
	delay_systick_ms(1000);
	usart_transmit_multi_data(USART2, (uint8_t *)reply, strlen(reply));
	
	usart_transmit_multi_data(USART2, (uint8_t *)ActivateTxMode, strlen(ActivateTxMode));
	usart_receiver_string(USART2, reply, 10);
	delay_systick_ms(1000);
	usart_transmit_multi_data(USART2, (uint8_t *)reply, strlen(reply));	
	
	usart_transmit_multi_data(USART2, (uint8_t *)PhoneNum, strlen(PhoneNum));
	usart_receiver_string(USART2, reply, 10);
	delay_systick_ms(1000);
	usart_transmit_multi_data(USART2, (uint8_t *)reply, strlen(reply));
	
	usart_transmit_multi_data(USART2, (uint8_t *)Message, strlen(Message));
	usart_receiver_string(USART2, reply, 10);
	delay_systick_ms(1000);
	usart_transmit_multi_data(USART2, (uint8_t *)reply, strlen(reply));	

	usart_transmit_multi_data(USART2, (uint8_t *)ch, strlen(ch));
	usart_receiver_string(USART2, reply, 10);
	delay_systick_ms(1000);
	usart_transmit_multi_data(USART2, (uint8_t *)reply, strlen(reply));	
	
	while(1)
	{
			
	}
}






