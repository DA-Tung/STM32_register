#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "tft.h"
#include "gpio.h"
#include "systick.h"

// FONT 5x8
unsigned char Font_symbol[96][8] =
	{ 
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// space
		{0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x00},// !
		{0x0A,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00},// "
		{0x0A,0x0A,0x1F,0x0A,0x1F,0x0A,0x0A,0x00},// #
		{0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04,0x00},// $
		{0x18,0x19,0x02,0x04,0x08,0x13,0x03,0x00},// %
		{0x0C,0x12,0x14,0x08,0x14,0x12,0x0D,0x00},// &
		{0x0C,0x04,0x08,0x00,0x00,0x00,0x00,0x00},// '
		{0x02,0x04,0x08,0x08,0x08,0x04,0x02,0x00},// (
		{0x08,0x04,0x02,0x02,0x02,0x04,0x08,0x00},// )
		{0x00,0x04,0x15,0x0E,0x15,0x04,0x00,0x00},// *
		{0x00,0x04,0x04,0x1F,0x04,0x04,0x00,0x00},// +
		{0x00,0x00,0x00,0x00,0x0C,0x04,0x08,0x00},// ,
		{0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00},// -
		{0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00},// .
		{0x00,0x01,0x02,0x04,0x08,0x10,0x00,0x00},// /
		
		{0x0E,0x11,0x13,0x15,0x19,0x11,0x0E,0x00},// 0
		{0x04,0x0C,0x04,0x04,0x04,0x04,0x0E,0x00},// 1
		{0x0E,0x11,0x01,0x02,0x04,0x08,0x1F,0x00},// 2
		{0x1F,0x02,0x04,0x02,0x01,0x11,0x0E,0x00},// 3
		{0x02,0x06,0x0A,0x12,0x1F,0x02,0x02,0x00},// 4
		{0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E,0x00},// 5
		{0x06,0x08,0x10,0x1E,0x11,0x11,0x0E,0x00},// 6
		{0x1F,0x01,0x02,0x04,0x08,0x08,0x08,0x00},// 7
		{0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E,0x00},// 8
		{0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C,0x00},// 9
		
		{0x00,0x01,0x02,0x04,0x08,0x10,0x00,0x00},// :
		{0x00,0x0C,0x0C,0x00,0x0C,0x04,0x08,0x00},// ;
		{0x02,0x04,0x08,0x10,0x08,0x04,0x02,0x00},// <
		{0x00,0x00,0x1F,0x00,0x1F,0x00,0x00,0x00},// =
		{0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x00},// >
		{0x0E,0x11,0x01,0x02,0x04,0x00,0x04,0x00},// ?
		{0x0E,0x11,0x01,0x0D,0x15,0x15,0x0E,0x00},// @
		
		{0x0E,0x11,0x11,0x11,0x1F,0x11,0x11,0x00},// A
		{0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E,0x00},// B
		{0x0E,0x11,0x10,0x10,0x10,0x11,0x0E,0x00},// C
		{0x1C,0x12,0x11,0x11,0x11,0x12,0x1C,0x00},// D
		{0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F,0x00},// E
		{0x1F,0x10,0x10,0x1E,0x10,0x10,0x10,0x00},// F
		{0x0E,0x11,0x10,0x17,0x11,0x11,0x0E,0x00},// G
		{0x11,0x11,0x11,0x1F,0x11,0x11,0x11,0x00},// H
		{0x0E,0x04,0x04,0x04,0x04,0x04,0x0E,0x00},// I
		{0x07,0x02,0x02,0x02,0x02,0x12,0x0C,0x00},// J
		{0x11,0x12,0x14,0x18,0x14,0x12,0x11,0x00},// K
		{0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0x00},// L
		{0x11,0x1B,0x15,0x15,0x11,0x11,0x11,0x00},// M
		{0x11,0x11,0x19,0x15,0x13,0x11,0x11,0x00},// N
		{0x0E,0x11,0x11,0x11,0x11,0x11,0x0E,0x00},// O
		{0x1E,0x11,0x11,0x1E,0x10,0x10,0x10,0x00},// P
		{0x0E,0x11,0x11,0x11,0x15,0x12,0x0D,0x00},// Q
		{0x1E,0x11,0x11,0x1E,0x14,0x12,0x11,0x00},// R
		{0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E,0x00},// S
		{0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x00},// T
		{0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00},// U
		{0x11,0x11,0x11,0x11,0x11,0x0A,0x04,0x00},// V
		{0x11,0x11,0x11,0x11,0x15,0x15,0x0E,0x00},// W
		{0x11,0x11,0x0A,0x04,0x0A,0x11,0x11,0x00},// X
		{0x11,0x11,0x11,0x0A,0x04,0x04,0x04,0x00},// Y
		{0x1F,0x01,0x02,0x04,0x08,0x10,0x1F,0x00},// Z
		
		{0x0E,0x08,0x08,0x08,0x08,0x08,0x0E,0x00},// [
		{0x11,0x0A,0x1F,0x04,0x1F,0x04,0x04,0x00},//
		{0x0E,0x02,0x02,0x02,0x02,0x02,0x0E,0x00},// ]
		{0x04,0x0A,0x11,0x00,0x00,0x00,0x00,0x00},// ^
		{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00},// _
		{0x08,0x04,0x00,0x00,0x00,0x00,0x00,0x00},// '
		
		{0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F,0x00},// a
		{0x10,0x10,0x1E,0x11,0x11,0x11,0x1E,0x00},// b
		{0x00,0x00,0x0E,0x10,0x10,0x11,0x0E,0x00},// c
		{0x01,0x01,0x0D,0x13,0x11,0x11,0x0F,0x00},// d
		{0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E,0x00},// e
		{0x06,0x09,0x08,0x1C,0x08,0x08,0x08,0x00},// f
		{0x00,0x0F,0x11,0x11,0x0F,0x01,0x0E,0x00},// g
		{0x10,0x10,0x16,0x19,0x11,0x11,0x11,0x00},// h
		{0x04,0x00,0x0C,0x04,0x04,0x04,0x0E,0x00},// i
		{0x02,0x00,0x06,0x02,0x02,0x12,0x0C,0x00},// j
		{0x10,0x10,0x12,0x14,0x18,0x14,0x12,0x00},// k
		{0x18,0x08,0x08,0x08,0x08,0x08,0x1C,0x00},// l
		{0x00,0x00,0x1A,0x15,0x15,0x11,0x11,0x00},// m
		{0x00,0x00,0x16,0x19,0x11,0x11,0x11,0x00},// n
		{0x00,0x00,0x0E,0x11,0x11,0x11,0x0E,0x00},// o
		{0x00,0x00,0x1E,0x11,0x1E,0x10,0x10,0x00},// p
		{0x00,0x00,0x0F,0x11,0x0F,0x01,0x01,0x00},// q
		{0x00,0x00,0x16,0x19,0x10,0x10,0x10,0x00},// r
		{0x00,0x00,0x0E,0x10,0x0E,0x01,0x1E,0x00},// s
		{0x08,0x08,0x1C,0x08,0x08,0x09,0x06,0x00},// t
		{0x00,0x00,0x11,0x11,0x11,0x13,0x0D,0x00},// u
		{0x00,0x00,0x11,0x11,0x11,0x0A,0x04,0x00},// v
		{0x00,0x00,0x11,0x11,0x11,0x15,0x0A,0x00},// w
		{0x00,0x00,0x11,0x0A,0x04,0x0A,0x11,0x00},// x
		{0x00,0x00,0x11,0x11,0x0F,0x01,0x0E,0x00},// y
		{0x00,0x00,0x1F,0x02,0x04,0x08,0x1F,0x00},// z
		
		{0x17,0x15,0x15,0x15,0x17,0x00,0x00,0x00},// {
		{0x17,0x11,0x17,0x14,0x17,0x00,0x00,0x00},// |
		{0x17,0x14,0x17,0x11,0x17,0x00,0x00,0x00},// }
		{0x01,0x05,0x09,0x1F,0x08,0x04,0x00,0x00},// -
		{0x00,0x07,0x0E,0x1C,0x11,0x1E,0x00,0x00} //  
	}; 

