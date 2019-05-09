/**
  *********************************************************************************************************
  * @file    lcd_controller.c
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

#include "lcd_controller.h"
#include "s3c2440_lcd_controller.h"
#include "lcd_4.3.h"

static p_lcd_controller p_lcd_controller_array[LCD_CONTROLLER_NUM];
static p_lcd_controller p_lcd_controller_selected;

/**********************************************************************************************************
 @Function			int LCD_Controller_register(p_lcd_controller plcdcon)
 @Description			LCD_Controller_register			: LCD控制注册
 @Input				plcdcon						: lcd控制参数
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int LCD_Controller_register(p_lcd_controller plcdcon)
{
	for (int i = 0; i < LCD_CONTROLLER_NUM; i++) {
		if (!p_lcd_controller_array[i]) {
			p_lcd_controller_array[i] = plcdcon;
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_Controller_select(char* name)
 @Description			LCD_Controller_select			: LCD选择控制
 @Input				name							: lcd控制名
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int LCD_Controller_select(char* name)
{
	for (int i = 0; i < LCD_CONTROLLER_NUM; i++) {
		if (p_lcd_controller_array[i] && !strcmp(p_lcd_controller_array[i]->name, name)) {
			p_lcd_controller_selected = p_lcd_controller_array[i];
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_Controller_init(p_lcd_params plcdparams)
 @Description			LCD_Controller_init				: LCD控制初始化
 @Input				plcdparams					: lcd参数
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_Controller_init(p_lcd_params plcdparams)
{
	if (p_lcd_controller_selected) {
		p_lcd_controller_selected->init(plcdparams);
		p_lcd_controller_selected->init_palette();
		return 0;
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_Controller_enable(void)
 @Description			LCD_Controller_enable			: LCD控制使能
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_Controller_enable(void)
{
	if (p_lcd_controller_selected) {
		p_lcd_controller_selected->enable();
		return 0;
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int LCD_Controller_disable(void)
 @Description			LCD_Controller_disable			: LCD控制失能
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int LCD_Controller_disable(void)
{
	if (p_lcd_controller_selected) {
		p_lcd_controller_selected->disable();
		return 0;
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			void LCD_Controller_add(void)
 @Description			LCD_Controller_add				: LCD控制器增加
 @Input				void
 @Return				void
**********************************************************************************************************/
void LCD_Controller_add(void)
{
	S3C2440_LCD_Controller_Register();
}

/********************************************** END OF FLEE **********************************************/
