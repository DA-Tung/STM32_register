#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "systick.h"
#include "modbus_rtu.h"
#include "usart.h"
#include "gpio.h"
#include "crc.h"

#define USART_MODBUS				USART2

#define TX_ENABLE					gpio_output(GPIOA, PIN4, PIN_SET)
#define TX_DISENABLE				gpio_output(GPIOA, PIN4, PIN_RESET)

//// MODBUS CRC_______________________________________________________________
//uint16_t modbus_crc(uint8_t *data_crc, uint8_t count_crc)
//{
//	uint16_t crc_calc = 0xFFFF;
//	
//	for (int j = 0; j < count_crc; j++)
//	{
//		crc_calc = crc_calc ^ (uint16_t)(*data_crc);
//		
//		for(int i = 0; i < 8; i++)
//		{
//			if(crc_calc & 0x0001)
//			{
//				crc_calc = (crc_calc >> 1) ^ 0xA001;
//			}
//			else
//			{
//				crc_calc >>= 1;		
//			}
//		}
//	}
//	return crc_calc;
//}

// MODBUS READ QUERY _______________________________________________________________
void modbus_read_query(MB_READ_QUERY *Data_Read_Query)
{
	// Start Tx
	TX_ENABLE;	
	
	uint8_t Buffer_Read_Query[8];
	
	// Address Slave
	Buffer_Read_Query[0] = Data_Read_Query->Addr_Slave;
	
	// Funcion Code
	Buffer_Read_Query[1] = Data_Read_Query->Func_Code;
	
	// Start Address
	Buffer_Read_Query[2] = (Data_Read_Query->Start_Addr >> 8) & 0xFF;
	Buffer_Read_Query[3] = Data_Read_Query->Start_Addr &0xFF;
	
	// Number register
	Buffer_Read_Query[4] = (Data_Read_Query->Num_Reg >> 8) & 0xFF;
	Buffer_Read_Query[5] = Data_Read_Query->Num_Reg &0xFF;	
	
	// Check CRC
	uint16_t crc_data = crc16(Buffer_Read_Query,6);
	
	//Read check CRC
	Buffer_Read_Query[6] = crc_data & 0xFF;
	Buffer_Read_Query[7] = (crc_data >> 8) & 0xFF;
	
	// Send data
	usart_transmit_multi_data(USART_MODBUS, Buffer_Read_Query, 8);	
	
	// End Tx
	TX_DISENABLE;
}

// MODBUS READ RESPONSE _______________________________________________________________
void modbus_read_response(MB_READ_RESPONSE *Data_Read_Response)
{
	uint8_t Buffer_Read_Response[256];

	// Start Tx
	TX_ENABLE;	

	// Address Slave
	Buffer_Read_Response[0] = Data_Read_Response->Addr_Slave;
	
	// Funcion Code
	Buffer_Read_Response[1] = Data_Read_Response->Func_Code;	
	
	// Byte count
	Buffer_Read_Response[2] = (Data_Read_Response->Byte_Count)*2;
	
	int index = 3;
	int StartAddr = Data_Read_Response->StartAddrData;
	
	// Save data to buffer
	for(int i = 0; i < Data_Read_Response->Byte_Count; i++)
	{
		Buffer_Read_Response[index++] = (Data_Read_Response->Value_Data[StartAddr] >> 8) & 0xFF;
		Buffer_Read_Response[index++] = Data_Read_Response->Value_Data[StartAddr] & 0xFF;
		StartAddr++;
	}	

	// Check CRC
	uint16_t crc_data = crc16(Buffer_Read_Response,index);
	
	//Read check CRC
	Buffer_Read_Response[index] = crc_data & 0xFF;
	Buffer_Read_Response[index+1] = (crc_data >> 8) & 0xFF;
	
	// Send data
	usart_transmit_multi_data(USART_MODBUS, Buffer_Read_Response, index+2);
	
	// End Tx	
	TX_DISENABLE;	
} 








