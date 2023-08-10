#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "tft.h"
#include "gpio.h"
#include "systick.h"
#include <math.h>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* abs */
#include "front.h"

//DEFINE****************************************************************

// Config RST = PC10
#define TFT_RST_LOW             	 gpio_output(GPIOC, TFT_RST, PIN_RESET)
#define TFT_RST_HIGH            	 gpio_output(GPIOC, TFT_RST, PIN_SET)
#define TFT_RST_ACTIVE          	 gpio_config(GPIOC, PORTC, TFT_RST, PIN_OUTPUT)

// Config CS = PC9
#define TFT_CS_LOW              	 gpio_output(GPIOC, TFT_CS, PIN_RESET)
#define TFT_CS_HIGH             	 gpio_output(GPIOC, TFT_CS, PIN_SET)
#define TFT_CS_ACTIVE           	 gpio_config(GPIOC, PORTC, TFT_CS, PIN_OUTPUT)

// Config DC_X = PC8
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
//uint32_t D0_7 = TFT_D0 | TFT_D1 | TFT_D2 | TFT_D3 | TFT_D4 | TFT_D5 | TFT_D6 | TFT_D7;
//#define TFT_PIN_D_CLEAR       		 gpio_output(GPIOD, D0_7, PIN_RESET)

//FUNCION****************************************************************

// Config PIN Input_______________________________________________________________	
void SetReadPin(void)
{
	gpio_config(GPIOD, PORTD, TFT_D0 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D1 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D2 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D3 , PIN_INPUT);	
	gpio_config(GPIOD, PORTD, TFT_D4 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D5 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D6 , PIN_INPUT);
	gpio_config(GPIOD, PORTD, TFT_D7 , PIN_INPUT);	
}

// Config PIN Output_______________________________________________________________
void SetWritePin(void)
{
	gpio_config(GPIOD, PORTD, TFT_D0 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D1 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D2 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D3 , PIN_OUTPUT);	
	gpio_config(GPIOD, PORTD, TFT_D4 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D5 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D6 , PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, TFT_D7 , PIN_OUTPUT);	
	
	GPIOD->ODR = 0;
}

// Config PIN_______________________________________________________________
void tft_pin_config(void)
{
	TFT_RST_ACTIVE; TFT_RST_HIGH;
	
	TFT_CS_ACTIVE; TFT_CS_HIGH;
	
	TFT_DC_X_ACTIVE; TFT_DC_X_HIGH;
	
	TFT_WR_ACTIVE; TFT_WR_HIGH;
	
	TFT_RD_ACTIVE; TFT_RD_HIGH;
	
	SetWritePin(); 
}

// Write 8 bit value_______________________________________________________________	
void write_8b(uint8_t value)
{
	GPIOD->ODR = (uint32_t)(value & 0xFF);
}

// Read 8 bit value_______________________________________________________________	
uint8_t read_8b(void)
{
	uint8_t value;
	
	value = (uint8_t)(GPIOD->IDR & 0x00FF);
	
	return value;
}

// Write 8 bit data_______________________________________________________________	
void tft_write_data_8b(uint8_t data)
{
	TFT_CS_LOW;
	
	TFT_RST_HIGH;
	
	TFT_DC_X_HIGH;

	TFT_RD_HIGH;
	  
	TFT_WR_LOW;
	
	write_8b(data);
	
	TFT_WR_HIGH;
	
	TFT_CS_HIGH;
}

// Write 16 bit data_______________________________________________________________	
void tft_write_data_16b(uint16_t data)
{
	uint8_t data_h = (data >> 8) & 0xFF;
	tft_write_data_8b(data_h);		
	
	uint8_t data_l = data & 0xFF;
	tft_write_data_8b(data_l);		
}

// Write 8 bit cmd_______________________________________________________________	
void tft_write_cmd_8b(uint8_t cmd)
{
	TFT_CS_LOW;
	
	TFT_RST_HIGH;
	
	TFT_RD_HIGH;
	
	TFT_DC_X_LOW;
	
	TFT_WR_LOW;
	
	write_8b(cmd);
	
	TFT_WR_HIGH;
	
	TFT_DC_X_HIGH;
	
	TFT_CS_HIGH;
}

// Read 8 bit data_______________________________________________________________	
uint8_t tft_read_data_8b(void)
{
	uint8_t data = 0;
	
	SetReadPin();
	
	TFT_CS_LOW;
	
	TFT_DC_X_HIGH;
	
	TFT_WR_HIGH;
	
	TFT_RD_LOW;
	
	data = read_8b();
	
	TFT_RD_HIGH;
	
	TFT_CS_HIGH;
	
	return data;
}

