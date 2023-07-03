#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "gpio.h"
#include "soft_i2c_master.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

/*	I2C Init	*/
void I2C_Master_Init(void)
{
	SDA_OUTPUT;		// Config PIN B7 ~ SDA
	SCL_OUTPUT;		// Config PIN B6 ~ SCL
}

/*	I2C Start	*/
void I2C_Start(void)
{
	// SDA = 1, SCL = 1
	SDA_HIGH;
	SCL_HIGH;
	delay_systick_us(5);
	// SDA = 0, SCL = 1
	SDA_LOW;
	SCL_HIGH;
	delay_systick_us(5);
	// SDA = 0, SCL = 0
	SDA_LOW;
	SCL_LOW;		
}

/*	I2C Stop	*/
void I2C_Stop(void)
{
	// SDA = 0, SCL = 0
	SDA_LOW;
	SCL_LOW;
	delay_systick_us(5);
	// SDA = 0, SCL = 1
	SDA_LOW;
	SCL_HIGH;
	delay_systick_us(5);
	// SDA = 1, SCL = 1
	SDA_HIGH;
	SCL_HIGH;		
}

/*	ACK bit I2C	*/
void I2C_Master_Ack(void)
{
	// SCL = 0, SDA = 0
	SCL_LOW;
	SDA_LOW;
	delay_systick_us(5);	
	// SCL = 1, SDA = 0
	SCL_HIGH;
	SDA_LOW;
	delay_systick_us(5);
	// SCL = 0, SDA = 0
	SCL_LOW;
	SDA_LOW;	
}

/*	NACK bit I2C	*/
void I2C_Master_NAck(void)
{
	// SCL = 0, SDA = 1
	SCL_LOW;
	SDA_HIGH;
	delay_systick_us(5);	
	// SCL = 1, SDA = 1jj
	SCL_HIGH;
	SDA_HIGH;
	delay_systick_us(5);
	// SCL = 0, SDA = 1
	SCL_LOW;
	SDA_HIGH;		
}

/*	Read bit ACK or NACK	*/
uint8_t I2C_Master_ReadACK(void)
{
	uint8_t ack;
	// Config SDA INPUT
	SDA_INPUT;
	// SCL = 0, start new bit
	SCL_LOW;
	// Delay time
	delay_systick_us(5);
	// SCL = 1 to stable value at SDA
	SCL_HIGH;
	// Read SDA
	ack = SDA_Check;
	// Delay time
	delay_systick_us(5);
	// SCL = 0 to end of clock
	SCL_LOW;
	// Reconfig SDA
	SDA_OUTPUT;
	// return ack
	return ack;
}

/*	I2C Send Byte	*/
void I2C_Master_Send_1Byte(uint8_t w_data)
{
	// config SDA is output
	SDA_OUTPUT;	
	// SCL = 0 to change data at SDA Pin
	SCL_LOW;
	/*	Send 1 byte data	*/
	for(uint8_t i = 0; i < 8; i++)
	{
		// Send each bit of data
		if((w_data&0x80) == 0)	SDA_LOW;
		else					SDA_HIGH;
		// shift bit to the left to continue transmit next bit			
		w_data = w_data << 1;			
		delay_systick_us(5);
		// Emit high level of a pulse clock to transmit data
		SCL_HIGH;	
		delay_systick_us(5);
		// Set SCL = 0 to waiting next bit 
		SCL_LOW;	
	}	
	// Check bit ACK
	if(I2C_Master_ReadACK() == 1)
	{
		// Receive NACK signal, stop transmit data
		I2C_Stop();
	}
}

/*	I2C Receive byte	*/
uint8_t I2C_Master_Read_1Byte()
{
	// Config SDA is input
	SDA_INPUT;
	// 
	uint8_t data;
	// Get data
	for(uint8_t i = 0; i < 8; i++)
	{
		// Set SCL = 0 to can change data of SDA
		SCL_LOW;
		delay_systick_us(5);
		// Set SCL = 1 to stable value of SDA
		SCL_HIGH;
		delay_systick_us(5);		
		// shift bit to the left to continue transmit next bit
		data = data << 1;
		// Read value PIN SDA to save data
		if(SDA_Check == PIN_SET)	data |= 0x01;
		else 						data &= ~0x01;
	}		
	// Return data
	return data;
}

/*	I2C Transmit multi byte	*/
void I2C_Master_Send_Data(uint8_t address_slave, RW_Data Write_data)
{
	// Start I2C
	I2C_Start();
	// Transmit next 8 bit to verify address : Transmit 7bit address + 1 bit write status
	I2C_Master_Send_1Byte(address_slave);
	// Tramsmit data
	for(int i = 0; i < Write_data.length; i++)
	{
		// Send data
		I2C_Master_Send_1Byte(Write_data.data[i]);
	}
	// End of transmit data
	I2C_Stop();
}

/*	I2C Receive multi byte	*/
void I2C_Master_Read_Data(uint8_t address, RW_Data *Read_data)
{
	// Create variable check
	bool check = true;
	uint8_t i = 0;
	// Start I2C
	I2C_Start();	
	// Create new variable address
	uint8_t address_slave = (address << 1) | 0x01;
	// Transmit next 8 bit to verify address : Transmit 7bit address + 1 bit write status
	I2C_Master_Send_1Byte(address_slave);	
	// Tramsmit data
	while(check)
	{	
		// Receive data
		uint8_t receive_data = I2C_Master_Read_1Byte();
		if(receive_data != 0)
		{
			// Read data form slave
			Read_data->data[i] = receive_data;
			i++;
			// Ceate Ack bit to feedback
			I2C_Master_Ack();
		}
		else
		{
			// break while loop
			check = false;
		}
	}	
	Read_data->length = i;
	// Create NAck bit to end of read data
	I2C_Master_NAck();
	// End of transmit data
	I2C_Stop();
}




