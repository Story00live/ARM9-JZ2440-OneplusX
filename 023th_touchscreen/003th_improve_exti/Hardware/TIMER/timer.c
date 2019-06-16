/**
  *********************************************************************************************************
  * @file    timer.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-11
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "timer.h"
#include "interrupt.h"
#include "led.h"
#include "key.h"

#define TIMER0_NUM				32

S3C2440_Timer0_Desc				S3C2440_Timer0_IRQ_Func_Array[TIMER0_NUM];		//32个定时器0中断源执行函数数组

/**********************************************************************************************************
 @Function			void S3C2440_TIMER0_Init(void)
 @Description			S3C2440_TIMER0_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER0_Init(void)
{
	/* -设置TIM0时钟- */
	
	/* -Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}-
	 * -{prescaler value} = 0~255-
	 * -{divider value} = 2, 4, 8, 16-
	 * -Timer clk = 50000000 / (49 + 1) / 16-
	 * -          = 62500-
	 */
	TCFG0 = 49;														/* Prescaler 0 = 49, 用于timer0,1 */
	
	TCFG1 &= ~(0x0F);
	TCFG1 |=  (0x03);													/* MUX 0 : 0011 = 1/16 */
	
	/* -设置TIM0初值- */
	TCNTB0 = 625;														/* 10毫秒中断一次 */
	
	/* -加载初值, 启动TIM0- */
	TCON |=  (1<<1);													/* Update from TCNTB0 & TCMPB0 */
	
	/* -设置为自动加载并启动- */
	TCON &= ~(1<<1);													/* The bit has to be cleared at next writing */
	TCON |=  (1<<0) | (1<<3);											/* bit0: start, bit3: auto reload */
	
	/* 注册中断源 */
	S3C2440_IRQRegister(INT_TIMER0, S3C2440_TIMER0_Irq);						/* INT_TIMER0 */
}

/**********************************************************************************************************
 @Function			int S3C2440_TIMER0_Register(char* name, S3C2440_Timer0_IRQ_Function fp)
 @Description			S3C2440_TIMER0_Register		: 注册定时器0中断源
 @Input				name						: 定时器0中断名
					fd						: 定时器0中断执行函数
 @Return				0						: Success
					-1						: Fail
**********************************************************************************************************/
int S3C2440_TIMER0_Register(char* name, S3C2440_Timer0_IRQ_Function fp)
{
	for (int i = 0; i < TIMER0_NUM; i++) {
		if (!S3C2440_Timer0_IRQ_Func_Array[i].fp) {
			S3C2440_Timer0_IRQ_Func_Array[i].name = name;
			S3C2440_Timer0_IRQ_Func_Array[i].fp   = fp;
			return 0;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int S3C2440_TIMER0_Unregister(char* name)
 @Description			S3C2440_TIMER0_Unregister	: 注销定时器0中断源
 @Input				name						: 定时器0中断名
 @Return				0						: Success
					-1						: Fail
**********************************************************************************************************/
int S3C2440_TIMER0_Unregister(char* name)
{
	for (int i = 0; i < TIMER0_NUM; i++) {
		if (!strcmp(S3C2440_Timer0_IRQ_Func_Array[i].name, name)) {
			S3C2440_Timer0_IRQ_Func_Array[i].name = NULL;
			S3C2440_Timer0_IRQ_Func_Array[i].fp   = NULL;
			return 0;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER0_Irq(unsigned int irqBit)
 @Description			S3C2440_TIMER0_Irq
 @Input				irqBit
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER0_Irq(unsigned int irqBit)
{
	if (irqBit == INT_TIMER0) {
		for (int i = 0; i < TIMER0_NUM; i++) {
			if (S3C2440_Timer0_IRQ_Func_Array[i].fp) {
				S3C2440_Timer0_IRQ_Func_Array[i].fp();
			}
		}
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER0_LED_Irq_Demo(void)
 @Description			S3C2440_TIMER0_LED_Irq_Demo
 @Input				irqBit
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER0_LED_Irq_Demo(void)
{
	static char nCount = 0;
	static char tTimer = 0;
	
	if (tTimer++ < 50) return;
	
	tTimer = 0;
	nCount++;
	
	if (nCount & 0x01) S3C2440_LED1_ON(); else S3C2440_LED1_OFF();
	if (nCount & 0x02) S3C2440_LED2_ON(); else S3C2440_LED2_OFF();
	if (nCount & 0x04) S3C2440_LED3_ON(); else S3C2440_LED3_OFF();
}

/********************************************** END OF FLEE **********************************************/
