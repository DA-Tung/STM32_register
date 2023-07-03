
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "Lis3dsh_spi.h"

// Declare variable***********************************************************
int16_t value_axis_g[3];


// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
  spi_init_master(SPI1);	
  Lis3dsh_init(FULL_SCALE_8);
	
	while(1)
	{
		Lis3dsh_get_value(SENSITIVITY_8G, value_axis_g);
		delay_systick_ms(1);		
	}
}

