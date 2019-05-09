#ifndef __S3C2440_INTERRUPT_H
#define   __S3C2440_INTERRUPT_H

#include "s3c2440_soc.h"

typedef void(*S3C2440_IRQ_Function)(unsigned int);							//声明中断执行函数

#if 0
void S3C2440_IRQInitialized(void);											//中断控制器初始化
#endif

void S3C2440_IRQRegister(unsigned char irqChannel, S3C2440_IRQ_Function fd);		//注册中断源
void S3C2440_IRQHandler(void);											//中断处理函数

#endif /* __S3C2440_INTERRUPT_H */
