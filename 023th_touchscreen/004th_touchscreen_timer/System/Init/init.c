/**
  *********************************************************************************************************
  * @file    init.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-04-07
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "init.h"
#include "norflash.h"
#include "nandflash.h"
#include "sdram.h"

/**********************************************************************************************************
 @Function			void S3C2440_Initialized_NandFlash(void)
 @Description			S3C2440_Initialized_NandFlash					: 初始化Nand Flash
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Initialized_NandFlash(void)
{
	S3C2440_NandFlashInitialized();
}

/**********************************************************************************************************
 @Function			void S3C2440_Initialized_NorFlash(void)
 @Description			S3C2440_Initialized_NorFlash					: 初始化Nor Flash
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Initialized_NorFlash(void)
{
	S3C2440_NorFlashInitialized();
}

/**********************************************************************************************************
 @Function			void S3C2440_Initialized_SDRAM(void)
 @Description			S3C2440_Initialized_SDRAM					: 初始化SDRAM
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Initialized_SDRAM(void)
{
	S3C2440_SDRAMInitialized();
}

/**********************************************************************************************************
 @Function			static int S3C2440_Initialized_CheckBootFromNorFlashOrNandFlash(void)
 @Description			S3C2440_Initialized_CheckBootFromNorFlashOrNandFlash
 @Input				void
 @Return				0						: Nand Flash
					1						: Nor  Flash
**********************************************************************************************************/
static int S3C2440_Initialized_CheckBootFromNorFlashOrNandFlash(void)
{
	volatile unsigned int* chipOnZeroAddr = (volatile unsigned int*)0x00000000;
	unsigned int chipOnZeroAddrVal = *chipOnZeroAddr;
	
	*chipOnZeroAddr = 0x12345678;
	if (*chipOnZeroAddr == 0x12345678) {
		/* 写入成功, 0x00000000对应片上SRAM起始地址, 为Nand Flash启动 */
		*chipOnZeroAddr = chipOnZeroAddrVal;
		return 0;
	}
	else {
		/* 写入失败, 0x00000000对应片外Nor Flash基地址, 为Nor Flash启动 */
		return 1;
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_Initialized_CopyCodeData(void)
 @Description			S3C2440_Initialized_CopyCodeData				: 重定位text, rodata, data段
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Initialized_CopyCodeData(void)
{
	/* lds文件中获取 __code_start __code_end 值 */
	extern unsigned int __code_start, __code_end;
	
	volatile unsigned int* dest_start = (volatile unsigned int*)&__code_start;
	volatile unsigned int* dest_end = (volatile unsigned int*)&__code_end;
	volatile unsigned int* src = (volatile unsigned int*)0x00000000;
	
	unsigned int len = ((int)&__code_end) - ((int)&__code_start);
	
	if (S3C2440_Initialized_CheckBootFromNorFlashOrNandFlash()) {
		/* Nor Flash Boot */
		S3C2440_NorFlash_Read((unsigned int)src, (unsigned char*)dest_start, len);
	}
	else {
		/* Nand Flash Boot */
		S3C2440_NandFlash_Read((unsigned int)src, (unsigned char*)dest_start, len);
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_Initialized_CleanBssData(void)
 @Description			S3C2440_Initialized_CleanBssData				: 清除bss段
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Initialized_CleanBssData(void)
{
	/* lds文件中获取 __bss_start __bss_end 值 */
	extern unsigned int __bss_start, __bss_end;
	
	volatile unsigned int* bss_start = (volatile unsigned int*)&__bss_start;
	volatile unsigned int* bss_end = (volatile unsigned int*)&__bss_end;
	
	while (bss_start <= bss_end) {
		*bss_start++ = 0x00000000;
	}
}

/********************************************** END OF FLEE **********************************************/