//FUNCION****************************************************************

// Config RST = PC10
#define TFT_RST_LOW             	 gpio_output(GPIOC, TFT_RST, PIN_RESET)
#define TFT_RST_HIGH            	 gpio_output(GPIOC, TFT_RST, PIN_SET)
#define TFT_RST_ACTIVE          	 gpio_config(GPIOC, PORTC, TFT_RST, PIN_OUTPUT)

// Config CS = PC9
#define TFT_CS_LOW              	 gpio_output(GPIOC, TFT_CS, PIN_RESET)
#define TFT_CS_HIGH             	 gpio_output(GPIOC, TFT_CS, PIN_SET)
#define TFT_CS_ACTIVE           	 gpio_config(GPIOC, PORTC, TFT_CS, PIN_OUTPUT)

// Config RS = PC8
#define TFT_DC_X_LOW              	 gpio_output(GPIOC, TFT_DC_X, PIN_RESET)
#define TFT_DC_X_HIGH             	 gpio_output(GPIOC, TFT_DC_X, PIN_SET)
#define TFT_DC_X_ACTIVE           	 gpio_config(GPIOC, PORTC, TFT_DC_X, PIN_OUTPUT)
 
// Config WR = PC7
#define TFT_WR_LOW               	 gpio_output(GPIOC, TFT_WR, PIN_RESET)
#define TFT_WR_HIGH              	 gpio_output(GPIOC, TFT_WR, PIN_SET)
#define TFT_WR_ACTIVE           	 gpio_config(GPIOC, PORTC, TFT_WR, PIN_OUTPUT)

