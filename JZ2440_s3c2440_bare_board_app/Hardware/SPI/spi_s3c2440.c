/**
  *********************************************************************************************************
  * @file    spi_s3c2440.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-07-08
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "spi_s3c2440.h"

/**********************************************************************************************************
 @Function			static void S3C2440_SPI_Ctrl_GPIO_Init(void)
 @Description			S3C2440_SPI_Ctrl_GPIO_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_SPI_Ctrl_GPIO_Init(void)
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
	GPGCON |=  ((3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
}

/**********************************************************************************************************
 @Function			static void S3C2440_SPI_Ctrl_Controller_Init(void)
 @Description			S3C2440_SPI_Ctrl_Controller_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_SPI_Ctrl_Controller_Init(void)
{
	/* -OLED   : 100ns, 10MHz-
	 * -W25QXX : 10ns, 104MHz-
	 * -10 = 50 / 2 / (Prescaler value + 1)-
	 * -Prescaler value = 1.5 = 2-
	 * -Baud rate = 50 / 2 / 3 = 8.3MHz-
	 */
	SPPRE0 = 2;
	SPPRE1 = 2;
	
	/* -[6:5] : 00, polling mode-
	 * -[4:4] : 1 = Enable-
	 * -[3:3] : 1 = Master-
	 * -[2:2] : 0-
	 * -[1:1] : 0 = format A-
	 * -[0:0] : 0 = normal mode-
	 */
	SPCON0 = (1<<4) | (1<<3);
	SPCON1 = (1<<4) | (1<<3);
}

/**********************************************************************************************************
 @Function			void S3C2440_SPI_CtrlInitialized(void)
 @Description			S3C2440_SPI_CtrlInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_SPI_CtrlInitialized(void)
{
	S3C2440_SPI_Ctrl_GPIO_Init();
	
	S3C2440_SPI_Ctrl_Controller_Init();
}

/**********************************************************************************************************
 @Function			unsigned char S3C2440_SPI_CtrlSendByte(unsigned char val)
 @Description			S3C2440_SPI_CtrlSendByte
 @Input				val
 @Return				void
**********************************************************************************************************/
unsigned char S3C2440_SPI_CtrlSendByte(unsigned char val)
{
	while (!(SPSTA1 & 0x01));
	
	SPTDAT1 = val;
	
	return 0;
}

/**********************************************************************************************************
 @Function			unsigned char S3C2440_SPI_CtrlRecvByte(void)
 @Description			S3C2440_SPI_CtrlRecvByte
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char S3C2440_SPI_CtrlRecvByte(void)
{
	SPTDAT1 = 0xFF;
	
	while (!(SPSTA1 & 0x01));
	
	return SPRDAT1;
}

/********************************************** END OF FLEE **********************************************/
