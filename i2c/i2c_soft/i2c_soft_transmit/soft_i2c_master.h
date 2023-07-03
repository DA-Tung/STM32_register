#ifndef _I2C_SOFT_MASTER_H_
#define _I2C_SOFT_MASTER_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

/* Declare */
#define SDA_PIN		7
#define SCL_PIN		6

#define SDA_INPUT	gpio_config(GPIOB,PORTB,SDA_PIN, PIN_INPUT)		// SDA is input pin
#define SDA_OUTPUT	gpio_config(GPIOB,PORTB,SDA_PIN, PIN_OUTPUT)	// SDA is output pin
#define SCL_OUTPUT	gpio_config(GPIOB,PORTB,SCL_PIN, PIN_OUTPUT)	// SCL is output pin

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

void I2C_Master_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Master_Ack(void);
void I2C_Master_NAck(void);
uint8_t I2C_Master_ReadACK(void);
void I2C_Master_Send_1Byte(uint8_t w_data);
uint8_t I2C_Master_Read_1Byte(void);
void I2C_Master_Send_Data(uint8_t address_slave, RW_Data Write_data);
void I2C_Master_Read_Data(uint8_t address_slave, RW_Data *Read_data);

#endif 





