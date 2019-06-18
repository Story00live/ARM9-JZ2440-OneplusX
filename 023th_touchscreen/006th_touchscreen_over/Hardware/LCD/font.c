/**
  *********************************************************************************************************
  * @file    font.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-05-10
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "font.h"
#include "framebuffer.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"

extern const unsigned char fontdata_8x16[];

/**********************************************************************************************************
 @Function			void LCD_Font_Print_char(int x, int y, char c, unsigned int color)
 @Description			LCD_Font_Print_char		: 输出字符
 @Input				x					: 坐标x
					y					: 坐标y
					c					: 字符
					color				: 颜色 32bit 0x00RRGGBB
 @Return				void
**********************************************************************************************************/
void LCD_Font_Print_char(int x, int y, char c, unsigned int color)
{
	unsigned char *dots = (unsigned char *)&fontdata_8x16[c * 16];
	unsigned char data;
	int bit;
	
	/* 根据点阵来设置对应象素的颜色 */
	for (int j = y; j < y + 16; j++) {
		data = *dots++;
		bit = 7;
		for (int i = x; i < x + 8; i++) {
			if (data & (1 << bit)) LCD_FrameBuffer_Pixel(i, j, color);
			bit--;
		}
	}
}

/**********************************************************************************************************
 @Function			void LCD_Font_Print_string(int x, int y, char* str, unsigned int color)
 @Description			LCD_Font_Print_string	: 输出字符串
 @Input				x					: 坐标x
					y					: 坐标y
					c					: 字符串
					color				: 颜色 32bit 0x00RRGGBB
 @Return				void
**********************************************************************************************************/
void LCD_Font_Print_string(int x, int y, char* str, unsigned int color)
{
	int i = 0;
	
	while (str[i]) {
		if (str[i] == '\n') {
			y = y + 16;
		}
		else if (str[i] == '\r') {
			x = 0;
		}
		else {
			LCD_Font_Print_char(x, y, str[i], color);
			x = x + 8;
			if (x >= lcd_user_selected.xres) {
				x = 0;
				y = y + 16;
			}
		}
		i++;
	}
}

/********************************************** END OF FLEE **********************************************/
