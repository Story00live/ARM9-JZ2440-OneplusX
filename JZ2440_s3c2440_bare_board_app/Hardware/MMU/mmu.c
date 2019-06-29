/**
  *********************************************************************************************************
  * @file    mmu.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-29
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *				<!创建一级页表>
  *				VA(虚拟地址)             PA(物理地址)             CB(Cache, Write Buff)
  *				
  *				0x00000000(+1MB)        0x00000000(+1MB)       00
  *				0x40000000(+1MB)        0x40000000(+1MB)       11
  *				
  *				64M SDRAM:              0x30000000 ~ 0x34000000
  *				0x30000000(+1MB)        0x30000000(+1MB)       11
  *				......
  *				0x33F00000(+1MB)        0x33F00000(+1MB)       11
  *				
  *				S342440 Register:       0x48000000 ~ 0x5B00001C
  *				0x48000000(+1MB)        0x48000000(+1MB)       00
  *				......
  *				0x5B000000(+1MB)        0x5B000000(+1MB)       00
  *				
  *				LCD Framebuffer:        0x33C00000 ~ 0x33D00000
  *				0x33C00000(+1MB)        0x33C00000(+1MB)       00
  *				
  *				Link Address:           0x80000000 ~ 0x80100000
  *				0x80000000(+1MB)        0x30000000(+1MB)       11
  *				
  *				
  *********************************************************************************************************
  */

#include "mmu.h"

#define MMU_SECDESC_AP             (3<<10)
#define MMU_SECDESC_DOMAIN         (0<<5)
#define MMU_SECDESC_NCNB           (0<<2)
#define MMU_SECDESC_YCYB           (3<<2)
#define MMU_SECDESC_TYPE           ((1<<4) | (1<<1))

#define MMU_SECDESC_FOR_IO         (MMU_SECDESC_AP | MMU_SECDESC_DOMAIN | MMU_SECDESC_NCNB | MMU_SECDESC_TYPE)
#define MMU_SECDESC_FOR_MEM        (MMU_SECDESC_AP | MMU_SECDESC_DOMAIN | MMU_SECDESC_YCYB | MMU_SECDESC_TYPE)

#define MMU_IO                     1
#define MMU_MEM                    0

/**********************************************************************************************************
 @Function			void S3C2440_MMU_CreatSecdesc(unsigned int* pageTab, unsigned int va, unsigned int pa, int io_mem)
 @Description			S3C2440_MMU_CreatSecdesc
 @Input				pageTab			: 页表地址
					va				: 虚拟地址
					pa				: 物理地址
					io_mem			: IO or MEM
 @Return				void
**********************************************************************************************************/
void __init S3C2440_MMU_CreatSecdesc(unsigned int* pageTab, unsigned int va, unsigned int pa, int io_mem)
{
	int index = va / 0x00100000;
	
	pageTab[index] = (io_mem) ? ((pa & 0xFFF00000) | MMU_SECDESC_FOR_IO) : ((pa & 0xFFF00000) | MMU_SECDESC_FOR_MEM);
}

/**********************************************************************************************************
 @Function			void S3C2440_MMU_CreatPageTable(void)
 @Description			S3C2440_MMU_CreatPageTable
 @Input				void
 @Return				void
**********************************************************************************************************/
void __init S3C2440_MMU_CreatPageTable(void)
{
	/* -1. 页表位置: 0x32000000(占据16KB)- */
	unsigned int* pageTab = (unsigned int*)0x32000000;
	
	/* -2. 根据va, pa设置页表条目- */
	unsigned int va, pa;
	
	/* 2.1 For SRAM / Nor Flash */
	va = 0x00000000;
	pa = 0x00000000;
	S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_IO);
	
	/* 2.2 For SRAM When Nor Boot */
	va = 0x40000000;
	pa = 0x40000000;
	S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_MEM);
	
	/* 2.3 For 64M SDRAM:              0x30000000 ~ 0x34000000*/
	va = 0x30000000;
	pa = 0x30000000;
	while (va < 0x34000000) {
		S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_MEM);
		va += 0x00100000;
		pa += 0x00100000;
	}
	
	/* 2.4 For S342440 Register:       0x48000000 ~ 0x5B00001C */
	va = 0x48000000;
	pa = 0x48000000;
	while (va <= 0x5B000000) {
		S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_IO);
		va += 0x00100000;
		pa += 0x00100000;
	}
	
	/* 2.5 For LCD Framebuffer:        0x33C00000 ~ 0x33D00000 */
	va = 0x33C00000;
	pa = 0x33C00000;
	S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_IO);
	
	/* 2.6 For Link Address:           0x80000000 ~ 0x80100000 */
	va = 0x80000000;
	pa = 0x30000000;
	S3C2440_MMU_CreatSecdesc(pageTab, va, pa, MMU_MEM);
}

/********************************************** END OF FLEE **********************************************/
