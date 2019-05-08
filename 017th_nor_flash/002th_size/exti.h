#ifndef __S3C2440_EXTI_H
#define   __S3C2440_EXTI_H

#include "s3c2440_soc.h"

void S3C2440_EXTI_Init(void);												//外部中断初始化
void S3C2440_EXTI_Irq(unsigned int irqBit);									//外部中断处理函数

#endif /* __S3C2440_EXTI_H */
