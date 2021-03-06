/**
  *********************************************************************************************************
  * @file    touchscreenlib.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-16
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					----------------------------
  *					|                          |
  *					|  +(A)              (B)+  |
  *					|                          |
  *					|                          |
  *					|                          |
  *					|            +(E)          |
  *					|                          |
  *					|                          |
  *					|                          |
  *					|  +(D)              (C)+  |
  *					|                          |
  *					----------------------------
  *
  *********************************************************************************************************
  */

#include "touchscreenlib.h"
#include "touchscreen.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcd_4.3.h"
#include "s3c2440_lcd_controller.h"
#include "geometry.h"
#include "font.h"
#include "led.h"
#include "key.h"
#include "uart.h"

//#define DEBUG

#ifndef DEBUG
#define Debug_Printf			printfnull
#else
#define Debug_Printf			printfln
#endif

#define TSHOLDRSE				TOUCHSCREEN_TSHOLDRSE
#define TSREVERSE				TOUCHSCREEN_TSREVERSE

static double TouchScreenKx;
static double TouchScreenKy;

static int TouchScreenXc, TouchScreenYc;
static int TouchScreenLcdXc, TouchScreenLcdYc;

static int TouchScreenXYSwap = TSHOLDRSE;

static int TouchScreenLib_GetLcdXFrm_TS_X(int tsX)
{
	return TouchScreenKx * (tsX - TouchScreenXc) + TouchScreenLcdXc;
}

static int TouchScreenLib_GetLcdYFrm_TS_Y(int tsY)
{
	return TouchScreenKy * (tsY - TouchScreenYc) + TouchScreenLcdYc;
}

/**********************************************************************************************************
 @Function			static void TouchScreenLib_GetCalibratePointData(int lcd_x, int lcd_y, int* px, int* py)
 @Description			TouchScreenLib_GetCalibratePointData
 @Input				lcd_x
					lcd_y
					px
					py
 @Return				void
**********************************************************************************************************/
static void TouchScreenLib_GetCalibratePointData(int lcd_x, int lcd_y, int* px, int* py)
{
	int touchScreenX, touchScreenY, touchScreenPressure;
	int touchScreenSumX = 0, touchScreenSumY = 0, touchScreenCnt = 0;
	
	/* -LCD画十字- */
	LCD_Geometry_Draw_Cross(lcd_x, lcd_y, 10, 0xFFFFFF);
	
	/* -等待点击- */
	do {
		TouchScreen_ReadRaw_TouchScreen_XY(&touchScreenX, &touchScreenY, &touchScreenPressure);
	} while (touchScreenPressure == 0);
	
	do {
		if (touchScreenCnt < 128) {
			touchScreenSumX += touchScreenX;
			touchScreenSumY += touchScreenY;
			touchScreenCnt++;
		}
		TouchScreen_ReadRaw_TouchScreen_XY(&touchScreenX, &touchScreenY, &touchScreenPressure);
		
		Debug_Printf("[TouchScreen Lib: Raw Data X = %08d, Y = %08d, Cnt = %d.]", touchScreenX, touchScreenY, touchScreenCnt);
		
	} while (touchScreenPressure);
	
	*px = touchScreenSumX / touchScreenCnt;
	*py = touchScreenSumY / touchScreenCnt;
	
	Debug_Printf("[TouchScreen Lib: Return Raw Data X = %08d, Y = %08d.]", *px, *py);
	
	/* -LCD清除十字- */
	LCD_Geometry_Draw_Cross(lcd_x, lcd_y, 10, 0x000000);
}

/**********************************************************************************************************
 @Function			static int TouchScreenLib_IsXY_Swap(int aTsX, int aTsY, int bTsX, int bTsY)
 @Description			TouchScreenLib_IsXY_Swap
 @Input				aTsX
					aTsY
					bTsX
					bTsY
 @Return				TSHOLDRSE
					TSREVERSE
**********************************************************************************************************/
static int TouchScreenLib_IsXY_Swap(int aTsX, int aTsY, int bTsX, int bTsY)
{
	int dx = bTsX - aTsX;
	int dy = bTsY - aTsY;
	
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	
	if (dx > dy) return TSHOLDRSE; else return TSREVERSE;
}

/**********************************************************************************************************
 @Function			static void TouchScreenLib_XY_Swap(int* px, int* py)
 @Description			TouchScreenLib_XY_Swap
 @Input				px
					py
 @Return				void
**********************************************************************************************************/
static void TouchScreenLib_XY_Swap(int* px, int* py)
{
	int swapVal = *px;
	
	*px = *py;
	*py = swapVal;
}