// Config RD = PC6
#define TFT_RD_LOW               	 gpio_output(GPIOC, TFT_RD, PIN_RESET)
#define TFT_RD_HIGH              	 gpio_output(GPIOC, TFT_RD, PIN_SET)
#define TFT_RD_ACTIVE				 gpio_config(GPIOC, PORTC, TFT_RD, PIN_OUTPUT)

// Config D0 ~ D7
uint32_t D0_7 = TFT_D0 | TFT_D1 | TFT_D2 | TFT_D3 | TFT_D4 | TFT_D5 | TFT_D6 | TFT_D7;

#define TFT_PIN_D0_7             	 gpio_config(GPIOD, PORTD, D0_7 , PIN_OUTPUT)
#define TFT_PIN_D0_7_CLEAR       	 gpio_output(GPIOD, D0_7 , PIN_RESET)

// Config PIN_______________________________________________________________	
void tft_pin_config(void)
{
	TFT_RST_ACTIVE; TFT_RST_LOW;
	
	TFT_CS_ACTIVE; TFT_CS_LOW;
	
	TFT_DC_X_ACTIVE; TFT_DC_X_LOW;
	
	TFT_WR_ACTIVE; TFT_WR_HIGH;
	
	TFT_RD_ACTIVE; TFT_RD_HIGH;
	
	TFT_PIN_D0_7; TFT_PIN_D0_7_CLEAR;
}

// Write 8 bit value_______________________________________________________________	
void tft_write_8b(uint8_t value)
{
	GPIOD->ODR =  0;
	
	GPIOD->ODR = ((uint16_t)value << 8) & 0xFF00;
}

// Write 8 bit data_______________________________________________________________	
void tft_write_data_8b(uint8_t data)
{
	TFT_CS_LOW;
	
	TFT_DC_X_HIGH;

	TFT_RD_HIGH;
	
	TFT_WR_LOW;
	
	tft_write_8b(data);
	
	TFT_WR_HIGH;
	
	TFT_CS_HIGH;
}

