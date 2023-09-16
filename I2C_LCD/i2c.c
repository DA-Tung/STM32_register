
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "i2c.h"
#include "systick.h"

// I2C INIT_______________________________________________________________
void i2c_init(I2C_TypeDef *I2Cx)
{
	#define Freq_i2c 		40
	
	// Enable I2C
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN | RCC_APB1ENR_I2C2EN | RCC_APB1ENR_I2C3EN;
	
	if(I2Cx == I2C1)
	{
		// Enable GPIO
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		
		// Select alternate funcion mode SCL = PB6, SDA = PB7
		GPIOB->MODER |= GPIO_MODER_MODE7_1 | GPIO_MODER_MODE6_1;
		
		// Output open drain
		GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;

		// Port output max speed
		GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
		
		// GPIO pull up
		GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
		
		// AF4 Alternate function for I2C
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL7_2;		
	}	
	else if(I2Cx == I2C2)
	{
		// Enable GPIO
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		
		// Select alternate funcion mode SCL = PB6, SDA = PB7
		GPIOB->MODER |= GPIO_MODER_MODE11_1 | GPIO_MODER_MODE10_1;
		
		// Output open drain
		GPIOB->OTYPER |= GPIO_OTYPER_OT11 | GPIO_OTYPER_OT10;

		// Port output max speed
		GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
		
		// GPIO pull up
		GPIOB->PUPDR |= GPIO_PUPDR_PUPD10_0 | GPIO_PUPDR_PUPD11_0;
		
		// AF4 Alternate function for I2C
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL11_2;		
	}	
	
	// Reset I2C
	I2Cx->CR1 |= I2C_CR1_SWRST;
	I2Cx->CR1 &= ~I2C_CR1_SWRST;
	
	// Disable I2C peripheral 
	I2Cx->CR1 &= ~(I2C_CR1_PE);
	
	// Configure clock frequency : max 40MHz
	I2Cx->CR2 |= (Freq_i2c << 0);

	// Select Fast mode for I2C
	I2Cx->CCR |= I2C_CCR_FS;
			
	// Duty cycle fast mode T_low/T_high = 16/9
	I2Cx->CCR |= I2C_CCR_DUTY;
	
	// Clock control register in master mode (t_high = 900ns) (table 58 - DM00115249)
	I2Cx->CCR |= 36 << 0;
	
	//Maximum rise time in Fast mode (T_rise_max = 300ns, t_freq = 1/40M = 25ns) (table 58 - DM00115249) 
	I2Cx->TRISE |= 13 << 0; 
		
	// Enable I2C peripheral 
	I2Cx->CR1 |= I2C_CR1_PE;
}

// I2C Start_______________________________________________________________
void i2c_start(I2C_TypeDef *I2Cx)
{
	// Start I2C
	I2Cx->CR1 |= I2C_CR1_START;
	
	// Waiting SB bit set 
	while(!(I2Cx->SR1 & I2C_SR1_SB));
}
	
// Stop I2C_______________________________________________________________
void i2c_stop(I2C_TypeDef *I2Cx)
{
	// Stop I2C
	I2Cx->CR1 |= I2C_CR1_STOP;
}

// I2C master transmit data_______________________________________________________________
void i2c_master_transmit(I2C_TypeDef *I2Cx, uint16_t Adress_slave, uint8_t* data, uint16_t length)
{
	//Start I2C
	i2c_start(I2Cx);
	
	// Read address slave
	I2Cx->DR = Adress_slave;
	
	// Waiting bit ADD is set
	while(!(I2Cx->SR1 & I2C_SR1_ADDR));
	
	// Clear bit ADDR
	uint16_t clear_bit = I2Cx->SR1 | I2Cx->SR2;
	
	for(int i = 0; i < length; i++)
	{
		// Waiting bit TXE is set
		while(!(I2Cx->SR1 & I2C_SR1_TXE));
		
		// Writing data in DR register
		I2Cx->DR = *(data+i);

		// Waiting bit BTF is set
		while(!(I2Cx->SR1 & I2C_SR1_BTF));
	}
	//Stop I2C to clear TXE and BTF
	i2c_stop(I2Cx);		
}

// I2C master receiver data_______________________________________________________________
void i2c_master_receive(I2C_TypeDef *I2Cx, uint16_t Adress_slave, uint8_t* data, uint16_t length)
{
	uint8_t i = 0;
	
	//Start I2C
	i2c_start(I2Cx);
	
	// Read address slave
	I2Cx->DR = Adress_slave;

	if(length == 0)
	{
		// Stop I2C
		i2c_stop(I2Cx);
	}
	
	else if(length == 1U)
	{
		//Reset bit ACK
		I2Cx->CR1 &= ~(I2C_CR1_ACK);
		
		// Waiting bit ADD is set
		while(!(I2Cx->SR1 & I2C_SR1_ADDR));
		
		// Clear bit ADDR
		uint16_t clear_bit = I2Cx->SR1 | I2Cx->SR2;

		// Read Data
		*(data + i) = I2Cx->DR;
		
		// Stop I2C
		i2c_stop(I2Cx);
		
		i++;
		length--;
	}
	
	else if(length == 2U)
	{
		// Waiting bit ADD is set
		while(!(I2Cx->SR1 & I2C_SR1_ADDR));

		//Reset bit ACK
		I2Cx->CR1 &= ~(I2C_CR1_ACK);

		// Set bit POS
		I2Cx->CR1 |= I2C_CR1_POS;
		
		// Clear bit ADDR
		uint16_t clear_bit = I2Cx->SR1 | I2Cx->SR2;

		// Waiting bit BTF is set
		while(!(I2Cx->SR1 & I2C_SR1_BTF));

		// Stop I2C
		i2c_stop(I2Cx);

		// Read Data
		*(data + i) = I2Cx->DR;

		i++;
		length--;
	}
	
	else if(length > 2U)
	{
		//Set bit ACK
		I2Cx->CR1 |= I2C_CR1_ACK;		
		
		// Waiting bit ADD is set
		while(!(I2Cx->SR1 & I2C_SR1_ADDR));		
		
		// Clear bit ADDR
		uint16_t clear_bit = I2Cx->SR1 | I2Cx->SR2;

		if(length == 3U)
		{
			// Waiting bit BTF is set
			while(!(I2Cx->SR1 & I2C_SR1_BTF));		
	
			//Reset bit ACK
			I2Cx->CR1 &= ~(I2C_CR1_ACK);	

			// Read data 3
			*(data + i) = I2Cx->DR;
			
			i++;
			length--;
			
			// Waiting bit BTF is set
			while(!(I2Cx->SR1 & I2C_SR1_BTF));

			// Stop I2C
			i2c_stop(I2Cx);		

			// Read data 2
			*(data + i) = I2Cx->DR;
			
			i++;
			length--;	

			// Read data 1
			*(data + i) = I2Cx->DR;
			
			i++;
			length--;
		}
		
		else if(length > 3U)
		{
			// Wait bit RXNE is set
			while(!(I2Cx->SR1 & I2C_SR1_RXNE));
			
			// Read data in DR register
			*(data+i) = I2Cx->DR;
			
			// Wait bit BTF is set
			while(!(I2Cx->SR1 & I2C_SR1_BTF));
			
			i++;
			length--;
		}
	}
}








