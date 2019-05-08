#ifndef __S3C2440_NOR_FLASH_H
#define   __S3C2440_NOR_FLASH_H

#include "s3c2440_soc.h"

#define	NOR_FLASH_CHIP_ADDR				0x00000000

#define	NOR_FLASH_BASE_ADDR				0x00000000
#define	NOR_FLASH_BYTE_SIZE				0x00200000

void S3C2440_NorFlashInitialized(void);														//NorFlash初始化

void S3C2440_NorFlash_ChipWrite_HalfWord(unsigned int base, unsigned int offset, unsigned short val);	//Nor Flash Chip 写1个半字

unsigned short S3C2440_NorFlash_ChipRead_HalfWord(unsigned int base, unsigned int offset);			//Nor Flash Chip 读1个半字

void S3C2440_NorFlash_Write_Command(unsigned int offset, unsigned short cmd);						//Nor Flash 写入指令

unsigned short S3C2440_NorFlash_Read_Data(unsigned int offset);									//Nor Flash 读取数据

void S3C2440_NorFlash_Wait_Ready(unsigned int addr);											//Nor Flash 等待空闲

void S3C2440_NorFlash_Erase(unsigned int addr);												//Nor Flash 擦除
void S3C2440_NorFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len);					//Nor Flash 读取
void S3C2440_NorFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len);				//Nor Flash 写入

#endif /* __S3C2440_NOR_FLASH_H */
