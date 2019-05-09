/**
  *********************************************************************************************************
  * @file    geometry.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-05-09
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "geometry.h"
#include "framebuffer.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"

/**********************************************************************************************************
 @Function			void LCD_Geometry_Draw_Circle(int x, int y, int r, int color)
 @Description			LCD_Geometry_Draw_Circle	: 画圆
 @Input				x					: 坐标x
					y					: 坐标y
					r					: 半径
					color				: 颜色 32bit 0x00RRGGBB
 @Return				void
**********************************************************************************************************/
void LCD_Geometry_Draw_Circle(int x, int y, int r, int color)
{
	int a, b, num;
	a = 0;
	b = r;
	
	while (22 * b * b >= r * r) {
		LCD_FrameBuffer_Pixel(x + a, y - b, color);
		LCD_FrameBuffer_Pixel(x - a, y - b, color);
		LCD_FrameBuffer_Pixel(x - a, y + b, color);
		LCD_FrameBuffer_Pixel(x + a, y + b, color);
		
		LCD_FrameBuffer_Pixel(x + b, y + a, color);
		LCD_FrameBuffer_Pixel(x + b, y - a, color);
		LCD_FrameBuffer_Pixel(x - b, y - a, color);
		LCD_FrameBuffer_Pixel(x - b, y + a, color);
		
		a++;
		num = (a * a + b * b) - r * r;
		if (num > 0) {
			b--;
			a--;
		}
	}
}

/**********************************************************************************************************
 @Function			void LCD_Geometry_Draw_Line(int x1, int y1, int x2, int y2, int color)
 @Description			LCD_Geometry_Draw_Line	: 画线
 @Input				x1					: 坐标x1
					y1					: 坐标y1
					x2					: 坐标x2
					y2					: 坐标y2
					color				: 颜色 32bit 0x00RRGGBB
 @Return				void
**********************************************************************************************************/
void LCD_Geometry_Draw_Line(int x1, int y1, int x2, int y2, int color)
{
	int dx, dy, e;
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	if (dx >= 0) {
		if (dy >= 0) {
			if (dx >= dy) {
				e = dy - dx / 2;
				while (x1 <= x2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {y1 += 1; e -= dx;}
					x1 += 1;
					e += dy;
				}
			}
			else {
				e = dx - dy / 2;
				while (y1 <= y2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {x1 += 1; e -= dy;}
					y1 += 1;
					e += dx;
				}
			}
		}
		else {
			dy = -dy;
			if (dx >= dy) {
				e = dy - dx / 2;
				while (x1 <= x2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {y1 -= 1; e -= dx;}
					x1 += 1;
					e += dy;
				}
			}
			else {
				e = dx - dy / 2;
				while (y1 >= y2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {x1 += 1; e -= dy;}
					y1 -= 1;
					e += dx;
				}
			}
		}
	}
	else {
		dx = -dx;
		if (dy >= 0) {
			if (dx >= dy) {
				e = dy - dx / 2;
				while (x1 >= x2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {y1 += 1; e -= dx;}
					x1 -= 1;
					e += dy;
				}
			}
			else {
				e = dx - dy / 2;
				while (y1 <= y2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0 ){x1 -= 1; e -= dy;}
					y1 += 1;
					e += dx;
				}
			}
		}
		else {
			dy = -dy;
			if (dx >= dy) {
				e = dy - dx / 2;
				while (x1 >= x2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0) {y1 -= 1; e -= dx;}
					x1 -= 1;
					e += dy;
				}
			}
			else {
				e = dx - dy / 2;
				while (y1 >= y2) {
					LCD_FrameBuffer_Pixel(x1, y1, color);
					if (e > 0 ){x1 -= 1; e -= dy;}
					y1 -= 1;
					e += dx;
				}
			}
		}
	}
}

/********************************************** END OF FLEE **********************************************/
