#ifndef __S3C2440_TIMER_H
#define   __S3C2440_TIMER_H

#include "s3c2440_soc.h"

typedef void(*S3C2440_Timer0_IRQ_Function)(void);								//声明定时器0中断执行函数

typedef struct S3C2440_Timer0_Desc
{
	char*					name;
	S3C2440_Timer0_IRQ_Function	fp;
}S3C2440_Timer0_Desc, *p_S3C2440_Timer0_Desc;								//声明定时器0中断执行函数结构体

void S3C2440_TIMER0_Init(void);											//初始化Timer0定时器

int  S3C2440_TIMER0_Register(char* name, S3C2440_Timer0_IRQ_Function fp);			//注册定时器0中断执行函数
int  S3C2440_TIMER0_Unregister(char* name);									//注销定时器0中断执行函数

void S3C2440_TIMER0_Irq(unsigned int irqBit);								//Timer0定时器中断处理函数

void S3C2440_TIMER0_LED_Irq_Demo(void);										//Timer0定时器中断处理函数Demo

#endif /* __S3C2440_TIMER_H */
