#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "systick.h"
#include "modbus_rtu.h"
#include "usart.h"
#include "gpio.h"

#define USART_MODBUS		USART1

#define TX_SEND				gpio_output(GPIOA, PIN4, PIN_SET)
#define RX_SEND				gpio_output(GPIOA, PIN4, PIN_RESET)

// MODBUS CRC_______________________________________________________________
uint16_t modbus_crc(uint8_t *data_crc, uint8_t count_crc)
{
	uint16_t crc_calc = 0xFFFF;
	
	for (int j = 0; j < count_crc; j++)
	{
		crc_calc = crc_calc ^ (uint16_t)(*data_crc);
		
		for(int i = 0; i < 8; i++)
		{
			if(crc_calc & 0x0001)
			{
				crc_calc = (crc_calc >> 1) ^ 0xA001;
			}
			else
			{
				crc_calc >>= 1;		
			}
		}
	}
	return crc_calc;
}

// QUERY READ MODBUS_______________________________________________________________
void modbus_read_frame(modbus_para_read data_read_mb)
{
	RX_SEND;
	
	uint8_t *buffer_read_mb;
	
	uint16_t crc_data;
		
	// Address Slave
	buffer_read_mb[0] = data_read_mb.addr_slave;
	
	// Funcion
	buffer_read_mb[1] = data_read_mb.funcion_mb;
	
	//Address
	buffer_read_mb[2] = (data_read_mb.start_addr >> 8) & 0xFF;
	buffer_read_mb[3] = data_read_mb.start_addr &0xFF;
	
	// Quanlity coils
	buffer_read_mb[4] = (data_read_mb.num_reg >> 8) & 0xFF;
	buffer_read_mb[5] = data_read_mb.num_reg &0xFF;	
	
	// Check CRC
	crc_data = modbus_crc(buffer_read_mb,6);
	
	//Read check CRC
	buffer_read_mb[6] = crc_data & 0xFF;
	buffer_read_mb[7] = (crc_data >> 8) & 0xFF;
	
	// Send data
	usart_send_string(USART_MODBUS, (char *)buffer_read_mb);
}

// QUERY WRITE MODBUS_______________________________________________________________
void modbus_write_frame(modbus_para_write data_write_mb)
{
	TX_SEND; 
	
	uint8_t *buffer_write_mb;

	// Write address slave
	buffer_write_mb[0] = data_write_mb.addr_slave;
	
	// Write funcion code
	buffer_write_mb[1] = data_write_mb.funcion_mb;
	
	// Write start address
	buffer_write_mb[2] = (data_write_mb.start_addr >> 8) & 0xFF;
	buffer_write_mb[3] = data_write_mb.start_addr & 0xFF;
	
	// Write quanlity register
	buffer_write_mb[4] = (data_write_mb.num_reg >> 8) & 0xFF;
	buffer_write_mb[5] = data_write_mb.num_reg & 0xFF;
	
	// Write length of data
	buffer_write_mb[6] = data_write_mb.length_data;
	
	// Save data to buffer
	for(int i = 0; i < buffer_write_mb[6]; i++)
	{
		buffer_write_mb[7 + i*2] = data_write_mb.value_data[i*2];
		buffer_write_mb[8 + i*2] = data_write_mb.value_data[i*2 + 1];
	}
	
	// Check CRC
	uint8_t count = data_write_mb.length_data*2 + 6;
	
	uint16_t crc_data = modbus_crc(buffer_write_mb,count);
	
	buffer_write_mb[count + 1] = crc_data & 0xFF;
	buffer_write_mb[count + 2] = (crc_data >> 8) & 0xFF;
	
	// Send data
	usart_send_string(USART_MODBUS, (char *)buffer_write_mb);	
}	





