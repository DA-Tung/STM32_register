
#include "stm32f4xx.h"
#include "stm32f407xx.h"

void i2c_init(I2C_TypeDef *I2Cx);
void i2c_start(I2C_TypeDef *I2Cx);
void i2c_stop(I2C_TypeDef *I2Cx);
void i2c_master_transmit(I2C_TypeDef *I2Cx, uint16_t Adress_slave, uint8_t* data, uint16_t length);
void i2c_master_receive(I2C_TypeDef *I2Cx, uint16_t Adress_slave, uint8_t* data, uint16_t length);



