#ifndef __S3C2440_SDRAM_H
#define   __S3C2440_SDRAM_H

#include "s3c2440_soc.h"

void S3C2440_SDRAMInitialized(void);										//SDRAM初始化

void S3C2440_SDRAM_CopyCodeData(void);										//SDRAM拷贝程序到SDRAM中
void S3C2440_SDRAM_CleanBssData(void);										//SDRAM清除SDRAM中BSS段数据

#endif /* __S3C2440_SDRAM_H */
