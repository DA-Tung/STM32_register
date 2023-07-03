
#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define flash_sector_0		0
#define flash_sector_1		1
#define flash_sector_2		2
#define flash_sector_3		3
#define flash_sector_4		4
#define flash_sector_5		5
#define flash_sector_6		6
#define flash_sector_7		7
#define flash_sector_8		8
#define flash_sector_9		9
#define flash_sector_10		10
#define flash_sector_11  	11

#define addr_flash_sector_0     ((uint32_t)0x08000000) /* Sector 0,  16 Kbytes */
#define addr_flash_sector_1     ((uint32_t)0x08004000) /* Sector 1,  16 Kbytes */
#define addr_flash_sector_2     ((uint32_t)0x08008000) /* Sector 2,  16 Kbytes */
#define addr_flash_sector_3     ((uint32_t)0x0800C000) /* Sector 3,  16 Kbytes */
#define addr_flash_sector_4     ((uint32_t)0x08010000) /* Sector 4,  64 Kbytes */
#define addr_flash_sector_5     ((uint32_t)0x08020000) /* Sector 5,  128 Kbytes */
#define addr_flash_sector_6     ((uint32_t)0x08040000) /* Sector 6,  128 Kbytes */
#define addr_flash_sector_7     ((uint32_t)0x08060000) /* Sector 7,  128 Kbytes */
#define addr_flash_sector_8     ((uint32_t)0x08080000) /* Sector 8,  129 Kbytes */
#define addr_flash_sector_9     ((uint32_t)0x080A0000) /* Sector 9,  128 Kbytes */
#define addr_flash_sector_10    ((uint32_t)0x080C0000) /* Sector 10, 128 Kbytes */
#define addr_flash_sector_11    ((uint32_t)0x080E0000) /* Sector 11, 128 Kbytes */

#define flash_type_byte 		         1
#define flash_type_haflword 		     2
#define flash_type_word		           3
#define flash_type_doubleword 		   4

void flash_lock(void);
void flash_unlock(void);
void flash_erase(uint32_t number_sector);
void flash_write_byte(uint32_t address_flash, uint8_t data_flash);
void flash_write_haflword(uint32_t address_flash, uint16_t data_flash);
void flash_write_word(uint32_t address_flash, uint32_t data_flash);
void flash_write_doubleword(uint32_t address_flash, uint64_t data_flash);
void flash_write_data(uint32_t address_flash, uint32_t index, void *data_write, uint32_t length, uint8_t flash_type);
void flash_read_data(uint32_t address_flash, uint32_t index, void *data_read, uint32_t length, uint8_t flash_type);





