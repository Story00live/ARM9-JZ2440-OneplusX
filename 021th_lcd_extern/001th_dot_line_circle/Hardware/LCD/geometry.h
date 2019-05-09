#ifndef __GEOMETRY_H
#define   __GEOMETRY_H

#include "s3c2440_soc.h"

void LCD_Geometry_Draw_Circle(int x, int y, int r, int color);					//画圆
void LCD_Geometry_Draw_Line(int x1, int y1, int x2, int y2, int color);			//画线

#endif /* __GEOMETRY_H */
