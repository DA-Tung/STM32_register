#ifndef _SPI_SOFT_MASTER_H_
#define _SPI_SOFT_MASTER_H_
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Declare Master PIN
#define Master_SS			4
#define Master_SCK			5
#define Master_MISO			6
#define Master_MOSI			7

// Config Master PIN
#define SS_CONFIG					gpio_config(GPIOA,PORTA,Master_SS,PIN_OUTPUT)		// Config SS Pin of Master is OUTPUT
#define SCK_CONFIG					gpio_config(GPIOA,PORTA,Master_SCK,PIN_OUTPUT)		// Config SCK Pin of Master is OUTPUT
#define MOSI_CONFIG					gpio_config(GPIOA,PORTA,Master_MOSI,PIN_OUTPUT)		// Config MOSI Pin of Master is OUTPUT
#define MISO_CONFIG					gpio_config(GPIOA,PORTA,Master_MISO,PIN_INPUT)		// Config MISO Pin of Master is INPUT
#define BUTTON_CONFIG				gpio_config(GPIOA,PORTA,0,PIN_INPUT)
// Define macro PIN
#define SS_DISENABLE				gpio_output(GPIOA,Master_SS,PIN_SET)			// Set SS Pin of Master
#define SS_ENABLE					gpio_output(GPIOA,Master_SS,PIN_RESET)		// Reset SS Pin of Master
#define SCK_SET						gpio_output(GPIOA,Master_SCK,PIN_SET)			// Set SCK Pin of Master
#define SCK_RST						gpio_output(GPIOA,Master_SCK,PIN_RESET)		// Reset SCK Pin of Master
#define MOSI_SET					gpio_output(GPIOA,Master_MOSI,PIN_SET)		// Set MOSI Pin of Master
#define MOSI_RST					gpio_output(GPIOA,Master_MOSI,PIN_RESET)		// Reset MOSI Pin of Master		
#define MISO_PIN					gpio_input(GPIOA,Master_MISO)					// MISO Pin INPUT
#define BUTTON						gpio_input(GPIOA,0)

// Declare Funcion Master
void SPI_Master_Init(void);								// Funcion init master SPI
void SPI_Master_Clock(void);							// Funcion create a pulse clock
void SPI_Master_Transmit(uint8_t data);					// Funicon transmit 1 byte of master
uint8_t SPI_Master_Receive(void);						// Funcion recieve 1 byte
uint8_t SPI_Master_ReadWrite(uint8_t w_data);			// Funcion transmit and receive 1 byte


#endif



