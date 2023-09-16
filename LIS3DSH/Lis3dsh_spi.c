
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "spi.h"
#include "systick.h"
#include "Lis3dsh_spi.h"
#include "gpio.h"

#define Enable_LIS3DSH      gpio_output(GPIOE,PIN3,PIN_RESET)
#define Disenable_LIS3DSH   gpio_output(GPIOE,PIN3,PIN_SET)
#define LIS3DSH_SPI			SPI1

// LIS3DSH READ_WRITE DATA_______________________________________________________________
uint8_t Lis3dsh_readwrite(uint8_t rw_addr)
{
	uint8_t rw_data = 0;
	
	spi_readwrite(LIS3DSH_SPI, (uint8_t *) &rw_addr, (uint8_t *) &rw_data, 1);
	
	return rw_data;
}

// LIS3DSH WRITE DATA_______________________________________________________________
void Lis3dsh_write_data(uint8_t addr_reg, uint8_t data_write)
{	
//	uint8_t data[2];

//	data[0] = addr_reg;
//	data[1] = data_write;
	
	// Enable SPI
	Enable_LIS3DSH;
	
	// Set register value
	spi_transmit(LIS3DSH_SPI, &addr_reg, 1);
	
	// Transmit data
	spi_transmit(LIS3DSH_SPI, &data_write, 1);
	
//	Lis3dsh_readwrite(addr_reg);
//	
//	Lis3dsh_readwrite(data_write);	
	
	// Disable SPI
	Disenable_LIS3DSH;
}

// LIS3DSH READ DATA_______________________________________________________________
uint8_t Lis3dsh_read_data(uint8_t addr_reg)
{
	uint8_t read_data = 0;
	
	// Set RW = 1 to read data
	addr_reg |= 0x80;
	
	// Enable SPI
	Enable_LIS3DSH;
	
	// Set register value
	spi_transmit(LIS3DSH_SPI, &addr_reg, 1);
	
	// Receive data
	spi_receive(LIS3DSH_SPI, &read_data, 1);
	
//	read_data = Lis3dsh_readwrite(addr_reg);
//	
//	read_data = Lis3dsh_readwrite(0);
	
	// Disenable SPI
	Disenable_LIS3DSH;
	
	return read_data;
}

// LIS3DSH INIT_______________________________________________________________
void Lis3dsh_init(uint8_t scale)
{
	uint8_t config_reg = 0;	
	// Check address
	
	config_reg = Lis3dsh_read_data(WHO_AM_I);
	while(config_reg != 0x3F);
	
	// Config control register 4
	config_reg &= 0;
	config_reg |= ODR_25Hz&0x38;        	// ODR = 25 Hz
	config_reg |= 0x07;            			// Enable X,Y,Z axis
	Lis3dsh_write_data(CTRL_REG4, config_reg);
	
	Lis3dsh_write_data(CTRL_REG1, 0);

	Lis3dsh_write_data(CTRL_REG2, 0);
	
	//Lis3dsh_write_data(CTRL_REG3, 0x88); 	// DR_EN = 1, INT1_EN = 1	
	
	// Config control register 5
	config_reg &= 0;
	config_reg |= BW_FILTER_50Hz&0xC0;  	// Filter bandwidth 50Hz
	config_reg |= scale&0x38;           	// Full scale
	Lis3dsh_write_data(CTRL_REG5, config_reg);	
	
	Lis3dsh_write_data(CTRL_REG6, 0);
}

// LIS3DSH GET VALUE _______________________________________________________________
Value_Axis_G Lis3dsh_get_value(uint8_t sensitivity)
{
	float sensitivity_value;
	int8_t data_axis[6];
	Value_Axis_G value_axis;
	
	// Config sensitivity parameter
	switch(sensitivity)
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
	data_axis[0] = Lis3dsh_read_data(OUT_X_L);
	data_axis[1] = Lis3dsh_read_data(OUT_X_H);
	value_axis.Axis_X = (data_axis[0] | (data_axis[1] << 8))*sensitivity_value;

	// Read value Y
	data_axis[2] = Lis3dsh_read_data(OUT_Y_L);
	data_axis[3] = Lis3dsh_read_data(OUT_Y_H);	
	value_axis.Axis_Y = (data_axis[2] | (data_axis[3] << 8))*sensitivity_value;
	
	// Read value Z
	data_axis[4] = Lis3dsh_read_data(OUT_Z_L);
	data_axis[5] = Lis3dsh_read_data(OUT_Z_H);			
	value_axis.Axis_Z = (data_axis[4] | (data_axis[5] << 8))*sensitivity_value;	
	
	return value_axis;
}

// LIS3DSH GET STATUS _______________________________________________________________
uint8_t Lis3dsh_get_status(void)
{
	uint8_t status;
	
	status = Lis3dsh_read_data(STATUS);
	
	if(status & 0x07)
	{
		return 1;
	}
	return 0;
}





