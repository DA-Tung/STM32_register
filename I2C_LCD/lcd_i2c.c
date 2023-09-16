
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "i2c.h"
#include "systick.h"
#include "lcd_i2c.h"

#define lcd_clear_display          0x01           // Clear display
#define lcd_return_home            0x02           // Return home
#define lcd_entry_mode             0x06           // Auto Move direction cursor 
#define lcd_off_display            0x80           // Off display
#define lcd_on_display             0x0C           // On display and off cursor
#define lcd_on_cursor              0x0E           // On display and cursor
#define lcd_set_funcion            0x28           // Set 4bit, 2 line, font 5x8
#define lcd_line_1st               0x80           // Move cursor at line 1
#define lcd_line_2nd	           0xC0           // Move cursor at line 2
#define lcd_line_3rd               0x94           // Move cursor at line 3
#define lcd_line_4th	           0xD4           // Move cursor at line 4

#define lcd_rs                     0x01           // register selector
#define lcd_rw                     0x02           // Read/write
#define lcd_en                     0x04           // Enable read/write
#define lcd_light                  0x08           // Adjust led lcd (allway on)

#define lcd_addr_slave             0x4E           // Address slave module

// LCD send instruction register_______________________________________________________________
void lcd_send_cmd(uint8_t cmd)
{
	uint8_t send_cmd[4];
	
	// Send 4 bit - first
	send_cmd[0] = (cmd & 0xf0) | lcd_en | lcd_light;
	send_cmd[1] = (cmd & 0xf0) | lcd_light;

	// Send 4 bit - second	
	send_cmd[2] = ((cmd << 4) & 0xf0) | lcd_en | lcd_light;
	send_cmd[3] = ((cmd << 4) & 0xf0) | lcd_light;
	
	// send data
	i2c_master_transmit(I2C1, lcd_addr_slave, send_cmd, 4);
}

// LCD send data register_______________________________________________________________
void lcd_send_data(uint8_t data)
{
	uint8_t send_data[4];
	
	// Send 4 bit - first
	send_data[0] = (data & 0xf0) | lcd_rs | lcd_en | lcd_light;
	send_data[1] = (data & 0xf0) | lcd_rs | lcd_light;

	// Send 4 bit - second	
	send_data[2] = ((data << 4) & 0xf0) | lcd_rs | lcd_en | lcd_light;
	send_data[3] = ((data << 4) & 0xf0) | lcd_rs | lcd_light;
	
	// send data
	i2c_master_transmit(I2C1, lcd_addr_slave, send_data, 4);	
}

// LCD send string_______________________________________________________________
void lcd_send_string(char *string)
{
	while(*string) lcd_send_data(*string++);
}

// LCD send position_______________________________________________________________
void lcd_set_pos(int row, int col)
{	
	uint8_t pos_addr;
	
	if(row == 1)
	{
		// set cursor line 1
		pos_addr = 0x80 | col; 
	}
	else if(row == 2)
	{
		// set cursor line 2		
		pos_addr = 0xC0 | col;
	}
	
	else if(row == 3)
	{
		// set cursor line 2		
		pos_addr = 0x90 | col;
	}	
	
		else if(row == 4)
	{
		// set cursor line 2		
		pos_addr = 0xD0 | col;
	}
	
	lcd_send_cmd(pos_addr);
}


// LCD init_______________________________________________________________
void lcd_init(void)
{
	// Wait for mor than 40ms
	delay_systick_ms(50);
	
	// Funcion set (interface is 8 bit long)
	lcd_send_cmd(0x30);
	
	// Wait for mor than 4.1ms
	delay_systick_ms(5);
	
	// Funcion set (interface is 8 bit long)
	lcd_send_cmd(0x30);	
	
		// Wait for mor than 100us
	delay_systick_ms(1);
	
	// Funcion set (interface is 8 bit long)
	lcd_send_cmd(0x30);
	
	// Wait.....
	delay_systick_ms(50);
	
	// Set interface to be 4 bits long
	lcd_send_cmd(0x20);
	
	// Set 4 bit, 2 line, font 5x8
	lcd_send_cmd(lcd_set_funcion);
	
	// off display
	lcd_send_cmd(lcd_off_display);
	
	// Clear display
	lcd_send_cmd(lcd_clear_display);
	
	// Set entry mode
	lcd_send_cmd(lcd_entry_mode);
	
	// On display
	lcd_send_cmd(lcd_on_display);
	
	// Return home
	lcd_send_cmd(lcd_return_home);
	
	// Set at line 1
	lcd_send_cmd(lcd_line_1st);
}





