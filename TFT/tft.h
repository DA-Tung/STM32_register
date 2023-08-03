#ifndef _TFT_H_
#define _TFT_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Define PIN TFT
#define TFT_RST					 	10U    // PC10 - eset TFT
#define TFT_CS					 	9U     // PC9 - Chip select control
#define TFT_DC_X        		 	8U     // PC8 - Select command/data control
#define TFT_WR           		 	7U     // PC7 - Write data
#define TFT_RD           		 	6U     // PC6 - Read data

#define TFT_D0					 	8U     // PD8
#define TFT_D1					 	9U     // PD9
#define TFT_D2					 	10U    // PD10
#define TFT_D3					 	11U    // PD11
#define TFT_D4					 	12U    // PD12
#define TFT_D5					 	13U    // PD13
#define TFT_D6					 	14U    // PD14
#define TFT_D7					 	15U    // PD15

// Level 1 command
#define tft_no_operation         	0x00								// No operation
#define tft_reset                	0x01								// Reset software
#define tft_rd_id_inform         	0x04								// 
#define tft_rd_status            	0x09
#define tft_rd_power_mode			0x0A
#define tft_rd_pixel_format      	0x0C
#define tft_rd_image_format      	0x0D
#define tft_rd_signal_mode       	0x0E
#define tft_sleep mode           	0x10
#define tft_sleep_out            	0x11
#define tft_normal_mode_on       	0x13								// Return the display to normal mode
#define tft_display_invert_off	 	0x20								// Inversion display off
#define tft_display_invert_on	    0x21								// Inversion display on (white -> black)
#define tft_gamma_set            	0x26								// Gamma set
#define tft_display_off          	0x28								// Off display
#define tft_display_on           	0x29								// On display
#define tft_column_addr          	0x2A								// Set address columnn
#define tft_page_addr            	0x2B								// Set address row
#define tft_memory_wr            	0x2C								// Transfer data from MCU to frame memory
#define tft_color_set            	0x2D
#define tft_memory_rd            	0x2E
#define tft_ver_scrolling			0x33
#define tft_memory_access        	0x36
#define tft_vertical_start_addr  	0x37
#define tft_idle_mode_off        	0x38
#define tft_idle_mode_on         	0x39
#define tft_pixel_format_set		0x3A
#define tft_wr_memory_continue   	0x3C
#define tft_rd_memory_continue   	0x3E
#define tft_wr_display_BR        	0x51
#define tft_RD_display_BR        	0x52
#define tft_wr_ctrl              	0x53
#define tft_rd_ctrl              	0x54
#define tft_rd_ID1               	0xDA
#define tft_rd_ID2               	0xDB
#define tft_rd_ID3               	0xDC

// Level 2 command
#define tft_rd_ID4              	0xD3
#define tft_rgb_interface        	0xB0
#define tft_frame_rate_ctrl     	0xB1
#define tft_frame_ctrl_idle      	0xB2
#define tft_blank_porch_ctrl     	0xB5
#define tft_display_func         	0xB6
#define tft_entry_mode_set       	0xB7
#define tft_backlight_ctrl_1     	0xB8
#define tft_backlight_ctrl_2     	0xB9
#define tft_backlight_ctrl_3     	0xBA
#define tft_backlight_ctrl_4     	0xBB
#define tft_backlight_ctrl_5     	0xBC
//#define tft_backlight_ctrl_6      0xBD
#define tft_backlight_ctrl_7     	0xBE
#define tft_backlight_ctrl_8     	0xBF
#define tft_power_ctrl_1			0xC0
#define tft_power_ctrl_2			0xC1
#define tft_vcom_ctrl_1				0xC5
#define tft_vcom_ctrl_2				0xC7
#define tft_pos_gamma_correct    	0xE0
#define tft_neg_gamma_correct    	0xE1
#define tft_digi_gamma_ctrl_1    	0xE2
#define tft_digi_gamma_ctrl_2		0xE3
#define tft_interface_ctrl       	0xF6

// Extend register commands
#define LCD_POWERA               	0xCB   /* Power control A register */
#define LCD_POWERB               	0xCF   /* Power control B register */
#define LCD_DTCA                 	0xE8   /* Driver timing control A */
#define LCD_DTCB                 	0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            	0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN           	0xF2   /* 3 Gamma enable register */
#define LCD_PRC                  	0xF7   /* Pump ratio control register */

#define tft_row		  				0x00F0
#define tft_col		 				0x0140

#define Top_to_Bottom    			0U
#define Bottom_to_Top    			1U

#define Left_to_Right    			0U
#define Right_to_Left    			1U

#define Normal_mode      			0U
#define Reverse_mode     			1U

#define LCD_Res_TtB      			0U
#define LCD_Res_BtT      			1U

#define RGB_interface    			0U
#define BGR_interface    			1U

#define LCD_Res_LtR      			0U
#define LCD_Res_RtL		 			1U

// Color
#define white_color					0xFFFF
#define black_color					0x0000
#define red_color	       			0xF800
#define green_color  				0x07E0
#define blue_color	  				0x001F
#define yellow_color  				0xFFE0
#define orange_color  				0xFBE4
#define cyan_color			  		0x07FF
#define magenta_color  				0xA254
#define gray_color  				0x7BEF
#define brown_color 				0xBBCA

typedef struct
{
	uint16_t MADCTL_B7;
	uint16_t MADCTL_B6;
	uint16_t MADCTL_B5;
	uint16_t MADCTL_B4;
	uint16_t MADCTL_B3;
	uint16_t MADCTL_B2;	
}MADCTL_para;

// config PIN and data read/write
void tft_init(void);
void tft_pin_config(void);

void write_8b(uint8_t value);
void tft_write_data_8b(uint8_t data);
void tft_write_data_16b(uint16_t data);
void tft_write_cmd_8b(uint8_t cmd);

uint8_t read_8b(void);
uint8_t tft_read_data_8b(void);
uint16_t tft_read_data_16b(void);

void MADCTL_config(MADCTL_para MADCTL_data);
void tft_access_ctrl(int dir_dislay);
void tft_init(void);

void tft_set_coordinates(uint16_t S_col_data, uint16_t E_col_data, uint16_t S_row_data, uint16_t E_row_data);
void tft_write_pixel(uint16_t col, uint16_t row, uint16_t m_data);
void tft_fill_screen(uint16_t S_col,uint16_t E_col, uint16_t S_row, uint16_t E_row, uint16_t color);
uint16_t tft_read_data_pixel(uint16_t col, uint16_t row);
void tft_write_font(uint16_t font_col, uint16_t font_row, uint8_t font_ascii, uint16_t font_color);
void TFT_Reset(void);

#endif //_TFT_H_

	
	


