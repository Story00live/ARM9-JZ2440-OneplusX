#ifndef __S3C2440_TIMER_H
#define   __S3C2440_TIMER_H

#include "s3c2440_soc.h"

void S3C2440_TIMER0_Init(void);											//初始化Timer0定时器
void S3C2440_TIMER0_Irq(unsigned int irqBit);								//Timer0定时器中断处理函数

#endif /* __S3C2440_TIMER_H */