// Read 16 bit data_______________________________________________________________	
uint16_t tft_read_data_16b(void)
{
	uint16_t data = 0;
	
	uint8_t data_h;
	uint8_t data_l;
	
	data_h = tft_read_data_8b();
	data_l = tft_read_data_8b();
	
	data = (data_h << 8) | data_l;
	
	return data;
}

//FUNCION****************************************************************

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
	
	// Set data
	M_data |= 0x08;			// Set BRG = 1, MH = 0, ML = 0
	
	// Write command memory access
	tft_write_cmd_8b(tft_memory_access);
	
	// Write data
	tft_write_data_8b(M_data & 0xFF);
}

// TFT MADCTL config_______________________________________________________________	
void MADCTL_config(MADCTL_para MADCTL_data)
{
	uint8_t read_value = 0;
	
	read_value |= (MADCTL_data.MADCTL_B2 << 2)
				| (MADCTL_data.MADCTL_B3 << 3)
				| (MADCTL_data.MADCTL_B4 << 4)
				| (MADCTL_data.MADCTL_B5 << 5) 
				| (MADCTL_data.MADCTL_B6 << 6)
				| (MADCTL_data.MADCTL_B7 << 7);
	
	// tft send command madctl display
	tft_write_cmd_8b(tft_madctl_display);
	// Write data
	tft_write_data_8b(read_value&0xFF);
	
}

// TFT reset soft_______________________________________________________________	
void tft_reset(void)
{
	TFT_RST_LOW;
	delay_systick_ms(5);
	TFT_RST_HIGH;
}

// TFT init_______________________________________________________________	
void tft_init(void)
{		
	tft_reset();
	
	// TFT Reset hardware
	tft_write_cmd_8b(tft_rst);
	delay_systick_ms(1000);
		
	/* Configure LCD */
	
	// Power control A
	tft_write_cmd_8b(LCD_POWERA);
	tft_write_data_8b(0x39);
	tft_write_data_8b(0x2C);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0x34);
	tft_write_data_8b(0x02);
	
	// Power control B
	tft_write_cmd_8b(LCD_POWERB);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0xC1);
	tft_write_data_8b(0x30);
	
	// Driver timing control A
	tft_write_cmd_8b(LCD_DTCA);
	tft_write_data_8b(0x85);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0x78);
	
	// Driver timing control B
	tft_write_cmd_8b(LCD_DTCB);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0x00);
	
	// Power on sequence control
	tft_write_cmd_8b(LCD_POWER_SEQ);
	tft_write_data_8b(0x64);
	tft_write_data_8b(0x03);
	tft_write_data_8b(0x12);
	tft_write_data_8b(0x81);
	
	// Pump ratio control
	tft_write_cmd_8b(LCD_PRC);
	tft_write_data_8b(0x20);
	
	// Power control 1
	tft_write_cmd_8b(tft_power_ctrl_1);
	tft_write_data_8b(0x23);
	
	// Power control 2
	tft_write_cmd_8b(tft_power_ctrl_2);
	tft_write_data_8b(0x10);
	
	// Vcom control 1
	tft_write_cmd_8b(tft_vcom_ctrl_1);
	tft_write_data_8b(0x3E);
	tft_write_data_8b(0x28);

	// Vcom control 2
	tft_write_cmd_8b(tft_vcom_ctrl_2);
	tft_write_data_8b(0x86);	
	
	// tft memory access
	tft_write_cmd_8b(tft_memory_access);
	tft_write_data_8b(0x48);
	
	// Pixel format set
	tft_write_cmd_8b(tft_pixel_format_set);
	tft_write_data_8b(0x55); 
	
	// Frame Rate Control
	tft_write_cmd_8b(tft_frame_rate_ctrl);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0x18);
	
	// Display funcion control
	tft_write_cmd_8b(tft_display_func);
	tft_write_data_8b(0x08);
	tft_write_data_8b(0x82);
	tft_write_data_8b(0x27);
	
	// 3Gamma Funcion disable
	tft_write_cmd_8b(LCD_3GAMMA_EN);
	tft_write_data_8b(0x00);
	
	// Gamma curve selected
	tft_write_cmd_8b(tft_gamma_set);
	tft_write_data_8b(0x01);	

	//Positive Gamma Correction
	tft_write_cmd_8b(tft_pos_gamma_correct);
	tft_write_data_8b(0x0F);
	tft_write_data_8b(0x31);
	tft_write_data_8b(0x2B);
	tft_write_data_8b(0x0C);
	tft_write_data_8b(0x0E);
	tft_write_data_8b(0x08);
	tft_write_data_8b(0x4E);
	tft_write_data_8b(0xF1);
	tft_write_data_8b(0x37);
	tft_write_data_8b(0x07);
	tft_write_data_8b(0x10);
	tft_write_data_8b(0x03);
	tft_write_data_8b(0x0E);
	tft_write_data_8b(0x09);
	tft_write_data_8b(0x00);

	//Negative Gamma  Correction
	tft_write_cmd_8b(tft_neg_gamma_correct);
	tft_write_data_8b(0x00);
	tft_write_data_8b(0x0E);
	tft_write_data_8b(0x14);
	tft_write_data_8b(0x03);
	tft_write_data_8b(0x11);
	tft_write_data_8b(0x07);
	tft_write_data_8b(0x31);
	tft_write_data_8b(0xC1);
	tft_write_data_8b(0x48);
	tft_write_data_8b(0x08);
	tft_write_data_8b(0x0F);
	tft_write_data_8b(0x0C);
	tft_write_data_8b(0x31);
	tft_write_data_8b(0x36);
	tft_write_data_8b(0x0F);

	// Sleep out
	tft_write_cmd_8b(tft_sleep_out);
	delay_systick_ms(1000);
	
	// Turn on Display 
	tft_write_cmd_8b(tft_display_on);
	delay_systick_ms(1000);
			
	// access control
	tft_access_ctrl(0);		
	
	// config MADCTL
	MADCTL_para MADCTL_inform;
	MADCTL_inform.MADCTL_B2 = LCD_Res_LtR;	
	MADCTL_inform.MADCTL_B3 = RGB_interface;
	MADCTL_inform.MADCTL_B4 = LCD_Res_TtB;
	MADCTL_inform.MADCTL_B5 = Normal_mode;
	MADCTL_inform.MADCTL_B6 = Left_to_Right;
	MADCTL_inform.MADCTL_B7 = Top_to_Bottom;
	MADCTL_config(MADCTL_inform);	
}

