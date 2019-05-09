/**
  *********************************************************************************************************
  * @file    lcd.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-05-07
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "lcd.h"
#include "s3c2440_lcd_controller.h"
#include "lcd_4.3.h"

static p_lcd_params p_lcd_params_array[LCD_PARAMS_NUM];
static p_lcd_params p_lcd_params_selected;

/**********************************************************************************************************
 @Function			int LCD_register(p_lcd_params plcd)
 @Description			LCD_register					: LCD注册
 @Input				plcd							: lcd参数
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int LCD_register(p_lcd_params plcd)
{
	for (int i = 0; i < LCD_PARAMS_NUM; i++) {
		if (!p_lcd_params_array[i]) {
			p_lcd_params_array[i] = plcd;
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_select(char* name)
 @Description			LCD_select					: LCD选择
 @Input				name							: lcd名
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int LCD_select(char* name)
{
	for (int i = 0; i < LCD_PARAMS_NUM; i++) {
		if (p_lcd_params_array[i] && !strcmp(p_lcd_params_array[i]->name, name)) {
			p_lcd_params_selected = p_lcd_params_array[i];
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_enable(void)
 @Description			LCD_enable					: LCD使能
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_enable(void)
{
	return LCD_Controller_enable();
}

/**********************************************************************************************************
 @Function			int LCD_disable(void)
 @Description			LCD_disable					: LCD失能
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_disable(void)
{
	return LCD_Controller_disable();
}

/**********************************************************************************************************
 @Function			void LCD_add(void)
 @Description			LCD_add						: LCD增加
 @Input				void
 @Return				void
**********************************************************************************************************/
void LCD_add(void)
{
	LCD_4_3_Register();
}

/**********************************************************************************************************
 @Function			int LCD_Init(void)
 @Description			LCD_Init						: LCD初始化
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_Init(void)
{
	/* 注册LCD */
	LCD_add();
	
	/* 注册LCD控制器 */
	LCD_Controller_add();
	
	/* 选择LCD */
	if (LCD_select("lcd_4.3") == -1) return -1;
	
	/* 选择LCD控制器 */
	if (LCD_Controller_select("s3c2440") == -1) return -1;
	
	/* 使用LCD参数, 初始化LCD控制器 */
	if (LCD_Controller_init(p_lcd_params_selected) == -1) return -1;
	
	return 0;
}

/**********************************************************************************************************
 @Function			void LCD_Get_Params(unsigned int* fb_base, int* xres, int* yres, int* bpp)
 @Description			LCD_Get_Params					: LCD参数获取
 @Input				参数存放地址
 @Return				void
**********************************************************************************************************/
void LCD_Get_Params(unsigned int* fb_base, int* xres, int* yres, int* bpp)
{
	*fb_base	= p_lcd_params_selected->fb_base;
	*xres	= p_lcd_params_selected->xres;
	*yres	= p_lcd_params_selected->yres;
	*bpp		= p_lcd_params_selected->bpp;
}

/********************************************** END OF FLEE **********************************************/
