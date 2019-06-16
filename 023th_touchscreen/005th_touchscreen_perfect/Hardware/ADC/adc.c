/**
  *********************************************************************************************************
  * @file    adc.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-05-12
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *
  *********************************************************************************************************
  */

#include "adc.h"

/**********************************************************************************************************
 @Function			void S3C2440_AdcInitialized(void)
 @Description			S3C2440_AdcInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_AdcInitialized(void)
{
	/* -[15:15] : ECFLG, 1 = End of A/D conversion 0 = A/D conversion in process-
	 * -[14:14] : PRSCEN, 1 = A/D converter prescaler enable 0 disable-
	 * -[13:06] : PRSCVL, adc clk = PCLK / (PRSCVL + 1)-
	 * -[05:03] : SEL_MUX, 000 = AIN 0-
	 * -[02:02] : STDBM, Standby mode select 0 = Normal operation mode 1 = Standby mode-
	 * -[01:01] : READ_ START, A/D conversion start by read-
	 * -[00:00] : ENABLE_START, A/D conversion starts by enable-
	 */
	ADCCON = (1<<14) | (49<<6) | (0<<3);
	
	ADCDLY = 0x00FF;
}

/**********************************************************************************************************
 @Function			int S3C2440_AdcRead_AIN0(void)
 @Description			S3C2440_AdcRead_AIN0
 @Input				void
 @Return				void
**********************************************************************************************************/
int S3C2440_AdcRead_AIN0(void)
{
	int adcval = 0;
	
	/* 启动ADC */
	ADCCON |=  (1<<0);
	
	/* 等待ADC转换 */
	while (!(ADCCON & (1<<15)));
	
	adcval = ADCDAT0 & 0x3FF;
	
	/* 关闭ADC */
	ADCCON &= ~(1<<0);
	
	return adcval;
}

/********************************************** END OF FLEE **********************************************/
