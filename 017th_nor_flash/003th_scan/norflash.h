#ifndef __S3C2440_NOR_FLASH_H
#define   __S3C2440_NOR_FLASH_H

#include "s3c2440_soc.h"

#define	NOR_FLASH_BASE_ADDR				0x00000000
#define	NOR_FLASH_BYTE_SIZE				0x00200000

void S3C2440_NorFlashInitialized(void);										//NorFlash初始化

void S3C2440_NorFlashTest(void);											//NorFlash测试

#endif /* __S3C2440_NOR_FLASH_H */