// Write 16 bit data_______________________________________________________________	
void tft_write_data_16b(uint16_t data)
{
	uint16_t data_h = data >> 8;
	uint16_t data_l = data & 0xFF;
	
	tft_write_8b(data_h);	
	tft_write_8b(data_l);		
}

// Write 8 bit cmd_______________________________________________________________	
void tft_write_cmd_8b(uint8_t cmd)
{
	TFT_CS_LOW;
	
	TFT_DC_X_LOW;
	
	TFT_RD_HIGH;
	
	TFT_WR_LOW;
	
	tft_write_8b(cmd);
	
	TFT_WR_HIGH;
	
	TFT_DC_X_HIGH;
	
	TFT_CS_HIGH;
}

// Read 8 bit data_______________________________________________________________	
uint8_t tft_read_8b(void)
{
	uint8_t data = 0;
	
	TFT_CS_LOW;
	
	TFT_DC_X_HIGH;
	
	TFT_WR_HIGH;
	
	TFT_RD_LOW;
	
	data = (GPIOD->IDR & 0xFF) >> 8;
	
	TFT_RD_HIGH;
	
	TFT_CS_HIGH;
	
	return data;
}

// Read 16 bit data_______________________________________________________________	
uint16_t tft_read_16b(void)
{
	uint16_t data = 0;
	
	uint8_t data_h;
	uint8_t data_l;
	
	data_h = tft_read_8b();
	data_l = tft_read_8b();
	
	data = (data_h << 8) | data_l;
	
	return data;
}

//FUNCION****************************************************************

void MADCTL_config(MADCTL_para MADCTL_data)
{
	uint16_t read_value = 0;
	
	// Top - Bottom
	if(MADCTL_data.MADCTL_B7 == Top_to_Bottom)
	{
		read_value |= 0 << 7;
	}
	else if(MADCTL_data.MADCTL_B7 == Bottom_to_Top)
	{
		read_value |= 1 << 7;
	}
		
	// Left - Right
	if(MADCTL_data.MADCTL_B6 == Left_to_Right)
	{
		read_value |= 0 << 6;
	}
	else if(MADCTL_data.MADCTL_B6 == Right_to_Left)
	{
		read_value |= 1 << 6;
	}	
	
	// Normal - Reverse
	if(MADCTL_data.MADCTL_B5 == Normal_mode)
	{
		read_value |= 0 << 5;
	}
	else if(MADCTL_data.MADCTL_B5 == Reverse_mode)
	{
		read_value |= 1 << 5;
	}
	
	// LCD Register Top - Bottom
	if(MADCTL_data.MADCTL_B4 == LCD_Res_TtB)
	{
		read_value |= 0 << 4;
	}
	else if(MADCTL_data.MADCTL_B4 == LCD_Res_BtT)
	{
		read_value |= 1 << 4;
	}	
	
	// Interface RGB
	if(MADCTL_data.MADCTL_B3 == RGB_interface)
	{
		read_value |= 0 << 3;
	}
	else if(MADCTL_data.MADCTL_B3 == BGR_interface)
	{
		read_value |= 1 << 3;
	}
	
	// LCD Register Left - Right
	if(MADCTL_data.MADCTL_B2 == LCD_Res_LtR)
	{
		read_value |= 0 << 2;
	}
	else if(MADCTL_data.MADCTL_B2 == LCD_Res_RtL)
	{
		read_value |= 1 << 2;
	}		
}

// Set coordinate pixel_______________________________________________________________	
void tft_set_coordinates(uint16_t S_col_data, uint16_t E_col_data, uint16_t S_row_data, uint16_t E_row_data)
{
	// Set Column  
	tft_write_cmd_8b(tft_column_addr);               // set cmd column address
	
	tft_write_data_8b(S_col_data >> 8);             // write data SC
	tft_write_data_8b(S_col_data & 0xFF);
	
	tft_write_data_8b(E_col_data >> 8);             // write data EC
	tft_write_data_8b(E_col_data & 0xFF);
	
	// Set Page
	tft_write_cmd_8b(tft_page_addr);                 // set cmd page address
	
	tft_write_data_8b(S_row_data >> 8);             // write data SP
	tft_write_data_8b(S_row_data & 0xFF);
	
	tft_write_data_8b(E_row_data >> 8);             // write data EP
	tft_write_data_8b(E_row_data & 0xFF);
}

