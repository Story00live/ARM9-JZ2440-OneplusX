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
	unsigned int fb_base;
	int xres, yres, bpp;
	
	unsigned char*  pbpp8;
	unsigned short* pbpp16;
	unsigned int*   pbpp32;
	
	/* LCD初始化 */
	LCD_Init();
	
	/* LCD使能 */
	LCD_enable();
	
	/* LCD获取参数: fb_base, xres, yres, bpp */
	LCD_Get_Params(&fb_base, &xres, &yres, &bpp);
	
	/* 向framebuffer中写数据 */
	switch (bpp)
	{
	case 8: {
			
		}
		break;
	
	case 16: {
			/* Red */
			pbpp16 = (unsigned short*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp16++ = 0xF800;
			
			delay_simulate(0x0003FFFF);
			
			/* Green */
			pbpp16 = (unsigned short*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp16++ = 0x07E0;
			
			delay_simulate(0x0003FFFF);
			
			/* Blue */
			pbpp16 = (unsigned short*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp16++ = 0x001F;
			
			delay_simulate(0x0003FFFF);
			
			/* Black */
			pbpp16 = (unsigned short*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp16++ = 0x0000;
		}
		break;
	
	case 32: {
			/* Red */
			pbpp32 = (unsigned int*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp32++ = 0xFF0000;
			
			delay_simulate(0x0003FFFF);
			
			/* Green */
			pbpp32 = (unsigned int*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp32++ = 0x00FF00;
			
			delay_simulate(0x0003FFFF);
			
			/* Blue */
			pbpp32 = (unsigned int*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp32++ = 0x0000FF;
			
			delay_simulate(0x0003FFFF);
			
			/* Black */
			pbpp32 = (unsigned int*)fb_base;
			for (int x = 0; x < xres; x++)
				for (int y = 0; y < yres; y++)
					*pbpp32++ = 0x000000;
		}
		break;
	
	default:
		break;
	}
	
	
	
	
	
	
	
	
	
	
}

/********************************************** END OF FLEE **********************************************/
