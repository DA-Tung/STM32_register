// System***********************************************************
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "usart.h"
#include "modbus_rtu.h"
#include "crc.h"

// Declare variable***********************************************************
uint8_t Rx_Data[256];
uint8_t Tx_Data[256];

MB_READ_QUERY Data_read_que;
MB_READ_RESPONSE Data_read_res;

uint16_t data_md[16] = {0x1234,0x2345,0x3456,0x4567,0x5678,0x6789,0x7891,0x8912};

// Function***********************************************************
void GPIO_ConfigPIN(void)
{	
	// Config Pin
	usart_ConfigPin();	
	
	// Config PIN DE/RE
	gpio_config(GPIOA, PORTA, PIN4, PIN_OUTPUT);
	
	// Config PIN LED
	gpio_config(GPIOD, PORTD,PIN12,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN13,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN14,PIN_OUTPUT);
	gpio_config(GPIOD, PORTD,PIN15,PIN_OUTPUT);
}

//void sendData (uint8_t *data)
//{
//	TX_ENABLE;
//	usart_transmit_multi_data(USART2, data, 8);
//	TX_DISENABLE;
//}

// Program main***********************************************************
int main()
{
	SysClock_configure();
	
	GPIO_ConfigPIN();
	
	usart_init(USART2);
		
	// Modbus slave query
//	Data_read_que.Addr_Slave = 0x04;
//	Data_read_que.Func_Code = 0x03;
//	Data_read_que.Start_Addr = 0x0001;
//	Data_read_que.Num_Reg = 0x0005;
//	
//	modbus_read_query(&Data_read_que); 

	// Modbus master response	
	/* Read data */
//	usart_receiver_string(USART2, Rx_Data, 32);	
//	uint16_t StartAddr = ((Rx_Data[2] << 8) | Rx_Data[3]);
//	uint16_t NumRegs =  ((Rx_Data[4] << 8) | Rx_Data[5]);
//	uint16_t EndAddr = StartAddr + NumRegs - 1;	
//	
//	/* Response data */
//	Data_read_res.Addr_Slave = Rx_Data[0];
//	Data_read_res.Func_Code = Rx_Data[1];
//	Data_read_res.Byte_Count = NumRegs;
//	Data_read_res.StartAddrData = StartAddr;
//	for(int i = 0; i < 8; i++)
//	{
//		Data_read_res.Value_Data[i] = data_md[i];
//	}
//	modbus_read_response(&Data_read_res);
	
	while(1)
	{	
		
	}
}