/**********************************************************************************************************
 @Function			void TouchScreenLib_Calibrate(void)
 @Description			TouchScreenLib_Calibrate
 @Input				void
 @Return				void
**********************************************************************************************************/
void TouchScreenLib_Calibrate(void)
{
	int AtouchScreenX, AtouchScreenY;
	int BtouchScreenX, BtouchScreenY;
	int CtouchScreenX, CtouchScreenY;
	int DtouchScreenX, DtouchScreenY;
	int EtouchScreenX, EtouchScreenY;
	
	/* -X轴方向- */
	int touchScreenS1, touchScreenS2;
	int lcdTouchScreenS;
	
	/* -Y轴方向- */
	int touchScreenD1, touchScreenD2;
	int lcdTouchScreenD;
	
	/* -LCD获取参数: fb_base, xres, yres, bpp- */
	LCD_Get_Params(&lcd_user_selected.fb_base, &lcd_user_selected.xres, &lcd_user_selected.yres, &lcd_user_selected.bpp);
	
	/* -对于ABCDE, 循环显示"+", 点击读TouchScreen原始值- */
	
	/* -A(50, 50)- */
	TouchScreenLib_GetCalibratePointData(50, 50, &AtouchScreenX, &AtouchScreenY);
	
	/* -B(xres-50, 50)- */
	TouchScreenLib_GetCalibratePointData(lcd_user_selected.xres-50, 50, &BtouchScreenX, &BtouchScreenY);
	
	/* -C(xres-50, yres-50)- */
	TouchScreenLib_GetCalibratePointData(lcd_user_selected.xres-50, lcd_user_selected.yres-50, &CtouchScreenX, &CtouchScreenY);
	
	/* -D(50, yres-50)- */
	TouchScreenLib_GetCalibratePointData(50, lcd_user_selected.yres-50, &DtouchScreenX, &DtouchScreenY);
	
	/* -E(xres/2, yres/2)- */
	TouchScreenLib_GetCalibratePointData(lcd_user_selected.xres/2, lcd_user_selected.yres/2, &EtouchScreenX, &EtouchScreenY);
	
	/* -确定触摸屏数据XY是否反转- */
	TouchScreenXYSwap = TouchScreenLib_IsXY_Swap(AtouchScreenX, AtouchScreenY, BtouchScreenX, BtouchScreenY);
	
	/* -坐标反转- */
	if (TouchScreenXYSwap == TSREVERSE) {
		TouchScreenLib_XY_Swap(&AtouchScreenX, &AtouchScreenY);
		TouchScreenLib_XY_Swap(&BtouchScreenX, &BtouchScreenY);
		TouchScreenLib_XY_Swap(&CtouchScreenX, &CtouchScreenY);
		TouchScreenLib_XY_Swap(&DtouchScreenX, &DtouchScreenY);
		TouchScreenLib_XY_Swap(&EtouchScreenX, &EtouchScreenY);
	}
	
	/* -确定公式参数并保存- */
	touchScreenS1    = BtouchScreenX - AtouchScreenX;
	touchScreenS2    = CtouchScreenX - DtouchScreenX;
	lcdTouchScreenS  = lcd_user_selected.xres - 50 - 50;
	
	touchScreenD1    = DtouchScreenY - AtouchScreenY;
	touchScreenD2    = CtouchScreenY - BtouchScreenY;
	lcdTouchScreenD  = lcd_user_selected.yres - 50 - 50;
	
	TouchScreenKx    = ((double)(2 * lcdTouchScreenS)) / (touchScreenS1 + touchScreenS2);
	TouchScreenKy    = ((double)(2 * lcdTouchScreenD)) / (touchScreenD1 + touchScreenD2);
	
	TouchScreenXc    = EtouchScreenX;
	TouchScreenYc    = EtouchScreenY;
	
	TouchScreenLcdXc = lcd_user_selected.xres / 2;
	TouchScreenLcdYc = lcd_user_selected.yres / 2;
	
	Debug_Printf("[TouchScreen Lib: A LCD-X = %08d, LCD-Y = %08d.]", TouchScreenLib_GetLcdXFrm_TS_X(AtouchScreenX), TouchScreenLib_GetLcdYFrm_TS_Y(AtouchScreenY));
	Debug_Printf("[TouchScreen Lib: B LCD-X = %08d, LCD-Y = %08d.]", TouchScreenLib_GetLcdXFrm_TS_X(BtouchScreenX), TouchScreenLib_GetLcdYFrm_TS_Y(BtouchScreenY));
	Debug_Printf("[TouchScreen Lib: C LCD-X = %08d, LCD-Y = %08d.]", TouchScreenLib_GetLcdXFrm_TS_X(CtouchScreenX), TouchScreenLib_GetLcdYFrm_TS_Y(CtouchScreenY));
	Debug_Printf("[TouchScreen Lib: D LCD-X = %08d, LCD-Y = %08d.]", TouchScreenLib_GetLcdXFrm_TS_X(DtouchScreenX), TouchScreenLib_GetLcdYFrm_TS_Y(DtouchScreenY));
	Debug_Printf("[TouchScreen Lib: E LCD-X = %08d, LCD-Y = %08d.]", TouchScreenLib_GetLcdXFrm_TS_X(EtouchScreenX), TouchScreenLib_GetLcdYFrm_TS_Y(EtouchScreenY));
	
	TouchScreenLib_Debug_Printf_Parameter();
}

