/**
  *********************************************************************************************************
  * @file    framebuffer.c
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

#include "framebuffer.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"

static unsigned short conver32bppto16bpp(unsigned int rgb)
{
	/* rgb888 */
	int r = (rgb >> 16) & 0xFF;
	int g = (rgb >>  8) & 0xFF;
	int b = (rgb >>  0) & 0xFF;
	
	/* rgb565 */
	r = r >> 3;
	g = g >> 2;
	b = b >> 3;
	
	return ((r << 11) | (g << 5) | (b << 0));
}

/**********************************************************************************************************
 @Function			void LCD_FrameBuffer_Pixel(int x, int y, unsigned int color)
 @Description			LCD_FrameBuffer_Pixel	: 画点
 @Input				x					: 坐标x
					y					: 坐标y
					color				: 颜色 32bit 0x00RRGGBB
 @Return				void
**********************************************************************************************************/
void LCD_FrameBuffer_Pixel(int x, int y, unsigned int color)
{
	unsigned char*  pbpp8;
	unsigned short* pbpp16;
	unsigned int*   pbpp32;
	
	unsigned int    pixel_base = lcd_user_selected.fb_base + (lcd_user_selected.xres * lcd_user_selected.bpp / 8) * y + x * lcd_user_selected.bpp / 8;
	
	switch (lcd_user_selected.bpp)
	{
	case 8: {
			pbpp8   = (unsigned char*)pixel_base;
			*pbpp8  = color;
		}
		break;
	
	case 16: {
			pbpp16  = (unsigned short*)pixel_base;
			*pbpp16 = conver32bppto16bpp(color);
		}
		break;
	
	case 32: {
			pbpp32  = (unsigned int*)pixel_base;
			*pbpp32 = color;
		}
		break;
	
	default:
		break;
	}
}

/********************************************** END OF FLEE **********************************************/
