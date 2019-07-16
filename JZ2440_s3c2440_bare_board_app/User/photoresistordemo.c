/**
  *********************************************************************************************************
  * @file    photoresistordemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-07-14
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "photoresistordemo.h"
#include "adc.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_PhotoresistorTest(void)
 @Description			S3C2440_PhotoresistorTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_PhotoresistorTest(void)
{
	int    val0, val1;
	double vol0, vol1;
	
	while (true) {
		val1 = S3C2440_AdcRead(S3C2440_ADC_AIN1);
		vol1 = (double)val1/1023*3.3;
		vol1 = (int)(vol1*100);
		vol1 = vol1/100;
		
		val0 = S3C2440_AdcRead(S3C2440_ADC_AIN0);
		vol0 = (double)val0/1023*3.3;
		vol0 = (int)(vol0*100);
		vol0 = vol0/100;
		
		printf("Photoresistor vol: %fv, Compare to threshold: %fv\r", vol1, vol0);
	}
}

/********************************************** END OF FLEE **********************************************/
