/* System */
/*-----------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "main.h"

/* Library*/
/*-----------------------------------------------------------------------------------*/
#include "gpio.h"
#include "systick.h"
#include "soft_i2c_master.h"

/* Declare */
/*-----------------------------------------------------------------------------------*/
uint8_t buffer[8] = {0x10,0x20,0x40,0x80};

/* Funcion */
/*-----------------------------------------------------------------------------------*/

/* Main Program */
/*-----------------------------------------------------------------------------------*/
int main()
{
	SysClock_configure();
	I2C_Master_Init();
	
	while(1)
	{
		I2C_Start();	
		I2C_Master_Send_1Byte(0x4E);		
		for(int i = 0; i < 4; i++)
		{
			I2C_Master_Send_1Byte(buffer[i]);
			delay_systick_ms(500);
		}
		I2C_Stop();		
	}
}



