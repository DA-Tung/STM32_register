
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "flash.h"

// Lock Flash_______________________________________________________________
void flash_lock(void)
{
	// Lock FLASH
	FLASH->CR |= FLASH_CR_LOCK;
}

// Unlock Flash_______________________________________________________________
void flash_unlock(void)
{
	// Unlock FLASH
	if(FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}

// Erase Flash_______________________________________________________________
void flash_erase(uint32_t number_sector)
{
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);
	
	// Clear Parallelism size
	FLASH->CR &= ~FLASH_CR_PSIZE;
	
	// Select Parallelism size
	FLASH->CR |= FLASH_CR_PSIZE_1; 
	
	//Active Sector Erase
	FLASH->CR |= FLASH_CR_SER;
	
	// Select sector need erase
	FLASH->CR |= (number_sector << 3);
	
	// Start erase operation
	FLASH->CR |= FLASH_CR_STRT;
	
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);
}

// Write data 1 byte_______________________________________________________________
void flash_write_byte(uint32_t address_flash, uint8_t data_flash)
{
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);
	
	// Clear Parallelism size
	FLASH->CR &= ~FLASH_CR_PSIZE;
	 	
	// Active flash program
	FLASH->CR |= FLASH_CR_PG;
	
	*(uint8_t*)address_flash = data_flash;
	
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);	
}

// Write data haflword_______________________________________________________________
void flash_write_haflword(uint32_t address_flash, uint16_t data_flash)
{
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);
	
	// Clear Parallelism size
	FLASH->CR &= ~FLASH_CR_PSIZE;

	// Clear Parallelism size
	FLASH->CR |= FLASH_CR_PSIZE_0;
	
	// Active flash program
	FLASH->CR |= FLASH_CR_PG;
	
	*(uint16_t*)address_flash = data_flash;
	
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);	
}

// Write data word_______________________________________________________________
void flash_write_word(uint32_t address_flash, uint32_t data_flash)
{
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);
	
	// Clear Parallelism size
	FLASH->CR &= ~FLASH_CR_PSIZE;

	// Clear Parallelism size
	FLASH->CR |= FLASH_CR_PSIZE_1;
	
	// Active flash program
	FLASH->CR |= FLASH_CR_PG;
	
	*(uint32_t*)address_flash = data_flash;
	
	// Wait bit BSY reset
	while(FLASH->SR & FLASH_SR_BSY);	
}

// Write data doubleword_______________________________________________________________
//void flash_write_doubleword(uint32_t address_flash, uint64_t data_flash)
//{
//	// Wait bit BSY reset
//	while(FLASH->SR & FLASH_SR_BSY);
	
//	// Clear Parallelism size
//	FLASH->CR &= ~FLASH_CR_PSIZE;

//	// Clear Parallelism size
//	FLASH->CR |= FLASH_CR_PSIZE;
	
//	// Active flash program
//	FLASH->CR |= FLASH_CR_PG;
	
//  *(uint32_t*)address_flash = (uint32_t)data_flash;	
//	*(uint32_t*)(address_flash+4) = (uint32_t)(data_flash >> 32);	
	
//	// Wait bit BSY reset
//	while(FLASH->SR & FLASH_SR_BSY);	
//}

// Write program flash_______________________________________________________________
void flash_write_data(uint32_t address_flash, uint32_t index, void *data_write, uint32_t length, uint8_t flash_type)
{
	uint32_t address = address_flash + index;
	
	switch(flash_type)
	{
		case flash_type_byte: 
			for(uint32_t i = 0; i < length; i++)
			{
				flash_write_byte(address + i, ((uint8_t*)data_write)[i]);
			}
		break;
			
		case flash_type_haflword: 
			for(uint32_t i = 0; i < length; i++)
			{
				flash_write_haflword(address + 2*i, ((uint16_t*)data_write)[i]);
			}
		break;	

		case flash_type_word: 
			for(uint32_t i = 0; i < length; i++)
			{
				flash_write_word(address+ 4*i, ((uint32_t*)data_write)[i]);
			}
		break;		

//		case flash_type_doubleword: 
//			for(uint32_t i = 0; i < length; i++)
//			{
//				flash_write_doubleword(address + 8*i, ((uint64_t*)data_write)[i]);
//			}
//		break;	
	}
}

// Read program flash_______________________________________________________________
void flash_read_data(uint32_t address_flash, uint32_t index, void *data_read, uint32_t length, uint8_t flash_type)
{
	uint32_t address = address_flash + index;
	
	switch(flash_type)
	{
		case flash_type_byte:
		for(uint32_t i = 0; i < length; i++)
		{
			((uint8_t*)data_read)[i] = *((uint8_t*)(address + i));
		}
		break;
	
		case flash_type_haflword:
		for(uint32_t i = 0; i < length; i++)
		{
			((uint16_t*)data_read)[i] = *((uint16_t*)(address + 2*i));
		}
		break;	

		case flash_type_word:
		for(uint32_t i = 0; i < length; i++)
		{
			((uint32_t*)data_read)[i] = *((uint32_t*)(address + 4*i));
		}
		break;
	
//		case flash_type_doubleword:
//		for(uint32_t i = 0; i < length; i++)
//		{
//			((uint32_t*)data_read)[i] = *((uint32_t*)(address + 8*i));
//			address+=8;
//		}
//		break;			
	}
}


