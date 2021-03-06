/**
  *********************************************************************************************************
  * @file    exti.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-11
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 EINT0  : KEY_S2 : GPF0
  *					 EINT2  : KEY_S3 : GPF2
  *					 EINT11 : KEY_S4 : GPG3
  *					 EINT19 : KEY_S5 : GPG11
  *********************************************************************************************************
  */

#include "exti.h"
#include "interrupt.h"
#include "led.h"
#include "key.h"

/**********************************************************************************************************
 @Function			void S3C2440_EXTI_Init(void)
 @Description			S3C2440_EXTI_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_EXTI_Init(void)
{
	/* -配置GPF0,GPF2,GPG3,GPG11为中断引脚- */
	GPFCON &= ~((3<<(0*2)) | (3<<(2*2)));
	GPFCON |=  ((2<<(0*2)) | (2<<(2*2)));
	
	GPGCON &= ~((3<<(3*2)) | (3<<(11*2)));
	GPGCON |=  ((2<<(3*2)) | (2<<(11*2)));
	
	/* -设置EINT0,EINT2,EINT11,EINT19中断触发方式: 双边沿触发- */
	EXTINT0 &= ~(7<<(0*4));
	EXTINT0 |=  (7<<(0*4));												/* EINT0 */
	
	EXTINT0 &= ~(7<<(2*4));
	EXTINT0 |=  (7<<(2*4));												/* EINT2 */
	
	EXTINT1 &= ~(7<<12);
	EXTINT1 |=  (7<<12);												/* EINT11 */
	
	EXTINT2 &= ~(7<<12);
	EXTINT2 |=  (7<<12);												/* EINT19 */
	
	/* -设置EINTMASK使能EINT11,EINT19- */
	EINTMASK &= ~((1<<11) | (1<<19));
	
	/* 注册中断源 */
	S3C2440_IRQRegister(0, S3C2440_EXTI_Irq);								/* EINT0 */
	S3C2440_IRQRegister(2, S3C2440_EXTI_Irq);								/* EINT2 */
	S3C2440_IRQRegister(5, S3C2440_EXTI_Irq);								/* EINT8_23 */
}

/**********************************************************************************************************
 @Function			void S3C2440_EXTI_Irq(unsigned int irqBit)
 @Description			S3C2440_EXTI_Irq
 @Input				irqBit
 @Return				void
 @attention			读EINTPEND分辨率哪个EINT产生, 清除中断时, 写EINTPEND的相应位
**********************************************************************************************************/
void S3C2440_EXTI_Irq(unsigned int irqBit)
{
	unsigned int eintPendBit = EINTPEND;
	
	switch (irqBit)
	{
		case 0:														/* EINT0 */
			if (S3C2440_KEYS2_READ()) {
				/* S2松开 */
				if (S3C2440_LED1_READ()) S3C2440_LED1_ON(); else S3C2440_LED1_OFF();
			}
			break;
	
		case 2:														/* EINT2 */
			if (S3C2440_KEYS3_READ()) {
				/* S3松开 */
				if (S3C2440_LED2_READ()) S3C2440_LED2_ON(); else S3C2440_LED2_OFF();
			}
			break;
	
		case 5:														/* EINT8_23 */
			if (eintPendBit & (1<<11)) {
				/* EINT11 */
				if (S3C2440_KEYS4_READ()) {
					/* S4松开 */
					if (S3C2440_LED3_READ()) S3C2440_LED3_ON(); else S3C2440_LED3_OFF();
				}
			}
			if (eintPendBit & (1<<19)) {
				/* EINT19 */
				if (S3C2440_KEYS5_READ()) {
					/* S5松开 */
					if (S3C2440_LED1_READ()) S3C2440_LED1_ON(); else S3C2440_LED1_OFF();
					if (S3C2440_LED2_READ()) S3C2440_LED2_ON(); else S3C2440_LED2_OFF();
					if (S3C2440_LED3_READ()) S3C2440_LED3_ON(); else S3C2440_LED3_OFF();
				}
			}
			break;
	
		default:
			break;
	}
	
	EINTPEND = eintPendBit;
}

/********************************************** END OF FLEE **********************************************/
