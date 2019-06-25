#ifndef __FONT_H
#define   __FONT_H

#include "s3c2440_soc.h"

void LCD_Font_Print_char(int x, int y, char c, unsigned int color);				//输出字符
void LCD_Font_Print_string(int x, int y, char* str, unsigned int color);			//输出字符串

#endif /* __FONT_H */
