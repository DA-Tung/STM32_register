#ifndef _SPI_H_
#define _SPI_H_


#include "stm32f4xx.h"
#include "stm32f407xx.h"

void spi_init_master(SPI_TypeDef* SPIx);
void spi_init_slave(SPI_TypeDef* SPIx);
void spi_write(SPI_TypeDef* SPIx, uint8_t* data, uint8_t length);
void spi_read(SPI_TypeDef* SPIx, uint8_t* data, uint8_t length);
void spi_readwrite(SPI_TypeDef* SPIx, uint8_t* write_data, uint8_t* read_data, uint8_t length);


#endif