// Write memory data 1 pixel_______________________________________________________________	
void tft_write_pixel(uint16_t col, uint16_t row, uint16_t m_data)
{
	// Set coordinate
	tft_set_coordinates(col, col, row, row);
	
	// Send cmd memory
	tft_write_cmd_8b(tft_memory_wr);
	
	// Write data
	tft_write_data_16b(m_data);
}

// Write memory data multi pixel_______________________________________________________________	
void tft_fill_screen(uint16_t S_col,uint16_t E_col, uint16_t S_row,uint16_t E_row, uint16_t color)
{
	unsigned long area;
	
	uint16_t delta_col, delta_row;
	
	delta_col = S_col - E_col + 1;
	delta_row = S_row - E_row + 1;
	
	area = delta_col *delta_row;
	
	// Set coordinate
	tft_set_coordinates(S_col, E_col, S_row, E_row);
	
	// Send cmd memory
	tft_write_cmd_8b(tft_memory_wr);
	
	for(int i = 0; i < area; i++)
	{
		// Write data
		tft_write_data_16b(color);		
	}
}

// Write font data_______________________________________________________________	
void tft_write_font(uint16_t font_col, uint16_t font_row, uint8_t font_ascii, uint16_t font_color)
{
	tft_fill_screen(0,0x00EF,0,0x13F,write_color);
	
	for(int row = 0; row < 8; row++)
	{
		for(int col = 0; col < 8; col++)
		{
			if((Font_symbol[font_ascii - 0x20][row] >> (7 - col)) & 0xFF)
			{
				tft_write_pixel(font_col + col, font_row + row, font_color);
			}
			else
			{
				tft_write_pixel(font_col + col, font_row + row, write_color);
			}
		}
	}
}

// Read memory data 1 pixel_______________________________________________________________	
uint16_t tft_read_data_pixel(uint16_t col, uint16_t row)
{
	uint16_t rd_data;
	
	tft_set_coordinates(col, col, row, row);
	
	tft_write_cmd_8b(tft_memory_rd);
	
	tft_read_16b();											// dummy data
	
	rd_data= tft_read_16b();						// Read data
	
	return rd_data;
}

// TFT config coordinate_______________________________________________________________	
void tft_access_ctrl(int dir_dislay)
{
	uint8_t M_data = 0;
	
	uint16_t value_h, value_v;
	
	switch(dir_dislay)
	{
		case 0 :	
			M_data |= (0 << 7) | (0 << 6) | (0 << 5);    // MY = 0, MX = 0, MV = 0
			break;
		
		case 1 :	
			M_data |= (1 << 7) | (0 << 6) | (0 << 5);    // MY = 1, MX = 0, MV = 0
			break;
		
		case 2 :	
			M_data |= (0 << 7) | (1 << 6) | (0 << 5);    // MY = 0, MX = 1, MV = 0
			break;
		
		case 3 :	
			M_data |= (1 << 7) | (1 << 6) | (0 << 5);    // MY = 1, MX = 1, MV = 0
			break;
		
		case 4 :	
			M_data |= (0 << 7) | (0 << 6) | (1 << 5);    // MY = 0, MX = 0, MV = 1
			break;
		
		case 5 :	
			M_data |= (1 << 7) | (0 << 6) | (1 << 5);    // MY = 1, MX = 0, MV = 1
			break;
		
		case 6 :	
			M_data |= (0 << 7) | (1 << 6) | (1 << 5);    // MY = 0, MX = 1, MV = 1
			break;
		
		case 7 :	
			M_data |= (1 << 7) | (1 << 6) | (1 << 5);    // MY = 1, MX = 1, MV = 1
			break;									
	}
	
	if(dir_dislay < 4)
	{
		value_h = 0x00EF;							// 239
		value_v = 0x013F;							// 319
	}
	else if(dir_dislay >= 4)
	{
		value_h = 0x013F;
		value_v = 0x00EF;
	}
	M_data |= 0x08;			// Set BRG = 1, MH = 0, ML = 0
	
	tft_write_cmd_8b(tft_memory_access);
	
	tft_write_data_8b(M_data & 0xFF);
	
	tft_set_coordinates(0,value_h,0,value_v);
}

