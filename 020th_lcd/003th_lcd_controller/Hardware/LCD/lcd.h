#ifndef __LCD_H
#define   __LCD_H

#include "s3c2440_soc.h"

/* 极性 */
enum {
	NORMAL			= 0x00,											// 正常极性
	INVERT			= 0x01											// 反转极性
};

/* LCD 引脚极性 */
typedef struct pins_polarity
{
	int				de;												// normal: 高电平时可以传输数据
	int				pwren;											// normal: 高电平有效
	int				vclk;											// normal: 下降沿获取数据
	int				rgb;												// normal: 高电平表示1
	int				hsync;											// normal: 高脉冲
	int				vsync;											// normal: 高脉冲
}pins_polarity, *p_pins_polarity;

/* LCD 时序 */
typedef struct time_sequence
{
	/* -垂直方向- */
	int				tvp;												// vysnc脉冲宽度
	int				tvb;												// 上边黑框, Vertical Back porch
	int				tvf;												// 下边黑框, Vertical Front porch
	
	/* -水平方向- */
	int				thp;												// hsync脉冲宽度
	int				thb;												// 左边黑框, Horizontal Back porch
	int				thf;												// 右边黑框, Horizontal Front porch
	
	/* -同步时钟- */
	int				vclk;
}time_sequence, *p_time_sequence;

/* LCD 参数 */
typedef struct lcd_params
{
	/* -引脚极性- */
	pins_polarity		pins_pol;
	
	/* -时序- */
	time_sequence		time_seq;
	
	/* -分辨率, bpp, 视屏输出格式- */
	int				xres;
	int				yres;
	int				bpp;
	int				frm565;
	
	/* -framebuffer地址- */
	unsigned int		fb_base;
}lcd_params, *p_lcd_params;

#endif /* __LCD_H */
