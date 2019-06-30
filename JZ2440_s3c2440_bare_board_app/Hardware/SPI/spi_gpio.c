/**
  *********************************************************************************************************
  * @file    spi_gpio.c
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

#include "spi_gpio.h"

/**********************************************************************************************************
 @Function			static void S3C2440_SPI_GPIO_Init(void)
 @Description			S3C2440_SPI_GPIO_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_SPI_GPIO_Init(void)
{
	/* -GPF1 OLED_CSn Output Init to High Disable NSS- */
	GPFCON &= ~(3<<(1*2));
	GPFCON |=  (1<<(1*2));
	
	GPFDAT |=  (1<<1);
	
	/* -GPG2 FLASH_CSn Output Init to High Disable NSS- */
	GPGCON &= ~(3<<(2*2));
	GPGCON |=  (1<<(2*2));
	
	GPGDAT |=  (1<<2);
	
	/* -GPG5 SPI_MISO Input -
	 * -GPG6 SPI_MOSI Output-
	 * -GPG7 SPI_CLK  Output-
	 */
	GPGCON &= ~((3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
	GPGCON |=  ((0<<(5*2)) | (1<<(6*2)) | (1<<(7*2)));
}

/**********************************************************************************************************
 @Function			static void S3C2440_SPI_GPIO_SET_CLK(bool val)
 @Description			S3C2440_SPI_GPIO_SET_CLK
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_SPI_GPIO_SET_CLK(bool val)
{
	if (val)
		GPGDAT |=  (1<<7);
	else
		GPGDAT &= ~(1<<7);
}

/**********************************************************************************************************
 @Function			static void S3C2440_SPI_GPIO_SET_DO(bool val)
 @Description			S3C2440_SPI_GPIO_SET_DO
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_SPI_GPIO_SET_DO(bool val)
{
	if (val)
		GPGDAT |=  (1<<6);
	else
		GPGDAT &= ~(1<<6);
}

/**********************************************************************************************************
 @Function			static bool S3C2440_SPI_GPIO_GET_DI(void)
 @Description			S3C2440_SPI_GPIO_GET_DI
 @Input				void
 @Return				val
**********************************************************************************************************/
static bool S3C2440_SPI_GPIO_GET_DI(void)
{
	if (GPGDAT & (1<<5))
		return 1;
	else
		return 0;
}

/**********************************************************************************************************
 @Function			void S3C2440_SPI_GPIOInitialized(void)
 @Description			S3C2440_SPI_GPIOInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_SPI_GPIOInitialized(void)
{
	S3C2440_SPI_GPIO_Init();
}

/**********************************************************************************************************
 @Function			unsigned char S3C2440_SPI_GPIOSendByte(unsigned char val)
 @Description			S3C2440_SPI_GPIOSendByte
 @Input				val
 @Return				void
**********************************************************************************************************/
unsigned char S3C2440_SPI_GPIOSendByte(unsigned char val)
{
	for (int i = 0; i < 8; i++) {
		S3C2440_SPI_GPIO_SET_CLK(0);
		S3C2440_SPI_GPIO_SET_DO(val & 0x80);
		S3C2440_SPI_GPIO_SET_CLK(1);
		val <<= 1;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			unsigned char S3C2440_SPI_GPIORecvByte(void)
 @Description			S3C2440_SPI_GPIORecvByte
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char S3C2440_SPI_GPIORecvByte(void)
{
	unsigned char val = 0;
	
	for (int i = 0; i < 8; i++) {
		val <<= 1;
		S3C2440_SPI_GPIO_SET_CLK(0);
		if (S3C2440_SPI_GPIO_GET_DI()) val |= 1;
		S3C2440_SPI_GPIO_SET_CLK(1);
	}
	
	return val;
}

/********************************************** END OF FLEE **********************************************/
