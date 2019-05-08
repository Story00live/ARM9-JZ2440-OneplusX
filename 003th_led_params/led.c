/**
  *********************************************************************************************************
  * @file    led.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-02-19
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 点亮nLED_1 : GPF4
  *					 点亮nLED_2 : GPF5
  *					 点亮nLED_4 : GPF6
  *					 配置GPF4,GPF5,GPF6为输出引脚
  *					 设置GPF4,GPF5,GPF6输出高电平
  *********************************************************************************************************
  */

#include "s3c2440_soc.h"

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
 @Function			int led_on(int which)
 @Description			led_on
 @Input				which
 @Return				int
**********************************************************************************************************/
int led_on(int which)
{
	GPFCON = 0x00001500;
	
	if (which == 1)
		GPFDAT = 0x00000000;
	else
		GPFDAT = 0xFFFFFFFF;
}

/**********************************************************************************************************
 @Function			int main(int argc, char const *argv[])
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int main(int argc, char const *argv[])
{
	/* -配置GPF4,GPF5,GPF6为输出引脚-
	 * -把 0x00001500 写到地址 0x56000050-
	 */
	GPFCON = 0x00001500;
	
	/* -配置GPF4,GPF5,GPF6输出低电平-
	 * -把 0x00000000 写到地址 0x56000054-
	 */
	GPFDAT = 0x00000000;
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
