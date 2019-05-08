/**
  *********************************************************************************************************
  * @file    s3c2440_lcd_controller.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-04-27
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "s3c2440_lcd_controller.h"

/**********************************************************************************************************
 @Function			static void S3C2440_LCD_GPIO_Init(void)
 @Description			S3C2440_LCD_GPIO_Init			: LCD引脚初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_LCD_GPIO_Init(void)
{
	/* -背光引脚 GPB0- */
	GPBCON &= ~(3<<(0*2));
	GPBCON |=  (1<<(0*2));
	
	/* -LCD专用引脚- */
	GPCCON = 0xAAAAAAAA;
	GPDCON = 0xAAAAAAAA;
	
	/* PWREN GPG4 */
	GPGCON &= ~(3<<(4*2));
	GPGCON |=  (3<<(4*2));
}

/**********************************************************************************************************
 @Function			void S3C2440_LCD_Controller_Init(p_lcd_params plcdparams)
 @Description			S3C2440_LCD_Controller_Init		: LCD控制初始化
 @Input				plcdparams					: LCD参数结构体指针
 @Return				void
**********************************************************************************************************/
void S3C2440_LCD_Controller_Init(p_lcd_params plcdparams)
{
	S3C2440_LCD_GPIO_Init();
	
	/* -[17: 8]: CLKVAL TFT：VCLK = HCLK / [(CLKVAL + 1) × 2]-
	 * -[ 6: 5]: 0b11 = TFT LCD 面板-
	 * -[ 4: 1]: BPPMODE-
	 * -[  0  ]: ENVID LCD video output and the logic enable/disable-
	 */
	int clkval  = (float)S3C2440_SYSTEM_HCLK / plcdparams->time_seq.vclk / 2 - 1 + 0.5;
	int pnrmode = 0b11;
	int bppmode = plcdparams->bpp == 8 ? 0b1011 : plcdparams->bpp == 16 ? 0b1100 : plcdparams->bpp == 24 ? 0b1101 : 0b1100;
	
	LCDCON1 = (clkval << 8) | (pnrmode << 5) | (bppmode << 1);
	
	/* -[31:24]: VBPD    = tvb  - 1-
	 * -[23:14]: LINEVAL = row  - 1-
	 * -[13: 6]: VFPD    = tvf  - 1-
	 * -[ 5: 0]: VSPW    = tvp  - 1-
	 */
	LCDCON2 = ((plcdparams->time_seq.tvb - 1) << 24) | \
			((plcdparams->yres         - 1) << 14) | \
			((plcdparams->time_seq.tvf - 1) <<  6) | \
			((plcdparams->time_seq.tvp - 1) <<  0);
	
	/* -[25:19]: HBPD	 = thb - 1-
	 * -[18: 8]: HOZVAL  = col - 1-
	 * -[ 7: 0]: HFPD	 = thf - 1-
	 */
	LCDCON3 = ((plcdparams->time_seq.thb - 1) << 19) | \
			((plcdparams->xres         - 1) <<  8) | \
			((plcdparams->time_seq.thf - 1) <<  0);
	
	/* -[ 7: 0]: HSPW	 = thp - 1-
	 */
	LCDCON4 = ((plcdparams->time_seq.thp - 1) << 0);
	
	/* -用来设置引脚极性, 设置16bpp, 设置内存中像素存放的格式-
	 * -[12]: BPP24BL-
	 * -[11]: FRM565, 1-565-
	 * -[10]: INVVCLK, 0 = The video data is fetched at VCLK falling edge-
	 * -[ 9]: HSYNC是否反转-
	 * -[ 8]: VSYNC是否反转-
	 * -[ 7]: INVVD, rgb是否反转-
	 * -[ 6]: INVVDEN-
	 * -[ 5]: INVPWREN-
	 * -[ 4]: INVLEND-
	 * -[ 3]: PWREN, LCD_PWREN output signal enable/disable-
	 * -[ 2]: ENLEND-
	 * -[ 1]: BSWP-
	 * -[ 0]: HWSWP-
	 */
	int pixelplace = plcdparams->bpp == 24 ? ((0<<12)|(0<<1)|(0<<0)) : \
				  plcdparams->bpp == 16 ? ((0<<12)|(0<<1)|(1<<0)) : \
				  plcdparams->bpp ==  8 ? ((0<<12)|(1<<1)|(0<<0)) : \
				  ((0<<12)|(0<<1)|(1<<0));
	
	LCDCON5 = (plcdparams->frm565         << 11) | \
			(plcdparams->pins_pol.vclk  << 10) | \
			(plcdparams->pins_pol.hsync <<  9) | \
			(plcdparams->pins_pol.vsync <<  8) | \
			(plcdparams->pins_pol.rgb   <<  7) | \
			(plcdparams->pins_pol.de    <<  6) | \
			(plcdparams->pins_pol.pwren <<  5) | \
			pixelplace;
	
	/* -framebuffer地址- */
	unsigned int addr = 0;
	/* -[29:21]: LCDBANK,  A[30:22] of fb-
	 * -[20: 0]: LCDBASEU, A[21: 1] of fb-
	 */
	addr = plcdparams->fb_base & ~(1<<31);
	LCDSADDR1 = (addr >> 1);
	
	/* -[20: 0]: LCDBASEL, A[21: 1] of end addr-
	 */
	addr = plcdparams->fb_base + plcdparams->xres * plcdparams->yres * plcdparams->bpp / 8;
	addr >>= 1;
	addr &=  0x1FFFFF;
	LCDSADDR2 = addr;
}

/**********************************************************************************************************
 @Function			void S3C2440_LCD_Controller_Enable(void)
 @Description			S3C2440_LCD_Controller_Enable		: LCD控制使能
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_LCD_Controller_Enable(void)
{
	/* -背光引脚 : GPB0- */
	GPBDAT  |=  (1<<0);
	
	/* -pwren : 给LCD提供AVDD- */
	LCDCON5 |=  (1<<3);
	
	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 |=  (1<<0);
}

/**********************************************************************************************************
 @Function			void S3C2440_LCD_Controller_Disable(void)
 @Description			S3C2440_LCD_Controller_Disable	: LCD控制失能
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_LCD_Controller_Disable(void)
{
	/* -背光引脚 : GPB0- */
	GPBDAT  &= ~(1<<0);
	
	/* -pwren : 给LCD提供AVDD- */
	LCDCON5 &= ~(1<<3);
	
	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 &= ~(1<<0);
}



















struct lcd_controller s3c2440_lcd_controller = {
	.name			= "s3c2440",
	.init			= S3C2440_LCD_Controller_Init,
	.enable			= S3C2440_LCD_Controller_Enable,
	.disable			= S3C2440_LCD_Controller_Disable,
	.init_palette		= NULL
};






/********************************************** END OF FLEE **********************************************/
