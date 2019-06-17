/**
  *********************************************************************************************************
  * @file    touchscreendemo.c
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

#include "touchscreendemo.h"
#include "touchscreenlib.h"
#include "touchscreen.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"
#include "framebuffer.h"
#include "geometry.h"
#include "font.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreenTest(void)
 @Description			S3C2440_TouchScreenTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TouchScreenTest(void)
{
	int touchScreenX, touchScreenY, touchScreenPressure;
	
	/* LCD��ʼ�� */
	LCD_Init();
	
	/* LCDʹ�� */
	LCD_enable();
	
	/* -LCD��ȡ����: fb_base, xres, yres, bpp- */
	LCD_Get_Params(&lcd_user_selected.fb_base, &lcd_user_selected.xres, &lcd_user_selected.yres, &lcd_user_selected.bpp);
	
	/* -��������ʼ��- */
	S3C2440_TouchScreen_Init();
	
	/* -����- */
	LCD_Geometry_ClearScr(0x000000);
	
	/* -��ʾУ׼- */
	LCD_Font_Print_string(70, 70, "Touch cross to calibrate touchscreen.", 0xFFFFFF);
	
	/* -У׼������- */
	TouchScreenLib_Calibrate();
	
	/* -��ʾ�滭- */
	LCD_Font_Print_string(70, lcd_user_selected.yres - 70, "OK! To draw!", 0xFFFFFF);
	
	while (true) {
		if (TouchScreenLib_ReadTsChangeLcd(&touchScreenX, &touchScreenY, &touchScreenPressure) != -1) {
			if (touchScreenPressure) {
				LCD_FrameBuffer_Pixel(touchScreenX-1, touchScreenY-1, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX-0, touchScreenY-1, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX+1, touchScreenY-1, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX-1, touchScreenY-0, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX-0, touchScreenY-0, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX+1, touchScreenY-0, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX-1, touchScreenY+1, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX-0, touchScreenY+1, 0x00FF00);
				LCD_FrameBuffer_Pixel(touchScreenX+1, touchScreenY+1, 0x00FF00);
			}
		}
	}
}

/********************************************** END OF FLEE **********************************************/
