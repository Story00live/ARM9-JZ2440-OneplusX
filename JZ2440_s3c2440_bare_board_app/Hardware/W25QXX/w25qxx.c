/**
  *********************************************************************************************************
  * @file    w25qxx.c
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

#include "w25qxx.h"
#include "spi_gpio.h"

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_SET_CS(bool val)
 @Description			S3C2440_W25QXX_SET_CS
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_SET_CS(bool val)
{
	/* -GPG2 FLASH_CS Output for Chip Select- */
	if (val)
		GPGDAT |=  (1<<2);
	else
		GPGDAT &= ~(1<<2);
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_SendAddr(unsigned int addr)
 @Description			S3C2440_W25QXX_SendAddr
 @Input				addr
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_SendAddr(unsigned int addr)
{
	S3C2440_SPI_GPIOSendByte((addr >> 16) & 0xFF);
	S3C2440_SPI_GPIOSendByte((addr >> 8 ) & 0xFF);
	S3C2440_SPI_GPIOSendByte((addr >> 0 ) & 0xFF);
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_WriteEnable(bool enable)
 @Description			S3C2440_W25QXX_WriteEnable
 @Input				enable
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_WriteEnable(bool enable)
{
	if (enable) {
		/* -Select W25QXX NSS- */
		S3C2440_W25QXX_SET_CS(0);
		/* -Send Byte- */
		S3C2440_SPI_GPIOSendByte(0x06);
		/* -De-Select W25QXX NSS- */
		S3C2440_W25QXX_SET_CS(1);
	}
	else {
		/* -Select W25QXX NSS- */
		S3C2440_W25QXX_SET_CS(0);
		/* -Send Byte- */
		S3C2440_SPI_GPIOSendByte(0x04);
		/* -De-Select W25QXX NSS- */
		S3C2440_W25QXX_SET_CS(1);
	}
}

/**********************************************************************************************************
 @Function			static unsigned char S3C2440_W25QXX_ReadStatusReg1(void)
 @Description			S3C2440_W25QXX_ReadStatusReg1
 @Input				void
 @Return				StatusReg1
**********************************************************************************************************/
static unsigned char S3C2440_W25QXX_ReadStatusReg1(void)
{
	unsigned char val;
	
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x05);
	val = S3C2440_SPI_GPIORecvByte();
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
	
	return val;
}

