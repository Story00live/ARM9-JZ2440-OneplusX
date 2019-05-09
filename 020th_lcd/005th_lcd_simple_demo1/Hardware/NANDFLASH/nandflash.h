#ifndef __S3C2440_NAND_FLASH_H
#define   __S3C2440_NAND_FLASH_H

#include "s3c2440_soc.h"

void S3C2440_NandFlashInitialized(void);													//Nand Flash初始化

void S3C2440_NandFlashReset(void);															//Nand Flash复位

void S3C2440_NandFlashSelect(void);														//使能Nand Flash片选
void S3C2440_NandFlashDeselect(void);														//禁止Nand Flash片选

void S3C2440_NandFlash_Write_Command(unsigned char cmmd);										//Nand Flash 写入指令
void S3C2440_NandFlash_Write_Address(unsigned char addr);										//Nand Flash 写入地址

unsigned char S3C2440_NandFlash_Read_Data(void);												//Nand Flash 读取数据
void S3C2440_NandFlash_Write_Data(unsigned char val);											//Nand Flash 写入数据

void S3C2440_NandFlash_Wait_Ready(void);													//Nand Flash 等待就绪

void S3C2440_NandFlash_Write_Data_Col_Address(unsigned int col);									//Nand Flash 写入数据列地址
void S3C2440_NandFlash_Write_Data_Row_Address(unsigned int row);									//Nand Flash 写入数据行地址
void S3C2440_NandFlash_Write_Data_Address(unsigned int addr);									//Nand Flash 写入数据地址

int S3C2440_NandFlash_Check_BadBlock(unsigned int addr);										//Nand Flash 检查坏块

int  S3C2440_NandFlash_Erase(unsigned int addr, unsigned int len);								//Nand Flash擦除
void S3C2440_NandFlash_Read(unsigned int addr, unsigned char* buf, unsigned int len);				//Nand Flash读取
void S3C2440_NandFlash_Write(unsigned int addr, unsigned char* buf, unsigned int len);				//Nand Flash写入

#endif /* __S3C2440_NAND_FLASH_H */
