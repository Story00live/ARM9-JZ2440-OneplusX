#ifndef __S3C2440_TIMER_H
#define   __S3C2440_TIMER_H

#include "s3c2440_soc.h"

typedef void(*S3C2440_Timer_IRQ_Function)(void);										//声明定时器中断执行函数

typedef struct S3C2440_Timer_Desc
{
	char*					name;
	unsigned char				irq;
	S3C2440_Timer_IRQ_Function	fp;
}S3C2440_Timer_Desc, *p_S3C2440_Timer_Desc;											//声明定时器中断执行函数结构体

void S3C2440_TIMER0_Init(void);													//初始化Timer0定时器
void S3C2440_TIMER1_Init(void);													//初始化Timer1定时器

int  S3C2440_TIMER_Register(char* name, unsigned char irq, S3C2440_Timer_IRQ_Function fp);	//注册定时器中断执行函数
int  S3C2440_TIMER_Unregister(char* name);											//注销定时器中断执行函数

void S3C2440_TIMER_Irq(unsigned int irqBit);											//Timer定时器中断处理函数

void S3C2440_TIMER0_uDelay(int unCount);											//基于Timer0延时微妙
void S3C2440_TIMER0_mDelay(int muCount);											//基于Timer0延时毫秒

unsigned long long S3C2440_TIMER0_GetSystem_TimeUs(void);								//获取系统运行时间微妙
unsigned long long S3C2440_TIMER0_Delta_TimeUs(unsigned long long pre, unsigned long long now);//计算两个时间差值微妙

void S3C2440_TIMER0_System_10ms_Irq(void);											//系统运行时间计数器
void S3C2440_TIMER0_LED_Irq_Demo(void);												//Timer0定时器中断处理函数Demo
void S3C2440_TIMER1_LED_Irq_Demo(void);												//Timer1定时器中断处理函数Demo

#endif /* __S3C2440_TIMER_H */