// TFT init_______________________________________________________________	
//void tft_init(void)
//{
//	// Reset TFT
//	tft_write_cmd_8b(tft_reset);
//	delay_systick_ms(1000);
//	
//	// Config MADCTL register
////	MADCTL_para MADCTL_read_data;
////	
////	MADCTL_read_data.MADCTL_B7 = Top_to_Bottom;
////	MADCTL_read_data.MADCTL_B6 = Left_to_Right;
////	MADCTL_read_data.MADCTL_B5 = Normal_mode;
////	MADCTL_read_data.MADCTL_B4 = LCD_Res_TtB;
////	MADCTL_read_data.MADCTL_B3 = BGR_interface;
////	MADCTL_read_data.MADCTL_B2 = LCD_Res_LtR;	
////	
////	MADCTL_config(MADCTL_read_data);
//	
////	// TFT config vertical scolling
////	tft_write_cmd_8b(tft_ver_scrolling);	
////	
////	tft_write_data_16b(0);									// TFA = 2
////	tft_write_data_16b(0x0002);
////	
////	tft_write_data_16b(0x0001);							// VSA = 316
////	tft_write_data_16b(0x003C);
////	
////	tft_write_data_16b(0);									// BFA = 2
////	tft_write_data_16b(0x0002);
//	
//	// Power control 1
//	tft_write_cmd_8b(tft_power_ctrl_1);
//	tft_write_data_8b(0x21);
//	
//	// Power control 2
//	tft_write_cmd_8b(tft_power_ctrl_2);
//	tft_write_data_8b(0x11);
//	
//	// VCOM Control 1
//	tft_write_cmd_8b(tft_vcom_ctrl_1);
//	tft_write_data_8b(0x45);						      // VMH = 4.425V
//	tft_write_data_8b(0x45);									// VML = -0.775V
//	
//	// VCOM Control 2
//	tft_write_cmd_8b(tft_vcom_ctrl_2);
//	tft_write_data_8b(0x90);									// VMH - 48, VML - 48 
//	
//	// TFT memory access
//	tft_access_ctrl(1);
//	
//	// Pixel Format set
//	tft_write_cmd_8b(tft_pixel_format_set);
//	tft_write_data_8b(0x05);
//	
//	// Frame Rate Control
//	tft_write_cmd_8b(tft_frame_rate_ctrl);
//	tft_write_cmd_8b(0);                       // F_osc
//	tft_write_data_8b(0x18);									 // Frame Rate = 79 Hz
//	
//	// Display FUnction Control
//	tft_write_cmd_8b(tft_display_func);
//	tft_write_data_8b(0x0A);
//	tft_write_data_8b(0x82);
//	tft_write_data_8b(0x27);
//	
//	// Sleep out
//	tft_write_cmd_8b(tft_sleep_out);
//	
//	// Display on
//	tft_write_cmd_8b(tft_display_on);
//}

