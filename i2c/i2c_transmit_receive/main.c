
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "i2c.h"

// Declare variable***********************************************************
uint8_t buffer[4] = {0x11,0x22,0x44,0x88};

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	i2c_init(I2C1);
	
	while(1)
	{
		
		i2c_master_transmit(I2C1, 0x4E, buffer, 4);
	}
}

