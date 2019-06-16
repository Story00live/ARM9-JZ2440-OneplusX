#ifndef __LCD_CONTROLLER_H
#define   __LCD_CONTROLLER_H

#include "s3c2440_soc.h"
#include "lcd.h"

#define LCD_CONTROLLER_NUM				10

/* LCD 控制方法 */
typedef struct lcd_controller
{
	char				*name;
	void				(*init)(p_lcd_params plcdparams);
	void				(*enable)(void);
	void				(*disable)(void);
	void				(*init_palette)(void);
}lcd_controller, *p_lcd_controller;

int LCD_Controller_register(p_lcd_controller plcdcon);							//LCD控制注册
int LCD_Controller_select(char* name);										//LCD选择控制

int LCD_Controller_init(p_lcd_params plcdparams);								//LCD控制初始化
int LCD_Controller_enable(void);											//LCD控制使能
int LCD_Controller_disable(void);											//LCD控制失能

void LCD_Controller_add(void);											//LCD控制器增加

#endif /* __LCD_CONTROLLER_H */
