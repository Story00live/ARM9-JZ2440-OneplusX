#ifndef __S3C2440_TOUCHSCREEN_LIB_H
#define   __S3C2440_TOUCHSCREEN_LIB_H

#include "s3c2440_soc.h"

#define TOUCHSCREEN_TSHOLDRSE		0
#define TOUCHSCREEN_TSREVERSE		1

#define TOUCHSCREEN_KX			0.540541
#define TOUCHSCREEN_KY			0.357588
#define TOUCHSCREEN_XC			526
#define TOUCHSCREEN_YC			526
#define TOUCHSCREEN_LCD_XC		240
#define TOUCHSCREEN_LCD_YC		136
#define TOUCHSCREEN_XYSWAP		TOUCHSCREEN_TSREVERSE

void TouchScreenLib_Calibrate(void);

int  TouchScreenLib_ReadTsChangeLcd(int* lcd_x, int* lcd_y, int* lcd_pressure);

void TouchScreenLib_SetCalibrateParameter(double tsKx, double tsKy, int tsXc, int tsYc, int lcdXc, int lcdYc, int tsXYSwap);

void TouchScreenLib_Debug_Printf_Parameter(void);

#endif /* __S3C2440_TOUCHSCREEN_LIB_H */
