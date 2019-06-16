/**
  *********************************************************************************************************
  * @file    lcd_4.3.c
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

#include "lcd_4.3.h"

struct lcd_params lcd_4_3_params = {
	.name			= "lcd_4.3",
	
	/* -引脚极性- */
	.pins_pol			= {
		.de				= NORMAL,										// normal: 高电平时可以传输数据
		.pwren			= NORMAL,										// normal: 高电平有效
		.vclk			= NORMAL,										// normal: 下降沿获取数据
		.rgb				= NORMAL,										// normal: 高电平表示1
		.hsync			= INVERT,										// normal: 高脉冲 invert: 低脉冲
		.vsync			= INVERT										// normal: 高脉冲 invert: 低脉冲
	},
	
	/* -时序- */
	.time_seq			= {
		/* -垂直方向- */
		.tvp				= 10,										// vysnc脉冲宽度
		.tvb				= 2,											// 上边黑框, Vertical Back porch
		.tvf				= 2,											// 下边黑框, Vertical Front porch
		
		/* -水平方向- */
		.thp				= 41,										// hsync脉冲宽度
		.thb				= 2,											// 左边黑框, Horizontal Back porch
		.thf				= 2,											// 右边黑框, Horizontal Front porch
		
		/* -同步时钟- */
		.vclk			= 9											// MHz
	},
	
	/* -分辨率, bpp, 视屏输出格式- */
	.xres			= 480,
	.yres			= 272,
	.bpp				= 32,											// 8, 16, 32 no 24bpp
	.frm565			= 1,
	
	/* -framebuffer地址- */
	.fb_base			= LCD_FB_BASE
};

/**********************************************************************************************************
 @Function			void LCD_4_3_Register(void)
 @Description			LCD_4_3_Register			: LCD4.3注册
 @Input				void
 @Return				void
**********************************************************************************************************/
void LCD_4_3_Register(void)
{
	LCD_register(&lcd_4_3_params);
}

/********************************************** END OF FLEE **********************************************/
