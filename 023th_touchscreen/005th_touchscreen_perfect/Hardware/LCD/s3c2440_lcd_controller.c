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
 @Description			S3C2440_LCD_GPIO_Init				: LCD引脚初始化
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
 @Description			S3C2440_LCD_Controller_Init			: LCD控制初始化
 @Input				plcdparams						: LCD参数结构体指针
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
	int pnrmode = 0x03;
	int bppmode = plcdparams->bpp == 8 ? 0x0B : plcdparams->bpp == 16 ? 0x0C : 0x0D;
	
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
	int pixelplace = plcdparams->bpp == 32 ? ((0<<12)|(0<<1)|(0<<0)) : \
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
 @Description			S3C2440_LCD_Controller_Enable			: LCD控制使能
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
 @Description			S3C2440_LCD_Controller_Disable		: LCD控制失能
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

static const unsigned short DEMO256pal[] = {
	0x0b5e,0xce9a,0xffd9,0x9d99,0xb63a,0xae7c,0xdd71,0x6c57,0xfd4d,0x00ae,0x9c4d,0xb5f8,0xad96,0x0131,0x0176,0xefff,0xcedd,0x9556,0xe4bf,0x00b6,0x22b7,0x002b,0x89de,0x002c,0x57df,0xab5f,0x3031,0x14bf,0x797e,0x5391,0x93ab,0x7239,0x7453,0xafdf,0x71b9,0x8c92,0x014d,0x302e,0x5175,0x0029,0x0969,0x004e,0x2a6d,0x0021,0x3155,0x4b6e,0xd677,0xf6b6,0x9b5f,0x4bb5,0xffd5,0x0027,0xdfdf,0x74d8,0x1256,0x6bcd,0x9b08,0x2ab2,0xbd72,0x84b5,0xfe52,0xd4ad,0x00ad,0xfffc,0x422b,0x73b0,0x0024,0x5246,0x8e5e,0x28b3,0x0050,0x3b52,0x2a4a,0x3a74,0x8559,0x3356,0x1251,0x9abf,0x4034,0x40b1,
	0x8cb9,0x00b3,0x5c55,0xdf3d,0x61b7,0x1f5f,0x00d9,0x4c59,0x0926,0xac3f,0x925f,0x85bc,0x29d2,0xc73f,0xef5c,0xcb9f,0x827b,0x5279,0x4af5,0x01b9,0x4290,0xf718,0x126d,0x21a6,0x515e,0xefbd,0xd75e,0x42ab,0x00aa,0x10b3,0x7349,0x63b5,0x61a3,0xaadf,0xcb27,0x87df,0x6359,0xc7df,0x4876,0xb5bc,0x4114,0xfe2e,0xef5e,0x65be,0x43b9,0xe5df,0x21c9,0x7d16,0x6abb,0x5c11,0x49f7,0xbc0b,0x9e1a,0x3b0f,0x202b,0xff12,0x821b,0x842f,0xbccf,0xdefb,0x8a3e,0x68fa,0xa4f1,0x38ae,0x28b7,0x21ad,0x31d7,0x0073,0x182b,0x1831,0x3415,0xbdf6,0x2dbf,0x0a5d,0xc73d,0x182c,0x293e,0x7b3d,0x643d,0x3cbd,
	0x92dd,0x09d4,0x1029,0x7cdd,0x6239,0x182e,0x5aea,0x11eb,0x8abc,0x7bfa,0x00a7,0x2153,0x1853,0x1318,0x0109,0x54fa,0x72a7,0x89e3,0x01cf,0x3a07,0x7b17,0x1a14,0x2150,0x23dc,0x4142,0x1b33,0x00a4,0xf6df,0x08fc,0x18ae,0x3a7e,0x18d1,0xa51c,0xff5a,0x1a0f,0x28fa,0xdfbe,0x82de,0x60d7,0x1027,0x48fa,0x5150,0x6213,0x89d6,0x110d,0x9bbb,0xbedd,0x28e1,0x1925,0xf449,0xaa79,0xd5f4,0x693c,0x110a,0x2889,0x08a2,0x923d,0x10a6,0xd9bc,0x5b2e,0x32ec,0xcf7f,0x1025,0x2148,0x74b4,0x6d59,0x9d14,0x0132,0x00f0,0x56bf,0x00f1,0xffff,0x0173,0x0133,0x00b0,0x00b1,0xf7ff,0x08b1,0xfffe,0x08b0,
	0x0171,0xf7bf,0x10f3,0xf7fe,0x08ef,0x1192,0xefbe,0x1131,0x2177,0xff9f,0x1116,0xffbc,0x5914,0x22ef,0xb285,0xa6df,
};

/**********************************************************************************************************
 @Function			void S3C2440_LCD_Controller_Init_Palette(void)
 @Description			S3C2440_LCD_Controller_Init_Palette	: LCD调色板初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_LCD_Controller_Init_Palette(void)
{
	volatile unsigned int* palette_base = (volatile unsigned int*)0x4D000400;
	int bit = LCDCON1 & (1<<0);
	
	if (bit) LCDCON1 &= ~(1<<0);
	
	for (int i = 0; i < 256; i++) {
		*palette_base++ = DEMO256pal[i];
	}
	
	if (bit) LCDCON1 |=  (1<<0);
}

struct lcd_controller s3c2440_lcd_controller = {
	.name			= "s3c2440",
	.init			= S3C2440_LCD_Controller_Init,
	.enable			= S3C2440_LCD_Controller_Enable,
	.disable			= S3C2440_LCD_Controller_Disable,
	.init_palette		= S3C2440_LCD_Controller_Init_Palette
};

/**********************************************************************************************************
 @Function			void S3C2440_LCD_Controller_Register(void)
 @Description			S3C2440_LCD_Controller_Register		: LCD控制注册
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_LCD_Controller_Register(void)
{
	LCD_Controller_register(&s3c2440_lcd_controller);
}

/********************************************** END OF FLEE **********************************************/
