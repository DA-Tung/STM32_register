#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "soft_i2c_slave.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

/*	I2C Init	*/
void I2C_Slave_Init(void)
{
	SDA_INPUT;											// Config PIN B7 ~ SDA
	SCL_INPUT;		// Config PIN B6 ~ SCL
}

/*	ACK bit I2C	*/
void I2C_Slave_Ack(void)
{
	// Config SDA,SCL output
	SDA_OUTPUT;
	SCL_OUTPUT;
	// SCL = 0, SDA = 0
	SCL_LOW;
	SDA_LOW;
	delay_systick_us(2);	
	// SCL = 1, SDA = 0
	SCL_HIGH;
	SDA_LOW;
	delay_systick_us(2);
	// SCL = 0, SDA = 0
	SCL_LOW;
	SDA_LOW;	
}

void I2C_Slave_NAck(void)
{
	// Config SDA,SCL output
	SDA_OUTPUT;
	SCL_OUTPUT;	
	// SCL = 0, SDA = 1
	SCL_LOW;
	SDA_HIGH;
	delay_systick_us(2);	
	// SCL = 1, SDA = 1jj
	SCL_HIGH;
	SDA_HIGH;
	delay_systick_us(2);
	// SCL = 0, SDA = 1
	SCL_LOW;
	SDA_HIGH;		
}

/*	Read bit ACK or NACK	*/
uint8_t I2C_Slave_ReadACK(void)
{
	uint8_t ack;
	// Config SDA input, SCL output
	SCL_OUTPUT;
	SDA_INPUT;
	// SCL = 0, start new bit
	SCL_LOW;
	// Delay time
	delay_systick_us(2);
	// SCL = 1 to stable value at SDA
	SCL_HIGH;
	// Read SDA
	ack = SDA_Check;
	// Delay time
	delay_systick_us(2);
	// SCL = 0 to end of clock
	SCL_LOW;
	// Reconfig SDA
	SDA_OUTPUT;
	// return ack
	return ack;
}

/*	I2C send 1 byte	*/
void I2C_Slave_Send_Byte(uint8_t w_data)
{
	// Config SDA output pin, SCL input
	SCL_INPUT;
	SDA_OUTPUT;
	// Send 1 byte
	for(int i = 0; i < 8; i++)
	{
		// Send each bit of data
		if((w_data&0x80) == 0)	SDA_LOW;
		else					SDA_HIGH;
		// Wait SCL is high to transmit data
		while(SCL_Check == BIT_RESET);		
		// Wait SCL is low to end of 1 clock cycle and waiting next bit
		while(SCL_Check == BIT_SET);
		// shift bit to the left to continue transmit next bit
		w_data = w_data << 1;
	}
}

/*	I2C read 1 byte	*/
uint8_t I2C_Slave_Read_Byte(void)
{
	// Config SDA input, SCL input
	SCL_INPUT;
	SDA_INPUT;
	// save byte 
	uint8_t r_data;
	// Wait SCL is low to change data at SDA
	while(SCL_Check == BIT_SET);
	// Send 1 byte
	for(int i = 0; i < 8; i++)
	{
		// Wait SCL is high to stable data at SDA
		while(SCL_Check == BIT_RESET);		
		// shift bit to the left to continue transmit next bit
		r_data = r_data << 1;
		// Check status SDA
		if(SDA_Check == BIT_SET)	r_data |= 0x01;
		else						r_data &= ~0x01;
		// Wait SCL is low to end of 1 clock cycle and waiting next bit
		while(SCL_Check == BIT_SET);			
	}	
	// Return value of r_data
	return r_data;
}

/*	I2C Slave Transmit multi byte	*/
void I2C_Slave_ReadWrite_Data(uint8_t address,  Type_Data *rw_data)
{
	// Create variable check
	bool check = true;	
	// Ceate new variable address to save data which get form master
	uint8_t address_check;
	// Read address form Master
	address_check = I2C_Slave_Read_Byte();
	// Compare address of slave with address which to send form master
	if(address == (address_check >> 1))
	{
		// Check status transmit (read or send)
		if((address_check&0x01) == 0)			// Master send data
		{	
			while(check)
			{
				// Read data
				uint8_t read_data = I2C_Slave_Read_Byte();
				
				if( read_data != 0)						// check receive data is not equal 0
				{
					rw_data->Read_Data.data[rw_data->Read_Data.length] = read_data;
					// increase num data
					rw_data->Read_Data.length++;
					// Send bit ACK
					I2C_Slave_Ack();					
				}
				else
				{
					// break while loop
					check = false;
				}
			}
		}
		else	// Master read data
		{	
			for(int i = 0; i < rw_data->Send_Data.length; i++)
			{
				// Read each byte in data
				I2C_Slave_Send_Byte(rw_data->Send_Data.data[i]);
					// Check bit ACK
				if(I2C_Slave_ReadACK() == 1)
				{
					// Receive NACK signal, stop transmit data
					return;
				}
			}			
		}
	}
}