// Set coordinate pixel_______________________________________________________________	
void tft_set_coordinates(uint16_t S_col_data, uint16_t E_col_data, uint16_t S_row_data, uint16_t E_row_data)
{
	// Set Column address
	tft_write_cmd_8b(tft_column_addr);               					
	
	// write data SC	
	tft_write_data_16b(S_col_data);

	// write data EC	
	tft_write_data_16b(E_col_data);	
	
	// Set Page adrress
	tft_write_cmd_8b(tft_page_addr);                					
	
	// write data SP	
	tft_write_data_16b(S_row_data);
	
	// write data EP	
	tft_write_data_16b(E_row_data);

	// Write to RAM
	tft_write_cmd_8b(tft_memory_wr);                 	
}

// Write memory data 1 pixel_______________________________________________________________	
void tft_write_pixel(uint16_t col, uint16_t row, uint16_t m_data)
{
	// Check col and row
	if((col >= tft_col) || (row >= tft_row))
		return;
	
	// Set coordinate
	tft_set_coordinates(col, col+1, row, row+1);
	
	// Write data
	tft_write_data_16b(m_data);	 	
}

// TFT fill area_______________________________________________________________	
void tft_fill_area(uint16_t S_col,uint16_t E_col, uint16_t S_row, uint16_t E_row, uint16_t color)
{	
	// Check col and row
	if((E_col >= tft_col) || (E_row >= tft_row))
		return;	
	
	// Set coordinate
	tft_set_coordinates(S_col, E_col, S_row, E_row);	
	
	// Write data for each pixel
	for(uint16_t i_col = S_col; i_col <= E_col ; i_col++)
	{
		for(uint16_t i_row = S_row; i_row <= E_row ; i_row++)		
		{
			// Write data
			tft_write_pixel(i_col,i_row,color);	
		}	
	} 			
}

// TFT Drawing Line_______________________________________________________________	
void tft_drawing_Line(uint16_t X_Coor, uint16_t Y_Coor, uint16_t witdh, uint16_t height,uint16_t Color)
{
	tft_fill_area(Y_Coor,Y_Coor + witdh,X_Coor,X_Coor+height, Color);
}

// TFT fill screen_______________________________________________________________
void tft_fill_screen(uint16_t color)
{
	tft_fill_area(0,tft_col-1,0,tft_row-1,color);
	
}

