/**
  *********************************************************************************************************
  * @file    lcddemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-05-08
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "lcddemo.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"
#include "geometry.h"
#include "font.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_LCDTest(void)
 @Description			S3C2440_LCDTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_LCDTest(void)
{
	unsigned char*  pbpp8;
	unsigned short* pbpp16;
	unsigned int*   pbpp32;
	
	/* LCD初始化 */
	LCD_Init();
	
	/* LCD使能 */
	LCD_enable();
	
	/* LCD获取参数: fb_base, xres, yres, bpp */
	LCD_Get_Params(&lcd_user_selected.fb_base, &lcd_user_selected.xres, &lcd_user_selected.yres, &lcd_user_selected.bpp);
	
	/* 向framebuffer中写数据 */
	switch (lcd_user_selected.bpp)
	{
	case 8: {
			/* Red */
			pbpp8 = (unsigned char*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp8++ = 12;
			
			delay_simulate(0x0003FFFF);
			
			/* Green */
			pbpp8 = (unsigned char*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp8++ = 47;
			
			delay_simulate(0x0003FFFF);
			
			/* Blue */
			pbpp8 = (unsigned char*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp8++ = 88;
			
			delay_simulate(0x0003FFFF);
			
			/* Black */
			pbpp8 = (unsigned char*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp8++ = 0;
			
			delay_simulate(0x0003FFFF);
		}
		break;
	
	case 16: {
			/* Red */
			pbpp16 = (unsigned short*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp16++ = 0xF800;
			
			delay_simulate(0x0003FFFF);
			
			/* Green */
			pbpp16 = (unsigned short*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp16++ = 0x07E0;
			
			delay_simulate(0x0003FFFF);
			
			/* Blue */
			pbpp16 = (unsigned short*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp16++ = 0x001F;
			
			delay_simulate(0x0003FFFF);
			
			/* Black */
			pbpp16 = (unsigned short*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp16++ = 0x0000;
		}
		break;
	
	case 32: {
			/* Red */
			pbpp32 = (unsigned int*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp32++ = 0xFF0000;
			
			delay_simulate(0x0003FFFF);
			
			/* Green */
			pbpp32 = (unsigned int*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp32++ = 0x00FF00;
			
			delay_simulate(0x0003FFFF);
			
			/* Blue */
			pbpp32 = (unsigned int*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp32++ = 0x0000FF;
			
			delay_simulate(0x0003FFFF);
			
			/* Black */
			pbpp32 = (unsigned int*)lcd_user_selected.fb_base;
			for (int x = 0; x < lcd_user_selected.xres; x++)
				for (int y = 0; y < lcd_user_selected.yres; y++)
					*pbpp32++ = 0x000000;
		}
		break;
	
	default:
		break;
	}
	
	delay_simulate(0x0003FFFF);
	
	/* 清屏 */
	LCD_Geometry_ClearScr(0xFF0000);
	delay_simulate(0x0001FFFF);
	LCD_Geometry_ClearScr(0x00FF00);
	delay_simulate(0x0001FFFF);
	LCD_Geometry_ClearScr(0x0000FF);
	delay_simulate(0x0001FFFF);
	LCD_Geometry_ClearScr(0x000000);
	delay_simulate(0x0003FFFF);
	
	/* 画线 */
	LCD_Geometry_Draw_Line(0, 0, lcd_user_selected.xres - 1, 0, 0x0023FF77);
	LCD_Geometry_Draw_Line(lcd_user_selected.xres - 1, 0, lcd_user_selected.xres - 1, lcd_user_selected.yres - 1, 0x0000FFFF);
	LCD_Geometry_Draw_Line(0, lcd_user_selected.yres - 1, lcd_user_selected.xres - 1, lcd_user_selected.yres - 1, 0x00FF00AA);
	LCD_Geometry_Draw_Line(0, 0, 0, lcd_user_selected.yres - 1, 0x00FF00EF);
	LCD_Geometry_Draw_Line(0, 0, lcd_user_selected.xres - 1, lcd_user_selected.yres - 1, 0x0000FF45);
	LCD_Geometry_Draw_Line(lcd_user_selected.xres - 1, 0, 0, lcd_user_selected.yres - 1, 0x00FF0780);
	
	delay_simulate(0x0003FFFF);
	
	/* 画圆 */
	LCD_Geometry_Draw_Circle(lcd_user_selected.xres / 2, lcd_user_selected.yres / 2, lcd_user_selected.yres / 4, 0x000000FF);
	
	delay_simulate(0x0003FFFF);
	
	/* 清屏 */
	LCD_Geometry_ClearScr(0x000000);
	
	/* 写字 */
	LCD_Font_Print_string(lcd_user_selected.xres / 3, lcd_user_selected.yres / 2, "LCD Test Demo!! So what?", 0xFF0000);
	
	delay_simulate(0x0003FFFF);
	
	/* 清屏 */
	LCD_Geometry_ClearScr(0x000000);
	
	/* 画十字 */
	LCD_Geometry_Draw_Cross(lcd_user_selected.xres / 4 * 1, lcd_user_selected.yres / 4 * 1, 10, 0xFF0000);
	LCD_Geometry_Draw_Cross(lcd_user_selected.xres / 4 * 3, lcd_user_selected.yres / 4 * 1, 10, 0xFF0000);
	LCD_Geometry_Draw_Cross(lcd_user_selected.xres / 4 * 1, lcd_user_selected.yres / 4 * 3, 10, 0xFF0000);
	LCD_Geometry_Draw_Cross(lcd_user_selected.xres / 4 * 3, lcd_user_selected.yres / 4 * 3, 10, 0xFF0000);
}

/********************************************** END OF FLEE **********************************************/
