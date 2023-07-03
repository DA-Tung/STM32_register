#ifndef _SPI_SOFT_H_
#define _SPI_SOFT_H_
#include "stm32f4xx.h"
#include "stm32f411xe.h"

// Declare Slave PIN
#define Slave_SS			4
#define Slave_SCK			5
#define Slave_MISO			6
#define Slave_MOSI			7

// Config Slave PIN
#define SS_CONFIG					gpio_config(GPIOA,PORTA,Slave_SS,PIN_INPUT)			// Config SS Pin of Slave is INPUT
#define SCK_CONFIG					gpio_config(GPIOA,PORTA,Slave_SCK,PIN_INPUT)		// Config SCK Pin of Slave is INPUT
#define MOSI_CONFIG					gpio_config(GPIOA,PORTA,Slave_MOSI,PIN_INPUT)		// Config MOSI Pin of Slave is INPUT
#define MISO_CONFIG					gpio_config(GPIOA,PORTA,Slave_MISO,PIN_OUTPUT)		// Config MISO Pin of Slave is OUTPUT
#define BUTTON_CONFIG				gpio_config(GPIOA,PORTA,0,PIN_INPUT)
// Define macro PIN
#define SS_PIN						gpio_input(GPIOA,Slave_SS)					// SS Pin input
#define SCK_PIN						gpio_input(GPIOA,Slave_SCK)					// SCK Pin input
#define MOSI_PIN					gpio_input(GPIOA,Slave_MOSI)					// MOSI Pin input
#define MISO_SET					gpio_output(GPIOA,Slave_MISO,PIN_SET)			// Set MISO Pin of Slave
#define MISO_RST					gpio_output(GPIOA,Slave_MISO,PIN_RESET)		// Reset MISO Pin of Slave						// MISO Pin INPUT
#define BUTTON						gpio_input(GPIOA,0)

// Declare Funcion Slave
void SPI_Slave_Init(void);							// Funcion init Slave SPI
void SPI_Slave_Transmit(uint8_t data);				// Funicon transmit 1 byte of Slave
uint8_t SPI_Slave_Receive(void);					// Funcion recieve 1 byte
uint8_t SPI_Slave_ReadWrite(uint8_t w_data);		// Funcion transmit and receive 1 byte

#endif


