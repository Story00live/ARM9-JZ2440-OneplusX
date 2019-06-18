/**
  *********************************************************************************************************
  * @file    norflash.c
  * @author  Devilhart -- KK
  * @version V0.0.2
  * @date    2019-03-14
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					Nor Flash 数据总线16位
  *					Nor Flash 地址总线20位
  *
  *********************************************************************************************************
  */

#include "norflash.h"

/**********************************************************************************************************
 @Function			void S3C2440_NorFlashInitialized(void)
 @Description			S3C2440_NorFlashInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlashInitialized(void)
{
	BANKCON0 |=  (7<<8);
	BANKCON0 &= ~(2<<8);
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_ChipWrite_HalfWord(unsigned int base, unsigned int offset, unsigned short val)
 @Description			S3C2440_NorFlash_ChipWrite_HalfWord			: Nor Flash Chip 写1个半字
 @Input				base										: 基地址
					offset									: 偏移量
					val										: 写入值
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_ChipWrite_HalfWord(unsigned int base, unsigned int offset, unsigned short val)
{
	/* -offset << 1 : Nor Flash 地址0位对应 S3C2440 地址1位, 所以 S3C2440 偏移地址左移1位- */
	volatile unsigned short* pNorFlashAddr = (volatile unsigned short*)(base + (offset << 1));
	
	*pNorFlashAddr = val;
}

/**********************************************************************************************************
 @Function			unsigned short S3C2440_NorFlash_ChipRead_HalfWord(unsigned int base, unsigned int offset)
 @Description			S3C2440_NorFlash_ChipRead_HalfWord				: Nor Flash Chip 读1个半字
 @Input				base										: 基地址
					offset									: 偏移量
 @Return				val										: 读取值
**********************************************************************************************************/
unsigned short __init S3C2440_NorFlash_ChipRead_HalfWord(unsigned int base, unsigned int offset)
{
	/* -offset << 1 : Nor Flash 地址0位对应 S3C2440 地址1位, 所以 S3C2440 偏移地址左移1位- */
	volatile unsigned short* pNorFlashAddr = (volatile unsigned short*)(base + (offset << 1));
	
	return *pNorFlashAddr;
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_Write_Command(unsigned int offset, unsigned short cmd)
 @Description			S3C2440_NorFlash_Write_Command				: Nor Flash 写入指令
 @Input				offset									: 偏移地址
					cmd										: 指令值
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_Write_Command(unsigned int offset, unsigned short cmd)
{
	S3C2440_NorFlash_ChipWrite_HalfWord(NOR_FLASH_CHIP_ADDR, offset, cmd);
}

/**********************************************************************************************************
 @Function			unsigned short S3C2440_NorFlash_Read_Data(unsigned int offset)
 @Description			S3C2440_NorFlash_Read_Data					: Nor Flash 读取数据
 @Input				offset									: 偏移地址
 @Return				val										: 数据值
**********************************************************************************************************/
unsigned short __init S3C2440_NorFlash_Read_Data(unsigned int offset)
{
	return S3C2440_NorFlash_ChipRead_HalfWord(NOR_FLASH_CHIP_ADDR, offset);
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_Wait_Ready(unsigned int addr)
 @Description			S3C2440_NorFlash_Wait_Ready					: Nor Flash 等待空闲
 @Input				addr
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_Wait_Ready(unsigned int addr)
{
	unsigned int val;
	unsigned int pre;
	
	pre = S3C2440_NorFlash_Read_Data(addr);
	val = S3C2440_NorFlash_Read_Data(addr);
	
	while ((val & (1<<6)) != (pre & (1<<6))) {
		pre = val;
		val = S3C2440_NorFlash_Read_Data(addr);
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_Erase(unsigned int addr)
 @Description			S3C2440_NorFlash_Erase						: Nor Flash 擦除
 @Input				addr										: 擦除地址
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_Erase(unsigned int addr)
{
	/* -解锁- */
	S3C2440_NorFlash_Write_Command(0x0555, 0xAA);
	S3C2440_NorFlash_Write_Command(0x02AA, 0x55);
	S3C2440_NorFlash_Write_Command(0x0555, 0x80);							// 擦除扇区
	
	/* -解锁- */
	S3C2440_NorFlash_Write_Command(0x0555, 0xAA);
	S3C2440_NorFlash_Write_Command(0x02AA, 0x55);
	S3C2440_NorFlash_Write_Command(addr >> 1, 0x30);							// 发出扇区地址
	
	/* -等待擦除完成- */
	S3C2440_NorFlash_Wait_Ready(addr >> 1);
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len)
 @Description			S3C2440_NorFlash_Read						: Nor Flash 读取
 @Input				addr										: 读取地址
					buf										: 读取数据存放地址
					len										: 读取长度
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len)
{
	unsigned int readIndex = 0;
	unsigned int readAddress;
	volatile unsigned char* pReadAddress;
	
	readAddress = addr;
	pReadAddress = (volatile unsigned char*)readAddress;
	
	while (len--) {
		buf[readIndex++] = *pReadAddress++;
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_NorFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len)
 @Description			S3C2440_NorFlash_Write						: Nor Flash 写入
 @Input				addr										: 写入地址
					buf										: 写入数据存放地址
					len										: 写入长度
 @Return				void
**********************************************************************************************************/
void __init S3C2440_NorFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len)
{
	unsigned int writeAddress;
	unsigned int highAddr = 1, lowAddr = 0;
	unsigned int writeVal = 0;
	
	writeAddress = addr;
	
	while (buf[lowAddr] && buf[highAddr]) {
		writeVal = buf[lowAddr] + (buf[highAddr] << 8);
		
		/* -解锁- */
		S3C2440_NorFlash_Write_Command(0x0555, 0xAA);
		S3C2440_NorFlash_Write_Command(0x02AA, 0x55);
		S3C2440_NorFlash_Write_Command(0x0555, 0xA0);						// 写入
		S3C2440_NorFlash_Write_Command(writeAddress >> 1, writeVal);			// 发出需写入数据
		
		/* -等待烧写完成- */
		S3C2440_NorFlash_Wait_Ready(writeAddress >> 1);
		
		highAddr     += 2;
		lowAddr      += 2;
		writeAddress += 2;
	}
	
	writeVal = buf[lowAddr];
	
	/* -解锁- */
	S3C2440_NorFlash_Write_Command(0x0555, 0xAA);
	S3C2440_NorFlash_Write_Command(0x02AA, 0x55);
	S3C2440_NorFlash_Write_Command(0x0555, 0xA0);							// 写入
	S3C2440_NorFlash_Write_Command(writeAddress >> 1, writeVal);				// 发出需写入数据
	
	/* -等待烧写完成- */
	S3C2440_NorFlash_Wait_Ready(writeAddress >> 1);
}

/********************************************** END OF FLEE **********************************************/
