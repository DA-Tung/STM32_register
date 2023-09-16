
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "i2c.h"
#include "systick.h"

void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_set_pos(int row, int col);
void lcd_send_string(char *string);
void lcd_init(void);



