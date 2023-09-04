#ifndef _MODBUS_RTU_
#define _MODBUS_RTU_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define Read_coil_status					1U     	// Register 0X
#define Read_input_status					2U     	// Register 1X
#define Read_hodling_reg					3U     	// Register 4X
#define Read_input_reg						4U	   	// Register 3X

#define Write_single_coils					5U     	// Register 0X
#define Write_single_reg					6U     	// Register 4X
#define Write_multi_coils					15U    	// Register 0X
#define Write_multi_reg					  	16U    	// Register 4X

typedef struct
{
	uint8_t addr_slave;
	uint8_t funcion_mb;
	uint16_t start_addr;
	uint16_t num_reg;
} modbus_para_read;

typedef struct
{
	uint8_t addr_slave;
	uint8_t funcion_mb;
	uint16_t start_addr;
	uint16_t num_reg;
	uint8_t length_data;
	uint8_t *value_data;
} modbus_para_write;

uint16_t modbus_crc(uint8_t *data_crc, uint8_t count_crc);
void modbus_read_frame(modbus_para_read data_write_mb);
void modbus_write_frame(modbus_para_write data_write_mb);

#endif // _MODBUS_RTU_