/**********************************************************************************************************
 @Function			static unsigned char S3C2440_W25QXX_ReadStatusReg2(void)
 @Description			S3C2440_W25QXX_ReadStatusReg2
 @Input				void
 @Return				StatusReg2
**********************************************************************************************************/
static unsigned char S3C2440_W25QXX_ReadStatusReg2(void)
{
	unsigned char val;
	
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x35);
	val = S3C2440_SPI_GPIORecvByte();
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
	
	return val;
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_WaitWhenBusy(void)
 @Description			S3C2440_W25QXX_WaitWhenBusy
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_WaitWhenBusy(void)
{
	while (S3C2440_W25QXX_ReadStatusReg1() & 0x01);
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_WriteStatusReg(unsigned char reg1, unsigned char reg2)
 @Description			S3C2440_W25QXX_WriteStatusReg
 @Input				reg1
					reg2
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_WriteStatusReg(unsigned char reg1, unsigned char reg2)
{
	S3C2440_W25QXX_WriteEnable(1);
	
	S3C2440_W25QXX_SET_CS(0);
	S3C2440_SPI_GPIOSendByte(0x01);
	S3C2440_SPI_GPIOSendByte(reg1);
	S3C2440_SPI_GPIOSendByte(reg2);
	S3C2440_W25QXX_SET_CS(1);
	
	S3C2440_W25QXX_WaitWhenBusy();
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_ClearProtectForStatusReg(void)
 @Description			S3C2440_W25QXX_ClearProtectForStatusReg
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_ClearProtectForStatusReg(void)
{
	unsigned char reg1, reg2;
	
	reg1 = S3C2440_W25QXX_ReadStatusReg1();
	reg2 = S3C2440_W25QXX_ReadStatusReg2();
	
	reg1 &= ~(1<<7);
	reg2 &= ~(1<<0);
	
	S3C2440_W25QXX_WriteStatusReg(reg1, reg2);
}

/**********************************************************************************************************
 @Function			static void S3C2440_W25QXX_ClearProtectForData(void)
 @Description			S3C2440_W25QXX_ClearProtectForData
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_W25QXX_ClearProtectForData(void)
{
	unsigned char reg1, reg2;
	
	reg1 = S3C2440_W25QXX_ReadStatusReg1();
	reg2 = S3C2440_W25QXX_ReadStatusReg2();
	
	reg1 &= ~(7<<2);
	reg2 &= ~(1<<6);
	
	S3C2440_W25QXX_WriteStatusReg(reg1, reg2);
}

/**********************************************************************************************************
 @Function			void S3C2440_W25QxxInitialized(void)
 @Description			S3C2440_W25QxxInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_W25QxxInitialized(void)
{
	S3C2440_SPI_GPIOInitialized();
	
	S3C2440_W25QXX_ClearProtectForStatusReg();
	S3C2440_W25QXX_ClearProtectForData();
}

/**********************************************************************************************************
 @Function			void S3C2440_W25QxxReadID(int* pMID, int* pDID)
 @Description			S3C2440_W25QxxReadID
 @Input				pMID
					pDID
 @Return				void
**********************************************************************************************************/
void S3C2440_W25QxxReadID(int* pMID, int* pDID)
{
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x90);
	
	S3C2440_W25QXX_SendAddr(0x00);
	
	*pMID = S3C2440_SPI_GPIORecvByte();
	*pDID = S3C2440_SPI_GPIORecvByte();
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
}

/**********************************************************************************************************
 @Function			void S3C2440_W25QxxEraseSector(unsigned int addr)
 @Description			S3C2440_W25QxxEraseSector
 @Input				addr
 @Return				void
**********************************************************************************************************/
void S3C2440_W25QxxEraseSector(unsigned int addr)
{
	S3C2440_W25QXX_WriteEnable(1);
	
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x20);
	
	S3C2440_W25QXX_SendAddr(addr);
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
	
	S3C2440_W25QXX_WaitWhenBusy();
}

/**********************************************************************************************************
 @Function			void S3C2440_W25QxxProgram(unsigned int addr, unsigned char* buf, int len)
 @Description			S3C2440_W25QxxProgram
 @Input				addr
					buf
					len
 @Return				void
**********************************************************************************************************/
void S3C2440_W25QxxProgram(unsigned int addr, unsigned char* buf, int len)
{
	
	S3C2440_W25QXX_WriteEnable(1);
	
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x02);
	
	S3C2440_W25QXX_SendAddr(addr);
	
	for (int i = 0; i < len; i++) {
		S3C2440_SPI_GPIOSendByte(buf[i]);
	}
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
	
	S3C2440_W25QXX_WaitWhenBusy();
}

/**********************************************************************************************************
 @Function			void S3C2440_W25QxxRead(unsigned int addr, unsigned char* buf, int len)
 @Description			S3C2440_W25QxxRead
 @Input				addr
					buf
					len
 @Return				void
**********************************************************************************************************/
void S3C2440_W25QxxRead(unsigned int addr, unsigned char* buf, int len)
{
	/* -Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(0);
	
	S3C2440_SPI_GPIOSendByte(0x03);
	
	S3C2440_W25QXX_SendAddr(addr);
	
	for (int i = 0; i < len; i++) {
		buf[i] = S3C2440_SPI_GPIORecvByte();
	}
	
	/* -De-Select W25QXX NSS- */
	S3C2440_W25QXX_SET_CS(1);
}

/********************************************** END OF FLEE **********************************************/
