/**
  *********************************************************************************************************
  * @file    main.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-02-23
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "s3c2440_soc.h"
#include "led.h"
#include "key.h"

/**********************************************************************************************************
 @Function			void delay(void)
 @Description			delay
 @Input				nCount
 @Return				void
**********************************************************************************************************/
void delay(int nCount)
{
	while (nCount--);
}

/**********************************************************************************************************
 @Function			int main(int argc, char const *argv[])
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int main(int argc, char const *argv[])
{
	S3C2440_LedInitialized();
	S3C2440_KeyInitialized();
	
#if 0
	while (true) {
		if (S3C2440_KEYS2_READ()) S3C2440_LED1_OFF(); else S3C2440_LED1_ON();
		if (S3C2440_KEYS3_READ()) S3C2440_LED2_OFF(); else S3C2440_LED2_ON();
		if (S3C2440_KEYS4_READ()) S3C2440_LED3_OFF(); else S3C2440_LED3_ON();
	}
#endif
	
	while (true) {
		S3C2440_LED1_ON();
		S3C2440_LED2_OFF();
		S3C2440_LED3_OFF();
		delay(0x0000FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_ON();
		S3C2440_LED3_OFF();
		delay(0x0000FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_OFF();
		S3C2440_LED3_ON();
		delay(0x0000FFFF);
	}
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
