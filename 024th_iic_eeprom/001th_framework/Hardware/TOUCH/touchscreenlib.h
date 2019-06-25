#ifndef __S3C2440_TOUCHSCREEN_LIB_H
#define   __S3C2440_TOUCHSCREEN_LIB_H

#include "s3c2440_soc.h"

void TouchScreenLib_Calibrate(void);

int  TouchScreenLib_ReadTsChangeLcd(int* lcd_x, int* lcd_y, int* lcd_pressure);

#endif /* __S3C2440_TOUCHSCREEN_LIB_H */
