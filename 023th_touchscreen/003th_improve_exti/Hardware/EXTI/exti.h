#ifndef __S3C2440_EXTI_H
#define   __S3C2440_EXTI_H

#include "s3c2440_soc.h"

typedef void(*S3C2440_EXTI_IRQ_Function)(void);								//声明外部中断执行函数

typedef struct S3C2440_EXTI_Desc
{
	char*					name;
	S3C2440_EXTI_IRQ_Function	fp;
}S3C2440_EXTI_Desc, *p_S3C2440_EXTI_Desc;									//声明外部中断执行函数结构体

void S3C2440_EXTI_Init(void);												//外部中断初始化

int  S3C2440_EXTI_Register(char* name, S3C2440_EXTI_IRQ_Function fp);				//注册外部中断源
int  S3C2440_EXTI_Unregister(char* name);									//注销外部中断源

void S3C2440_EXTI_Irq(unsigned int irqBit);									//外部中断处理函数

#endif /* __S3C2440_EXTI_H */
