/**
  *********************************************************************************************************
  * @file    sdram.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-04
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "sdram.h"
#include "norflash.h"
#include "nandflash.h"

/**********************************************************************************************************
 @Function			void S3C2440_SDRAMInitialized(void)
 @Description			S3C2440_SDRAMInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_SDRAMInitialized(void)
{
	/* 设置SDRAM数据总线宽度为32bit */
	BWSCON &= ~((3<<24) | (1<<26) | (1<<27));
	BWSCON &= ~((3<<28) | (1<<30) | (1<<31));
	BWSCON |=  ((2<<24) | (2<<28));
	
	/* 设置BANK控制器为SDRAM,RAS/CAS延时2个Clock,列地址9bit */
	BANKCON6 &= ~((3<<15) | (3<<2) | (3<<0));
	BANKCON6 |=  ((3<<15) | (0<<2) | (1<<0));
	BANKCON7 &= ~((3<<15) | (3<<2) | (3<<0));
	BANKCON7 |=  ((3<<15) | (0<<2) | (1<<0));
	
	/* 设置REFRESH刷新控制寄存器 */
	REFRESH = 0x008404F5;
	
	/* 设置BANKSIZE寄存器 */
	BANKSIZE = 0xB1;
	
	/* 设置MRSR寄存器 */
	MRSRB6 = 0x20;
	MRSRB7 = 0x20;
}

/**********************************************************************************************************
 @Function			void S3C2440_SDRAM_CopyCodeData(void)
 @Description			S3C2440_SDRAM_CopyCodeData
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_SDRAM_CopyCodeData(void)
{
	/* lds文件中获取 __code_start __code_end 值 */
	extern unsigned int __code_start, __code_end;
	
	volatile unsigned int* dest_start = (volatile unsigned int*)&__code_start;
	volatile unsigned int* dest_end = (volatile unsigned int*)&__code_end;
	volatile unsigned int* src = (volatile unsigned int*)0x00000000;
	
	while (dest_start < dest_end) {
		*dest_start++ = *src++;
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_SDRAM_CleanBssData(void)
 @Description			S3C2440_SDRAM_CleanBssData
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_SDRAM_CleanBssData(void)
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
