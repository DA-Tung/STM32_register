
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "spi.h"

#define INFO1               0x0D
#define INFO2               0X0E

#define WHO_AM_I            0x0F

#define CTRL_REG1           0x21
#define CTRL_REG2           0x22
#define CTRL_REG3           0x23
#define CTRL_REG4           0x20
#define CTRL_REG5           0x24
#define CTRL_REG6           0x25

#define STATUS              0x27

#define OUT_T               0x0C
#define OFF_X               0x10
#define OFF_Y               0x11
#define OFF_Z               0x12

#define CS_X                0x13
#define CS_Y                0x14
#define CS_Z                0x15
#define LC_L                0x16
#define LC_H                0x17

#define STAT                0x18

#define OUT_X_L             0x28
#define OUT_X_H             0x29
#define OUT_Y_L             0x2A
#define OUT_Y_H             0x2B
#define OUT_Z_L             0x2C
#define OUT_Z_H             0x2D

// Measurement range
#define FULL_SCALE_2        0x00
#define FULL_SCALE_4        0x08
#define FULL_SCALE_6        0x10
#define FULL_SCALE_8        0x18
#define FULL_SCALE_16       0x20

// Sensitivity parameter
#define SENSITIVITY_2G         1  // g = 0.06
#define SENSITIVITY_4G         2  // g = 0.12
#define SENSITIVITY_6G         3  // g = 0.18
#define SENSITIVITY_8G         4  // g = 0.24
#define SENSITIVITY_16G        5  // g = 0.73

// Output data rate 
#define ODR_POWER_DOWN      0x00
#define ODR_3_125Hz         0x10
#define ODR_6_25Hz          0x20
#define ODR_12_5Hz          0x30
#define ODR_25Hz            0x40
#define ODR_50Hz            0x50
#define ODR_100Hz           0x60
#define ODR_400Hz           0x70
#define ODR_800Hz           0x80
#define ODR_1600Hz          0x90
  
// Anti-aliasing filter bandwidthfilter bandwidth
#define BW_FILTER_800Hz     0x00
#define BW_FILTER_400Hz     0x40
#define BW_FILTER_200Hz     0x80
#define BW_FILTER_50Hz      0xC0


void Lis3dsh_write_data(uint8_t addr_reg, uint8_t *data_write);
void Lis3dsh_read_data(uint8_t addr_reg, uint8_t *data_read);
void Lis3dsh_init(uint8_t scale);
void Lis3dsh_get_value(uint8_t scale, int16_t *raw_data);
static uint8_t Lis3dsh_readwrite(uint8_t rw_data);




