#ifndef _MODBUS_RTU_
#define _MODBUS_RTU_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define Read_coil_status					0x01     	// Register 0X
#define Read_input_status					0x02     	// Register 1X
#define Read_hodling_reg					0x03     	// Register 4X
#define Read_input_reg						0x04	   	// Register 3X

#define Write_single_coils					0x05     	// Register 0X
#define Write_single_reg					0x06     	// Register 4X
#define Write_multi_coils					0x0E    	// Register 0X
#define Write_multi_reg					  	0x0F    	// Register 4X

#define SLAVE_ID 							5

#define ILLEGAL_FUNCTION       				0x01
#define ILLEGAL_DATA_ADDRESS   				0x02
#define ILLEGAL_DATA_VALUE     				0x03

typedef struct
{
	uint8_t Addr_Slave;
	uint8_t Func_Code;
	uint16_t Start_Addr;
	uint16_t Num_Reg;
} MB_READ_QUERY;

typedef struct
{
	uint8_t Addr_Slave;
	uint8_t Func_Code;
	uint8_t Byte_Count;
	uint16_t StartAddrData;
	uint16_t Value_Data[125];
} MB_READ_RESPONSE;

static uint16_t Holding_Registers_Database[50]={
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999,   // 0-9   40001-40010
		12345, 15432, 15535, 10234, 19876, 13579, 10293, 19827, 13456, 14567,  // 10-19 40011-40020
		21345, 22345, 24567, 25678, 26789, 24680, 20394, 29384, 26937, 27654,  // 20-29 40021-40030
		31245, 31456, 34567, 35678, 36789, 37890, 30948, 34958, 35867, 36092,  // 30-39 40031-40040
		45678, 46789, 47890, 41235, 42356, 43567, 40596, 49586, 48765, 41029,  // 40-49 40041-40050
};

static const uint16_t Input_Registers_Database[50]={
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999,   // 0-9   30001-30010
		12345, 15432, 15535, 10234, 19876, 13579, 10293, 19827, 13456, 14567,  // 10-19 30011-30020
		21345, 22345, 24567, 25678, 26789, 24680, 20394, 29384, 26937, 27654,  // 20-29 30021-30030
		31245, 31456, 34567, 35678, 36789, 37890, 30948, 34958, 35867, 36092,  // 30-39 30031-30040
		45678, 46789, 47890, 41235, 42356, 43567, 40596, 49586, 48765, 41029,  // 40-49 30041-30050
};

uint16_t modbus_crc(uint8_t *data_crc, uint8_t count_crc);

void modbus_read_query(MB_READ_QUERY *Data_Read_Query);
void modbus_read_response(MB_READ_RESPONSE *Data_Read_Response);


#endif // _MODBUS_RTU_

