
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "spi.h"
#include "systick.h"
#include "Lis3dsh_spi.h"
#include "gpio.h"

#define Enable_LIS3DSH      gpio_output(GPIOE,PORTE,PIN3,PIN_RESET);
#define Disenable_LIS3DSH   gpio_output(GPIOE,PORTE,PIN3,PIN_SET);


// LIS3DSH READ_WRITE DATA_______________________________________________________________
static uint8_t Lis3dsh_readwrite(uint8_t rw_addr)
{
	uint8_t rw_data = 0;
	
	spi_readwrite(SPI1, (uint8_t *) &rw_addr, (uint8_t *) &rw_data, 1);
	
	return rw_data;
}

// LIS3DSH WRITE DATA_______________________________________________________________
void Lis3dsh_write_data(uint8_t addr_reg, uint8_t *data_write)
{	
	// Enable SPI
	Enable_LIS3DSH;
	
	// Set register value
	Lis3dsh_readwrite(addr_reg);
	
	// Transmit data 	
	Lis3dsh_readwrite(*data_write);
	
	// Disable SPI
	Disenable_LIS3DSH;
}

// LIS3DSH READ DATA_______________________________________________________________
void Lis3dsh_read_data(uint8_t addr_reg, uint8_t *data_read)
{
	// Set RW = 1 to read data
	addr_reg |= 0x80;
	
	// Enable SPI
	Enable_LIS3DSH;
	
	// Set register value
	Lis3dsh_readwrite(addr_reg);
	
	// Receive data
	*data_read = Lis3dsh_readwrite(0);
	
	// Disenable SPI
	Disenable_LIS3DSH;
}

// LIS3DSH INIT_______________________________________________________________
void Lis3dsh_init(uint8_t scale)
{
	uint8_t config_reg = 0;
	
	// Init SPI
	spi_init_master(SPI1);
	
	// Check address
	
//	Lis3dsh_read_data(WHO_AM_I, &config_reg);
//	while(config_reg != 0x3F);
	
	// Config control register 4
	config_reg = 0;
	config_reg |= ODR_25Hz;        // ODR = 25 Hz
	config_reg |= 0x07;            // Enable X,Y,Z axis
	Lis3dsh_write_data(CTRL_REG4, &config_reg);
	
	// Config control register 5
	config_reg = 0;
	config_reg |= BW_FILTER_50Hz;  // Filter bandwidth 50Hz
	config_reg |= scale;           // Full scale
	Lis3dsh_write_data(CTRL_REG5, &config_reg);	
}

// LIS3DSH GET VALUE _______________________________________________________________
void Lis3dsh_get_value(uint8_t scale, int16_t *raw_data)
{
	float sensitivity_value;
	int8_t data_axis[6];
	
	// Config sensitivity parameter
	switch(scale)
	{
		case SENSITIVITY_2G :
			sensitivity_value = 0.06;
			break;
		
		case SENSITIVITY_4G :
			sensitivity_value = 0.12;
			break;
		
		case SENSITIVITY_6G :
			sensitivity_value = 0.18;
			break;
				
		case SENSITIVITY_8G :
			sensitivity_value = 0.24;
			break;
		
		case SENSITIVITY_16G :
			sensitivity_value = 0.73;
			break;		
	}
	
	// Read value X
	Lis3dsh_read_data(OUT_X_L, (uint8_t *)&data_axis[0]);
	Lis3dsh_read_data(OUT_X_H, (uint8_t *)&data_axis[1]);
	raw_data[0] = (data_axis[0] | (data_axis[1] << 8))*sensitivity_value;

	// Read value Y
	Lis3dsh_read_data(OUT_Y_L, (uint8_t *)&data_axis[2]);
	Lis3dsh_read_data(OUT_Y_H, (uint8_t *)&data_axis[3]);
	raw_data[1] = (data_axis[2] | (data_axis[3] << 8))*sensitivity_value;
	
	// Read value Z
	Lis3dsh_read_data(OUT_Z_L, (uint8_t *)&data_axis[4]);
	Lis3dsh_read_data(OUT_Z_H, (uint8_t *)&data_axis[5]);
	raw_data[2] = (data_axis[4] | (data_axis[5] << 8))*sensitivity_value;	
}





