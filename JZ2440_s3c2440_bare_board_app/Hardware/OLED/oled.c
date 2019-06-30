/**
  *********************************************************************************************************
  * @file    oled.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-30
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "oled.h"
#include "oledfont.h"
#include "spi_gpio.h"

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_GPIO_Init(void)
 @Description			S3C2440_OLED_GPIO_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_GPIO_Init(void)
{
	/* -GPG4 OLED_DC Output- */
	GPGCON &= ~(3<<(4*2));
	GPGCON |=  (1<<(4*2));
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_SET_DC(bool val)
 @Description			S3C2440_OLED_SET_DC
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_SET_DC(bool val)
{
	/* -GPG4 OLED_DC Output for Data or Cmd- */
	if (val)
		GPGDAT |=  (1<<4);
	else
		GPGDAT &= ~(1<<4);
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_SET_CS(bool val)
 @Description			S3C2440_OLED_SET_CS
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_SET_CS(bool val)
{
	/* -GPF1 OLED_CS Output for Chip Select- */
	if (val)
		GPFDAT |=  (1<<1);
	else
		GPFDAT &= ~(1<<1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_WriteCmd(unsigned char cmd)
 @Description			S3C2440_OLED_WriteCmd
 @Input				cmd
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_WriteCmd(unsigned char cmd)
{
	/* -Command- */
	S3C2440_OLED_SET_DC(0);
	/* -Select OLED NSS- */
	S3C2440_OLED_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(cmd);
	
	/* -De-Select OLED NSS- */
	S3C2440_OLED_SET_CS(1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_WriteDat(unsigned char dat)
 @Description			S3C2440_OLED_WriteDat
 @Input				dat
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_WriteDat(unsigned char dat)
{
	/* -Data- */
	S3C2440_OLED_SET_DC(1);
	/* -Select OLED NSS- */
	S3C2440_OLED_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(dat);
	
	/* -De-Select OLED NSS- */
	S3C2440_OLED_SET_CS(1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_SetPageAddrMode(void)
 @Description			S3C2440_OLED_SetPageAddrMode
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_SetPageAddrMode(void)
{
	S3C2440_OLED_WriteCmd(0x20);
	S3C2440_OLED_WriteCmd(0x02);
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_SetPos(int page, int col)
 @Description			S3C2440_OLED_SetPos
 @Input				page
					col
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_SetPos(int page, int col)
{
	/* -Page Address- */
	S3C2440_OLED_WriteCmd(0xB0 + page);
	
	/* -Lower Column Start Address- */
	S3C2440_OLED_WriteCmd(col & 0x0F);
	/* -High Column Start Address- */
	S3C2440_OLED_WriteCmd(0x10 + (col >> 4));
}

/**********************************************************************************************************
 @Function			static void S3C2440_OLED_Clear(void)
 @Description			S3C2440_OLED_Clear
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_OLED_Clear(void)
{
	for (int page = 0; page < 8; page++) {
		S3C2440_OLED_SetPos(page, 0);
		for (int i = 0; i < 128; i++) {
			S3C2440_OLED_WriteDat(0);
		}
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_OLEDInitialized(void)
 @Description			S3C2440_OLEDInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_OLEDInitialized(void)
{
	S3C2440_SPI_GPIOInitialized();
	
	S3C2440_OLED_GPIO_Init();
	
	/* -向OLED发送命令初始化- */
	S3C2440_OLED_WriteCmd(0xAE); /*display off*/
	S3C2440_OLED_WriteCmd(0x00); /*set lower column address*/
	S3C2440_OLED_WriteCmd(0x10); /*set higher column address*/
	S3C2440_OLED_WriteCmd(0x40); /*set display start line*/
	S3C2440_OLED_WriteCmd(0xB0); /*set page address*/
	S3C2440_OLED_WriteCmd(0x81); /*contract control*/
	S3C2440_OLED_WriteCmd(0x66); /*128*/
	S3C2440_OLED_WriteCmd(0xA1); /*set segment remap*/
	S3C2440_OLED_WriteCmd(0xA6); /*normal / reverse*/
	S3C2440_OLED_WriteCmd(0xA8); /*multiplex ratio*/
	S3C2440_OLED_WriteCmd(0x3F); /*duty = 1/64*/
	S3C2440_OLED_WriteCmd(0xC8); /*Com scan direction*/
	S3C2440_OLED_WriteCmd(0xD3); /*set display offset*/
	S3C2440_OLED_WriteCmd(0x00);
	S3C2440_OLED_WriteCmd(0xD5); /*set osc division*/
	S3C2440_OLED_WriteCmd(0x80);
	S3C2440_OLED_WriteCmd(0xD9); /*set pre-charge period*/
	S3C2440_OLED_WriteCmd(0x1f);
	S3C2440_OLED_WriteCmd(0xDA); /*set COM pins*/
	S3C2440_OLED_WriteCmd(0x12);
	S3C2440_OLED_WriteCmd(0xdb); /*set vcomh*/
	S3C2440_OLED_WriteCmd(0x30);
	S3C2440_OLED_WriteCmd(0x8d); /*set charge pump enable*/
	S3C2440_OLED_WriteCmd(0x14);
	
	S3C2440_OLED_SetPageAddrMode();
	
	S3C2440_OLED_Clear();
	
	S3C2440_OLED_WriteCmd(0xAF);
}

/**********************************************************************************************************
 @Function			void S3C2440_OLEDPutChar(int page, int col, char ch)
 @Description			S3C2440_OLEDPutChar
 @Input				page
					col
					ch
 @Return				void
**********************************************************************************************************/
void S3C2440_OLEDPutChar(int page, int col, char ch)
{
	const unsigned char* dots = oled_asc2_8x16[ch - ' '];
	
	S3C2440_OLED_SetPos(page, col);
	for (int i = 0; i < 8; i++) {
		S3C2440_OLED_WriteDat(dots[i]);
	}
	
	S3C2440_OLED_SetPos(page+1, col);
	for (int i = 0; i < 8; i++) {
		S3C2440_OLED_WriteDat(dots[i+8]);
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_OLEDPrint(int page, int col, char* str)
 @Description			S3C2440_OLEDPrint
 @Input				page
					col
					str
 @Return				void
**********************************************************************************************************/
void S3C2440_OLEDPrint(int page, int col, char* str)
{
	for (int i = 0; str[i] != 0; i++) {
		S3C2440_OLEDPutChar(page, col, str[i]);
		col += 8;
		if (col > 127) {
			col = 0;
			page += 2;
		}
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_OLEDClearPage(int page)
 @Description			S3C2440_OLEDClearPage
 @Input				page
 @Return				void
**********************************************************************************************************/
void S3C2440_OLEDClearPage(int page)
{
	S3C2440_OLED_SetPos(page, 0);
	for (int i = 0; i < 128; i++) {
		S3C2440_OLED_WriteDat(0);
	}
}

/********************************************** END OF FLEE **********************************************/
