#ifndef __S3C2440_INTERRUPT_H
#define   __S3C2440_INTERRUPT_H

#include "s3c2440_soc.h"

void S3C2440_IRQInitialized(void);											//中断控制器初始化
void S3C2440_IRQHandler(void);											//中断处理函数

#endif /* __S3C2440_INTERRUPT_H */
