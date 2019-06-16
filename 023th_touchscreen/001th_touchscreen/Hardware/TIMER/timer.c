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
	 * -Timer clk = 50000000 / (99 + 1) / 16-
	 * -          = 31250-
	 */
	TCFG0 = 99;														/* Prescaler 0 = 99, 用于timer0,1 */
	
	TCFG1 &= ~(0x0F);
	TCFG1 |=  (0x03);													/* MUX 0 : 0011 = 1/16 */
	
	/* -设置TIM0初值- */
	TCNTB0 = 15625;													/* 0.5秒中断一次 */
	
	/* -加载初值, 启动TIM0- */
	TCON |=  (1<<1);													/* Update from TCNTB0 & TCMPB0 */
	
	/* -设置为自动加载并启动- */
	TCON &= ~(1<<1);													/* The bit has to be cleared at next writing */
	TCON |=  (1<<0) | (1<<3);											/* bit0: start, bit3: auto reload */
	
	/* 注册中断源 */
	S3C2440_IRQRegister(INT_TIMER0, S3C2440_TIMER0_Irq);						/* INT_TIMER0 */
}

/**********************************************************************************************************
 @Function			void S3C2440_TIMER0_Irq(unsigned int irqBit)
 @Description			S3C2440_TIMER0_Irq
 @Input				irqBit
 @Return				void
**********************************************************************************************************/
void S3C2440_TIMER0_Irq(unsigned int irqBit)
{
	static char nCount = 0;
	
	if (irqBit == 10) {
		nCount++;
		
		if (nCount & 0x01) S3C2440_LED1_ON(); else S3C2440_LED1_OFF();
		if (nCount & 0x02) S3C2440_LED2_ON(); else S3C2440_LED2_OFF();
		if (nCount & 0x04) S3C2440_LED3_ON(); else S3C2440_LED3_OFF();
	}
}

/********************************************** END OF FLEE **********************************************/