void tft_init(void)
{	
	tft_pin_config();
	
	  /* Configure LCD */
  tft_write_cmd_8b(0xCA);
  tft_write_data_8b(0xC3);
  tft_write_data_8b(0x08);
  tft_write_data_8b(0x50);
	
  tft_write_cmd_8b(LCD_POWERB);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0xC1);
  tft_write_data_8b(0x30);
	
  tft_write_cmd_8b(LCD_POWER_SEQ);
  tft_write_data_8b(0x64);
  tft_write_data_8b(0x03);
  tft_write_data_8b(0x12);
  tft_write_data_8b(0x81);
	
  tft_write_cmd_8b(LCD_DTCA);
  tft_write_data_8b(0x85);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x78);
	
  tft_write_cmd_8b(LCD_POWERA);
  tft_write_data_8b(0x39);
  tft_write_data_8b(0x2C);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x34);
  tft_write_data_8b(0x02);
	
  tft_write_cmd_8b(LCD_PRC);
  tft_write_data_8b(0x20);
	
  tft_write_cmd_8b(LCD_DTCB);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x00);
	
  tft_write_cmd_8b(tft_frame_rate_ctrl);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x1B);
	
  tft_write_cmd_8b(tft_display_func);
  tft_write_data_8b(0x0A);
  tft_write_data_8b(0xA2);
	
  tft_write_cmd_8b(tft_power_ctrl_1);
  tft_write_data_8b(0x10);
	
  tft_write_cmd_8b(tft_power_ctrl_2);
  tft_write_data_8b(0x10);
	
  tft_write_cmd_8b(tft_vcom_ctrl_1);
  tft_write_data_8b(0x45);
  tft_write_data_8b(0x15);
	
  tft_write_cmd_8b(tft_vcom_ctrl_2);
  tft_write_data_8b(0x90);
	
  tft_write_cmd_8b(tft_memory_access);
  tft_write_data_8b(0xC8);
	
  tft_write_cmd_8b(LCD_3GAMMA_EN);
  tft_write_data_8b(0x00);
	
  tft_write_cmd_8b(tft_rgb_interface);
  tft_write_data_8b(0xC2);
	
  tft_write_cmd_8b(tft_display_func);
  tft_write_data_8b(0x0A);
  tft_write_data_8b(0xA7);
  tft_write_data_8b(0x27);
  tft_write_data_8b(0x04);
  
  /* Colomn address set */
  tft_write_cmd_8b(tft_column_addr);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0xEF);
	
  /* Page address set */
  tft_write_cmd_8b(tft_page_addr);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x01);
  tft_write_data_8b(0x3F);
	
  tft_write_cmd_8b(tft_interface_ctrl);
  tft_write_data_8b(0x01);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x06);
  
  tft_write_cmd_8b(tft_memory_wr);
  delay_systick_ms(200);
  
  tft_write_cmd_8b(tft_gamma_set);
  tft_write_data_8b(0x01);
  
  tft_write_cmd_8b(tft_pos_gamma_correct);
  tft_write_data_8b(0x0F);
  tft_write_data_8b(0x29);
  tft_write_data_8b(0x24);
  tft_write_data_8b(0x0C);
  tft_write_data_8b(0x0E);
  tft_write_data_8b(0x09);
  tft_write_data_8b(0x4E);
  tft_write_data_8b(0x78);
  tft_write_data_8b(0x3C);
  tft_write_data_8b(0x09);
  tft_write_data_8b(0x13);
  tft_write_data_8b(0x05);
  tft_write_data_8b(0x17);
  tft_write_data_8b(0x11);
  tft_write_data_8b(0x00);
	
  tft_write_cmd_8b(tft_neg_gamma_correct);
  tft_write_data_8b(0x00);
  tft_write_data_8b(0x16);
  tft_write_data_8b(0x1B);
  tft_write_data_8b(0x04);
  tft_write_data_8b(0x11);
  tft_write_data_8b(0x07);
  tft_write_data_8b(0x31);
  tft_write_data_8b(0x33);
  tft_write_data_8b(0x42);
  tft_write_data_8b(0x05);
  tft_write_data_8b(0x0C);
  tft_write_data_8b(0x0A);
  tft_write_data_8b(0x28);
  tft_write_data_8b(0x2F);
  tft_write_data_8b(0x0F);
  
  tft_write_cmd_8b(tft_sleep_out);
  delay_systick_ms(200);
	
  tft_write_cmd_8b(tft_display_on);
	
  /* GRAM start writing */
//  tft_write_cmd_8b(tft_memory_wr);
}