// Write font data_______________________________________________________________	
void tft_write_font(uint16_t X_Coor, uint16_t Y_Coor, char ch, FontDef Font_Type, uint16_t font_color)
{	
	// Check data
	if(X_Coor + Font_Type.height >= tft_row || Y_Coor + Font_Type.width > tft_col)
		return;
	
	uint16_t ch_pos;
	// Set row of character
	for(int row = 0; row < Font_Type.height; row++)
	{
		// Find position of each element in a character
		ch_pos = Font_Type.data[(ch-32)*Font_Type.height + row];
		// set col of character
		for(int col = 0; col < Font_Type.width; col++)
		{
			// Check bit set or not
			if((ch_pos << col)&0x8000)
			{
				tft_write_pixel(X_Coor+row,Y_Coor+col,font_color);
			}
		}
	}
}

// Tft write string_______________________________________________________________	
void tft_write_string(uint16_t X_Coor, uint16_t Y_Coor, char *str, FontDef Font_Type, uint16_t font_color, Mode_font mode)
{
	while(*str)
	{
		// Check data		
		if(X_Coor + Font_Type.height >= tft_row || Y_Coor + Font_Type.width > tft_col)
			return;		
		
		if(*str == ' ')
		{
			// next character
			str++;
			// Increase position of X_coor or Y_coor
			if(mode == mode_h) X_Coor +=Font_Type.height;
			else if(mode == mode_v) Y_Coor += Font_Type.width;
			
			continue;
		}
		
		// Check mode
		if(mode == mode_h)
		{
			// Write each character in string
			tft_write_font(X_Coor,Y_Coor,*str, Font_Type,font_color);
			// Increase position by horizontal
			X_Coor += Font_Type.height;
			// Increase position pointer of string
			str++;
		}
		else if(mode == mode_v)
		{
			// Write each character in string			
			tft_write_font(X_Coor,Y_Coor,*str, Font_Type,font_color);
			// Increase position by horizontal
			Y_Coor += Font_Type.width;
			// Increase position pointer of string
			str++;			
		}
	}
}

// Tft drawing circle_______________________________________________________________	
void tft_drawing_circle(uint16_t X_Coor, uint16_t Y_Coor, uint16_t Radius, uint16_t Color)
{
	int X = Radius;
	int Y = 0;
	int dX = 1;
	int dY = 1;
	int err = dX - (Radius*2);
	
	while(X >= Y)
	{
		// Drawing pixel for circle
		tft_write_pixel(X_Coor + X, Y_Coor + Y, Color);
		tft_write_pixel(X_Coor + Y, Y_Coor + X, Color);	
		tft_write_pixel(X_Coor - Y, Y_Coor + X, Color);
		tft_write_pixel(X_Coor - X, Y_Coor + Y, Color);	
		tft_write_pixel(X_Coor - X, Y_Coor - Y, Color);
		tft_write_pixel(X_Coor - Y, Y_Coor - X, Color);	
		tft_write_pixel(X_Coor + Y, Y_Coor - X, Color);
		tft_write_pixel(X_Coor + X, Y_Coor - Y, Color);		
		if(err <= 0)
		{
			Y++;
			err += dY;
			dY += 2;
		}
		else if(err > 0)
		{
			X--;
			dX += 2;
			err += -(Radius*2) + dX;
		}
	}
}

// Tft fill circle_______________________________________________________________	
void tft_fill_circle(uint16_t X_Coor, uint16_t Y_Coor, uint16_t Radius, uint16_t Color)
{
	int X = Radius;
	int Y = 0;
	int dX = 1;
	int dY = 1;
	int err = dX - (Radius*2);
	
	while(X >= Y)
	{	
		// Drawing line for circle
		tft_drawing_Line(X_Coor - X, Y_Coor + Y, 1, 2*X, Color);
		tft_drawing_Line(X_Coor - Y, Y_Coor + X, 1, 2*Y, Color);
		tft_drawing_Line(X_Coor - X, Y_Coor - Y, 1, 2*X, Color);
		tft_drawing_Line(X_Coor - Y, Y_Coor - X, 1, 2*Y, Color);
		if(err <= 0)
		{
			Y++;
			err += dY;
			dY += 2;
		}
		else if(err > 0)
		{
			X--;
			dX += 2;
			err += -(Radius*2) + dX;
		}
	}
}

// Read memory data 1 pixel_______________________________________________________________	
uint16_t tft_read_data_pixel(uint16_t col, uint16_t row)
{
	uint16_t rd_data;
	
	tft_set_coordinates(col, col+1, row, row+1);
	
	tft_write_cmd_8b(tft_memory_rd);
	
	tft_read_data_16b();								// dummy data
	
	rd_data= tft_read_data_16b();						// Read data
	
	return rd_data;
}











