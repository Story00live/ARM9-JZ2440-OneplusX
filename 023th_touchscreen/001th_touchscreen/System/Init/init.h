#ifndef __S3C2440_INIT_H
#define   __S3C2440_INIT_H

#include "s3c2440_soc.h"

void S3C2440_Initialized_NandFlash(void);											//初始化Nand Flash

void S3C2440_Initialized_NorFlash(void);											//初始化Nor Flash

void S3C2440_Initialized_SDRAM(void);												//初始化SDRAM

void S3C2440_Initialized_CopyCodeData(void);											//重定位text, rodata, data段

void S3C2440_Initialized_CleanBssData(void);											//清除bss段

#endif /* __S3C2440_INIT_H */
