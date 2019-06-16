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

#define TIMER_NUM				32

S3C2440_Timer_Desc				S3C2440_Timer_IRQ_Func_Array[TIMER_NUM];		//32个定时器中断源执行函数数组

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
	TCFG0 &= ~((255 & 0xFF) << 0);
	TCFG0 |=  ((49  & 0xFF) << 0);										/* Prescaler 0 = 49, 用于timer0,1 */
	
	TCFG1 &= ~(0x0F << 0);
	TCFG1 |=  (0x03 << 0);												/* MUX 0 : 0011 = 1/16 */
	
	/* -设置TIM0初值- */
	TCNTB0 = 625;														/* 10毫秒中断一次 */
	
	/* -加载初值, 启动TIM0- */
	TCON |=  (1<<1);													/* Update from TCNTB0 & TCMPB0 */
	
	/* -设置为自动加载并启动- */
	TCON &= ~(1<<1);													/* The bit has to be cleared at next writing */
	TCON |=  (1<<0) | (1<<3);											/* bit0: start, bit3: auto reload */
	
	/* 注册中断源 */
	S3C2440_IRQRegister(INT_TIMER0, S3C2440_TIMER_Irq);						/* INT_TIMER0 */
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER1_Init(void)
 @Description			S3C2440_TIMER1_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER1_Init(void)
{
	/* -设置TIM0时钟- */
	
	/* -Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}-
	 * -{prescaler value} = 0~255-
	 * -{divider value} = 2, 4, 8, 16-
	 * -Timer clk = 50000000 / (49 + 1) / 16-
	 * -          = 62500-
	 */
	TCFG0 &= ~((255 & 0xFF) << 0);
	TCFG0 |=  ((49  & 0xFF) << 0);										/* Prescaler 0 = 49, 用于timer0,1 */
	
	TCFG1 &= ~(0x0F << 4);
	TCFG1 |=  (0x03 << 4);												/* MUX 1 : 0011 = 1/16 */
	
	/* -设置TIM1初值- */
	TCNTB1 = 6250;														/* 100毫秒中断一次 */
	
	/* -加载初值, 启动TIM0- */
	TCON |=  (1<<9);													/* Update from TCNTB1 & TCMPB1 */
	
	/* -设置为自动加载并启动- */
	TCON &= ~(1<<9);													/* The bit has to be cleared at next writing */
	TCON |=  (1<<8) | (1<<11);											/* bit0: start, bit3: auto reload */
	
	/* 注册中断源 */
	S3C2440_IRQRegister(INT_TIMER1, S3C2440_TIMER_Irq);						/* INT_TIMER1 */
}

/**********************************************************************************************************
 @Function			int S3C2440_TIMER_Register(char* name, unsigned char irq, S3C2440_Timer_IRQ_Function fp)
 @Description			S3C2440_TIMER_Register		: 注册定时器中断源
 @Input				name						: 定时器中断名
					irq						: 中断源
					fd						: 定时器中断执行函数
 @Return				0						: Success
					-1						: Fail
**********************************************************************************************************/
int S3C2440_TIMER_Register(char* name, unsigned char irq, S3C2440_Timer_IRQ_Function fp)
{
	for (int i = 0; i < TIMER_NUM; i++) {
		if (!S3C2440_Timer_IRQ_Func_Array[i].fp) {
			S3C2440_Timer_IRQ_Func_Array[i].name = name;
			S3C2440_Timer_IRQ_Func_Array[i].irq  = irq;
			S3C2440_Timer_IRQ_Func_Array[i].fp   = fp;
			return 0;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int S3C2440_TIMER_Unregister(char* name)
 @Description			S3C2440_TIMER_Unregister		: 注销定时器中断源
 @Input				name						: 定时器中断名
 @Return				0						: Success
					-1						: Fail
**********************************************************************************************************/
int S3C2440_TIMER_Unregister(char* name)
{
	for (int i = 0; i < TIMER_NUM; i++) {
		if (!strcmp(S3C2440_Timer_IRQ_Func_Array[i].name, name)) {
			S3C2440_Timer_IRQ_Func_Array[i].name = NULL;
			S3C2440_Timer_IRQ_Func_Array[i].irq  = 0;
			S3C2440_Timer_IRQ_Func_Array[i].fp   = NULL;
			return 0;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER_Irq(unsigned int irqBit)
 @Description			S3C2440_TIMER_Irq
 @Input				irqBit
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER_Irq(unsigned int irqBit)
{
	for (int i = 0; i < TIMER_NUM; i++) {
		if (S3C2440_Timer_IRQ_Func_Array[i].fp && (S3C2440_Timer_IRQ_Func_Array[i].irq == irqBit)) {
			S3C2440_Timer_IRQ_Func_Array[i].fp();
		}
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER0_LED_Irq_Demo(void)
 @Description			S3C2440_TIMER0_LED_Irq_Demo
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER0_LED_Irq_Demo(void)
{
	static char nCount0 = 0;
	static char tTimer0 = 0;
	
	if (tTimer0++ < 50) return;
	
	tTimer0 = 0;
	nCount0++;
	
	if (nCount0 & 0x01) S3C2440_LED1_ON(); else S3C2440_LED1_OFF();
	if (nCount0 & 0x02) S3C2440_LED2_ON(); else S3C2440_LED2_OFF();
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER1_LED_Irq_Demo(void)
 @Description			S3C2440_TIMER1_LED_Irq_Demo
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER1_LED_Irq_Demo(void)
{
	static char nCount1 = 0;
	static char tTimer1 = 0;
	
	if (tTimer1++ < 2) return;
	
	tTimer1 = 0;
	nCount1++;
	
	if (nCount1 & 0x01) S3C2440_LED3_ON(); else S3C2440_LED3_OFF();
}

/********************************************** END OF FLEE **********************************************/
