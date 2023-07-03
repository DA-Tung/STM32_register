
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "flash.h"

// Declare variable***********************************************************
uint8_t write_flash_8[5] = {0x11,0x22,0x33,0x44,0x55};
uint8_t read_flash_8[5];

uint16_t write_flash_16[5] = {0x11AA,0x22BB,0x33CC,0x44DD,0x55EE};
uint16_t read_flash_16[5];

uint32_t write_flash_32[5] = {0x11AA77BB,0x22BB88CC,0x33CC44FF,0x44DD99CC,0x55EE6677};
uint32_t read_flash_32[5];

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	flash_unlock();
	flash_erase(flash_sector_6);		
	flash_write_data(addr_flash_sector_6,0,write_flash_16,5,flash_type_word);
	flash_lock();	
	flash_read_data(addr_flash_sector_6,0,write_flash_16,5,flash_type_word);
	
	while(1)
	{

	}
}




