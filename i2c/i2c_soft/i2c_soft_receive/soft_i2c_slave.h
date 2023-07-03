#ifndef _I2C_SOFT_Slave_H_
#define _I2C_SOFT_Slave_H_

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "gpio.h"

#define SDA_PIN		7
#define SCL_PIN		6

#define SCL_INPUT	gpio_config(GPIOB,PORTB,SCL_PIN,PIN_INPUT)		// SCL is output pin
#define SCL_OUTPUT	gpio_config(GPIOB,PORTB,SCL_PIN, PIN_OUTPUT)	// SCL is output pin
#define SDA_INPUT	gpio_config(GPIOB,PORTB,SDA_PIN,PIN_INPUT)		// SDA is input pin
#define SDA_OUTPUT	gpio_config(GPIOB,PORTB,SDA_PIN,PIN_OUTPUT)		// SDA is output pin

#define SCL_Check	gpio_input(GPIOB,SCL_PIN)						// Check status PIN SDA
#define SDA_Check	gpio_input(GPIOB,SDA_PIN)						// Check status PIN SDA

#define SDA_HIGH	gpio_output(GPIOB,SDA_PIN,PIN_SET)				// Set SDA = 1
#define SDA_LOW		gpio_output(GPIOB,SDA_PIN,PIN_RESET)			// Reset SDA
#define SCL_HIGH	gpio_output(GPIOB,SCL_PIN,PIN_SET)				// Ser SCL = 1
#define SCL_LOW		gpio_output(GPIOB,SCL_PIN,PIN_RESET)			// Reset SCL

// Create struct
typedef struct{
	uint8_t* data;
	uint8_t length;
}RW_Data;

// Create enum
typedef struct{
	RW_Data  Send_Data;
	RW_Data  Read_Data;
}Type_Data;


void I2C_Slave_Init(void);
void I2C_Slave_Ack(void);
void I2C_Slave_NAck(void);
uint8_t I2C_Slave_ReadACK(void);
uint8_t I2C_Slave_Read_Byte(void);
void I2C_Slave_Send_Byte(uint8_t w_data);
void I2C_Slave_ReadWrite_Data(uint8_t address, Type_Data *rw_data);

#endif 