/**********************************************************************************************************
 @Function			int TouchScreenLib_ReadTsChangeLcd(int* lcd_x, int* lcd_y, int* lcd_pressure)
 @Description			TouchScreenLib_ReadTsChangeLcd
 @Input				lcd_x
					lcd_y
					lcd_pressure
 @Return				0 or -1
**********************************************************************************************************/
int TouchScreenLib_ReadTsChangeLcd(int* lcd_x, int* lcd_y, int* lcd_pressure)
{
	int touchScreenX, touchScreenY, touchScreenPressure;
	
	TouchScreen_ReadRaw_TouchScreen_XY(&touchScreenX, &touchScreenY, &touchScreenPressure);
	
	/* -坐标反转- */
	if (TouchScreenXYSwap == TSREVERSE) {
		TouchScreenLib_XY_Swap(&touchScreenX, &touchScreenY);
	}
	
	/* -使用公式计算- */
	*lcd_x = TouchScreenKx * (touchScreenX - TouchScreenXc) + TouchScreenLcdXc;
	*lcd_y = TouchScreenKy * (touchScreenY - TouchScreenYc) + TouchScreenLcdYc;
	*lcd_pressure = touchScreenPressure;
	
	if (*lcd_x < 0 || *lcd_x >= lcd_user_selected.xres || *lcd_y < 0 || *lcd_y >= lcd_user_selected.yres) {
		*lcd_x = 0;
		*lcd_y = 0;
		*lcd_pressure = 0;
		return -1;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			void TouchScreenLib_SetCalibrateParameter(double tsKx, double tsKy, int tsXc, int tsYc, int lcdXc, int lcdYc, int tsXYSwap)
 @Description			TouchScreenLib_SetCalibrateParameter
 @Input				double tsKx, double tsKy, int tsXc, int tsYc, int lcdXc, int lcdYc, int tsXYSwap
 @Return				void
**********************************************************************************************************/
void TouchScreenLib_SetCalibrateParameter(double tsKx, double tsKy, int tsXc, int tsYc, int lcdXc, int lcdYc, int tsXYSwap)
{
	TouchScreenKx = tsKx;
	TouchScreenKy = tsKy;
	
	TouchScreenXc = tsXc;
	TouchScreenYc = tsYc;
	
	TouchScreenLcdXc = lcdXc;
	TouchScreenLcdYc = lcdYc;
	
	TouchScreenXYSwap = tsXYSwap;
}

/**********************************************************************************************************
 @Function			void TouchScreenLib_Debug_Printf_Parameter(void)
 @Description			TouchScreenLib_Debug_Printf_Parameter
 @Input				void
 @Return				void
**********************************************************************************************************/
void TouchScreenLib_Debug_Printf_Parameter(void)
{
	Debug_Printf("[TouchScreen Lib: TouchScreenKx = %lf.]", TouchScreenKx);
	Debug_Printf("[TouchScreen Lib: TouchScreenKy = %lf.]", TouchScreenKy);
	
	Debug_Printf("[TouchScreen Lib: TouchScreenXc = %d.]", TouchScreenXc);
	Debug_Printf("[TouchScreen Lib: TouchScreenYc = %d.]", TouchScreenYc);
	
	Debug_Printf("[TouchScreen Lib: TouchScreenLcdXc = %d.]", TouchScreenLcdXc);
	Debug_Printf("[TouchScreen Lib: TouchScreenLcdYc = %d.]", TouchScreenLcdYc);
	
	Debug_Printf("[TouchScreen Lib: TouchScreenXYSwap = %d.]", TouchScreenXYSwap);
}

/********************************************** END OF FLEE **********************************************/
