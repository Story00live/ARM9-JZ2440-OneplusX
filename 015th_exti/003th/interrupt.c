/**
  *********************************************************************************************************
  * @file    interrupt.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-10
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "interrupt.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "exti.h"
#include "norflash.h"
#include "sdram.h"

/**
  *********************************************************************************************************
  * SRCPND 用来显示哪个中断产生了, 需要清除对应位
  * bit0-EINT0
  * bit2-EINT2
  * bit5-EINT8_23
  *********************************************************************************************************
  */

/**
  *********************************************************************************************************
  * INTMSK 用来屏蔽中断, 1-masked
  * bit0-EINT0
  * bit2-EINT2
  * bit5-EINT8_23
  *********************************************************************************************************
  */

/**
  *********************************************************************************************************
  * INTPND 用来显示当前优先级最高的, 正在发生的中断, 需要清除对应位
  * bit0-EINT0
  * bit2-EINT2
  * bit5-EINT8_23
  *********************************************************************************************************
  */

/**
  *********************************************************************************************************
  * INTOFFSET : 用来显示INTPND中哪一位被设置为1
  *********************************************************************************************************
  */

/**********************************************************************************************************
 @Function			void S3C2440_IRQInitialized(void)
 @Description			S3C2440_IRQInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_IRQInitialized(void)
{
	INTMSK &= ~((1<<0) | (1<<2) | (1<<5));
}

/**********************************************************************************************************
 @Function			void S3C2440_IRQHandler(void)
 @Description			S3C2440_IRQHandler
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_IRQHandler(void)
{
	/* 分辨中断源 */
	unsigned int irqSrcBit = INTOFFSET;
	
	/* 调用对应处理函数 */
	if ((irqSrcBit == 0) || (irqSrcBit == 2) || (irqSrcBit == 5)) {				/* EINT0, EINT2, EINT8_23 */
		S3C2440_EXTI_Irq(irqSrcBit);										/* 处理中断, 清中断源EINTPEND */
	}
	
	/* 清中断 */
	SRCPND = (1<<irqSrcBit);
	INTPND = (1<<irqSrcBit);
}

/********************************************** END OF FLEE **********************************************/
