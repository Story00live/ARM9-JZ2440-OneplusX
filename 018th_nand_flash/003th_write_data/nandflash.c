/**
  *********************************************************************************************************
  * @file    nandflash.c
  * @author  Devilhart -- KK
  * @version V0.0.2
  * @date    2019-04-05
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "nandflash.h"

/**********************************************************************************************************
 @Function			void S3C2440_NandFlashInitialized(void)
 @Description			S3C2440_NandFlashInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlashInitialized(void)
{
	/* -设置NAND FLASH时序- */
	NFCONF = (0<<12) | (1<<8) | (0<<4);
	
	/* -使能NAND FLASH控制器, 初始化ECC, 禁止片选- */
	NFCONT = (1<<4) | (1<<1) | (1<<0);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlashSelect(void)
 @Description			S3C2440_NandFlashSelect						: 使能Nand Flash片选
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlashSelect(void)
{
	NFCONT &= ~(1<<1);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlashDeselect(void)
 @Description			S3C2440_NandFlashDeselect					: 禁止Nand Flash片选
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlashDeselect(void)
{
	NFCONT |=  (1<<1);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Command(unsigned char cmmd)
 @Description			S3C2440_NandFlash_Write_Command				: Nand Flash 写入指令
 @Input				cmmd										: 指令值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Command(unsigned char cmmd)
{
	NFCMMD = cmmd;
	for (volatile int i = 0; i < 10; i++);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Address(unsigned char addr)
 @Description			S3C2440_NandFlash_Write_Address				: Nand Flash 写入地址
 @Input				addr										: 地址值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Address(unsigned char addr)
{
	NFADDR = addr;
	for (volatile int i = 0; i < 10; i++);
}

/**********************************************************************************************************
 @Function			unsigned char S3C2440_NandFlash_Read_Data(void)
 @Description			S3C2440_NandFlash_Read_Data					: Nand Flash 读取数据
 @Input				void
 @Return				val										: 数据值
**********************************************************************************************************/
unsigned char S3C2440_NandFlash_Read_Data(void)
{
	return NFDATA;
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Data(unsigned char val)
 @Description			S3C2440_NandFlash_Write_Data					: Nand Flash 写入数据
 @Input				val										: 数据值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Data(unsigned char val)
{
	NFDATA = val;
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Wait_Ready(void)
 @Description			S3C2440_NandFlash_Wait_Ready					: Nand Flash 等待就绪
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Wait_Ready(void)
{
	while (!(NFSTAT & 1));
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Data_Col_Address(unsigned int col)
 @Description			S3C2440_NandFlash_Write_Data_Col_Address		: Nand Flash 写入数据列地址
 @Input				col										: 列地址值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Data_Col_Address(unsigned int col)
{
	/* -发出Column Address- */
	S3C2440_NandFlash_Write_Address((col >>  0) & 0xFF);
	S3C2440_NandFlash_Write_Address((col >>  8) & 0xFF);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Data_Row_Address(unsigned int row)
 @Description			S3C2440_NandFlash_Write_Data_Row_Address		: Nand Flash 写入数据行地址
 @Input				row										: 行地址值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Data_Row_Address(unsigned int row)
{
	/* -发出Row Address- */
	S3C2440_NandFlash_Write_Address((row >>  0) & 0xFF);
	S3C2440_NandFlash_Write_Address((row >>  8) & 0xFF);
	S3C2440_NandFlash_Write_Address((row >> 16) & 0xFF);
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write_Data_Address(unsigned int addr)
 @Description			S3C2440_NandFlash_Write_Data_Address			: Nand Flash 写入数据地址
 @Input				addr										: 地址值
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write_Data_Address(unsigned int addr)
{
	unsigned int nandFlashRow = addr / 2048;
	unsigned int nandFlashCol = addr % 2048;
	
	/* -发出Column Address- */
	S3C2440_NandFlash_Write_Data_Col_Address(nandFlashCol);
	/* -发出Row Address- */
	S3C2440_NandFlash_Write_Data_Row_Address(nandFlashRow);
}

/**********************************************************************************************************
 @Function			int S3C2440_NandFlash_Check_BadBlock(unsigned int addr)
 @Description			S3C2440_NandFlash_Check_BadBlock				: Nand Flash 检查坏块
 @Input				addr										: 地址值
 @Return				BadBlock									: 1 坏块
															  0 好块 
**********************************************************************************************************/
int S3C2440_NandFlash_Check_BadBlock(unsigned int addr)
{
	unsigned int nandFlashRow = addr / 2048;
	unsigned int nandFlashCol =        2048;
	unsigned char nandFlashBadBlockOOBVal;
	
	/* -使能Nand Flash片选- */
	S3C2440_NandFlashSelect();
	/* -发送读数据命令- */
	S3C2440_NandFlash_Write_Command(0x00);
	/* -发出Column Address- */
	S3C2440_NandFlash_Write_Data_Col_Address(nandFlashCol);
	/* -发出Row Address- */
	S3C2440_NandFlash_Write_Data_Row_Address(nandFlashRow);
	/* -发送读数据确认命令- */
	S3C2440_NandFlash_Write_Command(0x30);
	/* -等待就绪- */
	S3C2440_NandFlash_Wait_Ready();
	/* -读取数据- */
	nandFlashBadBlockOOBVal = S3C2440_NandFlash_Read_Data();
	/* -禁止Nand Flash片选- */
	S3C2440_NandFlashDeselect();
	
	if (nandFlashBadBlockOOBVal != 0xFF) return 1;
	
	return 0;
}

/**********************************************************************************************************
 @Function			int S3C2440_NandFlash_Erase(unsigned int addr, unsigned int len)
 @Description			S3C2440_NandFlash_Erase						: Nand Flash 擦除
 @Input				addr										: 擦除地址
					len										: 擦除长度
 @Return				0										: True
					-1										: Error
**********************************************************************************************************/
int S3C2440_NandFlash_Erase(unsigned int addr, unsigned int len)
{
	unsigned int nandFlashRow = addr / 2048;
	
	/* -检查地址与长度是否块对齐- */
	if ((addr & 0x0001FFFF) || (len & 0x0001FFFF)) return -1;
	
	/* -使能Nand Flash片选- */
	S3C2440_NandFlashSelect();
	
	while (len >= (128*1024)) {
		/* -获取页地址- */
		nandFlashRow = addr / 2048;
		/* -发送擦除命令- */
		S3C2440_NandFlash_Write_Command(0x60);
		/* -发出Row Address- */
		S3C2440_NandFlash_Write_Data_Row_Address(nandFlashRow);
		/* -发送擦除确认命令- */
		S3C2440_NandFlash_Write_Command(0xD0);
		
		/* -等待就绪- */
		S3C2440_NandFlash_Wait_Ready();
		
		len  -= (128*1024);
		addr += (128*1024);
	}
	
	/* -禁止Nand Flash片选- */
	S3C2440_NandFlashDeselect();
	
	return 0;
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len)
 @Description			S3C2440_NandFlash_Read						: Nand Flash 读取
 @Input				addr										: 读取地址
					buf										: 读取数据存放地址
					len										: 读取长度
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len)
{
	unsigned int nandFlashReadIndex = 0;
	unsigned int nandFlashRow = addr / 2048;
	unsigned int nandFlashCol = addr % 2048;
	
	while (nandFlashReadIndex < len) {
		/* -检查坏块, 出厂时每块的第一页的2048地址存放坏块信息, 不为0xFF为坏块- */
		if (!(addr & 0x0001FFFF) && S3C2440_NandFlash_Check_BadBlock(addr)) {
			/* -坏块, 跳过当前Block- */
			addr += (128 * 1024);
			continue;
		}
		
		/* -使能Nand Flash片选- */
		S3C2440_NandFlashSelect();
		
		/* -发送读数据命令- */
		S3C2440_NandFlash_Write_Command(0x00);
		/* -发送数据地址- */
		S3C2440_NandFlash_Write_Data_Address(addr);
		/* -发送读数据确认命令- */
		S3C2440_NandFlash_Write_Command(0x30);
		
		/* -等待就绪- */
		S3C2440_NandFlash_Wait_Ready();
		
		/* -读取数据- */
		for (; (nandFlashCol < 2048) && (nandFlashReadIndex < len); nandFlashCol++) {
			buf[nandFlashReadIndex++] = S3C2440_NandFlash_Read_Data();
			addr++;
		}
		if (nandFlashReadIndex == len) break;
		
		nandFlashCol = 0;
		nandFlashRow++;
		
		/* -禁止Nand Flash片选- */
		S3C2440_NandFlashDeselect();
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len)
 @Description			S3C2440_NandFlash_Write						: Nand Flash 写入
 @Input				addr										: 写入地址
					buf										: 写入数据存放地址
					len										: 写入长度
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len)
{
	unsigned int nandFlashWriteIndex = 0;
	unsigned int nandFlashRow = addr / 2048;
	unsigned int nandFlashCol = addr % 2048;
	
	while (nandFlashWriteIndex < len) {
		/* -检查坏块, 出厂时每块的第一页的2048地址存放坏块信息, 不为0xFF为坏块- */
		if (!(addr & 0x0001FFFF) && S3C2440_NandFlash_Check_BadBlock(addr)) {
			/* -坏块, 跳过当前Block- */
			addr += (128 * 1024);
			continue;
		}
		
		/* -使能Nand Flash片选- */
		S3C2440_NandFlashSelect();
		
		/* -发送写数据命令- */
		S3C2440_NandFlash_Write_Command(0x80);
		/* -发送数据地址- */
		S3C2440_NandFlash_Write_Data_Address(addr);
		
		/* -写入数据- */
		for (; (nandFlashCol < 2048) && (nandFlashWriteIndex < len); nandFlashCol++) {
			S3C2440_NandFlash_Write_Data(buf[nandFlashWriteIndex++]);
			addr++;
		}
		
		/* -发送写数据确认命令- */
		S3C2440_NandFlash_Write_Command(0x10);
		
		/* -等待就绪- */
		S3C2440_NandFlash_Wait_Ready();
		
		if (nandFlashWriteIndex == len) break;
		
		nandFlashCol = 0;
		nandFlashRow++;
		
		/* -禁止Nand Flash片选- */
		S3C2440_NandFlashDeselect();
	}
}

/********************************************** END OF FLEE **********************************************/
