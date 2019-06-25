/**
  *********************************************************************************************************
  * @file    main.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-02-23
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					变量		: 小驼峰模式
  *					宏值		: 全部大写
  * 					基本返回值 : 
  *							  0 True
  * 							 -1 False
  *********************************************************************************************************
  */

#include "s3c2440_soc.h"
#include "init.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "exti.h"
#include "timer.h"
#include "adc.h"
#include "touchscreen.h"
#include "touchscreenlib.h"
#include "norflash.h"
#include "nandflash.h"
#include "sdram.h"
#include "interrupt.h"
#include "norflashdemo.h"
#include "nandflashdemo.h"
#include "lcddemo.h"
#include "touchscreendemo.h"

#if 0
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
#endif

#if 0
/**********************************************************************************************************
 @Function			int sdramTest(void)
 @Description			sdramTest
 @Input				void
 @Return				int
**********************************************************************************************************/
int sdramTest(void)
{
	volatile unsigned char *pSdram = (volatile unsigned char *)0x30000000;
	
	memset((void *)pSdram, 0x55, 1000);
	
	for (int i = 0; i < 1000; i++) {
		if (pSdram[i] != 0x55) return -1;
	}
	
	return 0;
}
#endif

/**********************************************************************************************************
 @Function			void printException(unsigned int cpsr, char* str)
 @Description			printException
 @Input				cpsr
					str
 @Return				void
**********************************************************************************************************/
void __init printException(unsigned int cpsr, char* str)
{
	S3C2440_LedInitialized();
	S3C2440_KeyInitialized();
	S3C2440_Uart0Initialized();
	
	standard_putchar_callback(S3C2440_Uart0_Putchar);
	
#if 1
	printf("Exception : %s\r\n", str);
	printf("CPSR      : %X\r\n", cpsr);
	printf("String    : %s\r\n", str);
#else
	S3C2440_Uart0_Puts("Exception : Undefined instruction!\r\n");
#endif
}

/**********************************************************************************************************
 @Function			void printSWIVal(unsigned int *pSWI)
 @Description			printSWIVal
 @Input				pSWI
 @Return				void
**********************************************************************************************************/
void __init printSWIVal(unsigned int *pSWI)
{
	unsigned int val = *pSWI & (~0xFF000000);
	
	S3C2440_LedInitialized();
	S3C2440_KeyInitialized();
	S3C2440_Uart0Initialized();
	
	standard_putchar_callback(S3C2440_Uart0_Putchar);
	
	printf("SWI Val = %X\r\n", val);
}

/**********************************************************************************************************
 @Function			void debugPrint1(void)
 @Description			debugPrint1
 @Input				void
 @Return				void
**********************************************************************************************************/
void __init debugPrint1(void)
{
	S3C2440_Uart0Initialized();
	
	S3C2440_Uart0_Puts("Debug : 1\r\n");
}

/**********************************************************************************************************
 @Function			void debugPrint2(void)
 @Description			debugPrint2
 @Input				void
 @Return				void
**********************************************************************************************************/
void __init debugPrint2(void)
{
	S3C2440_Uart0Initialized();
	
	S3C2440_Uart0_Puts("Debug : 2\r\n");
}

char  ch1;
char  ch2 = 'A';
char  ch3 = 0;
char  chscanf = 0;
int   it4 = 0;
char* str = "JZ2440 Touch Screen Test : IIC Framework OK!!";

/**********************************************************************************************************
 @Function			int main(int argc, char const *argv[])
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int __init main(int argc, char const *argv[])
{
	S3C2440_LedInitialized();
	S3C2440_KeyInitialized();
	S3C2440_Uart0Initialized();
	
#if 1
	/* -初始化外部中断- */
	S3C2440_EXTI_Init();
	
	/* -初始化Timer0定时器- */
	S3C2440_TIMER0_Init();
	/* -初始化Timer1定时器- */
	S3C2440_TIMER1_Init();
#endif
	
	standard_putchar_callback(S3C2440_Uart0_Putchar);
	
	string_putchar_callback(S3C2440_Uart0_Putchar);
	string_getchar_callback(S3C2440_Uart0_Getchar);
	
#if 1
	/* -注册EINT0中断执行函数- */
	S3C2440_EXTI_Register("LED1_EXTI", EINT0, S3C2440_EXTI_LED1_Irq_Demo);
	/* -注册EINT2中断执行函数- */
	S3C2440_EXTI_Register("LED2_EXTI", EINT2, S3C2440_EXTI_LED2_Irq_Demo);
	/* -注册EINT8_23中断执行函数- */
	S3C2440_EXTI_Register("LED3_EXTI", EINT8_23, S3C2440_EXTI_LED3_Irq_Demo);
	
	/* -注册Timer0中断执行函数- */
	S3C2440_TIMER_Register("LED0_TIMER", INT_TIMER0, S3C2440_TIMER0_LED_Irq_Demo);
	/* -注册Timer1中断执行函数- */
	S3C2440_TIMER_Register("LED1_TIMER", INT_TIMER1, S3C2440_TIMER1_LED_Irq_Demo);
#endif
	
#if 0
	S3C2440_Uart0_Puts(str);
#endif
	
#if 1
	printfln("JZ2440 ARM920T S3C2440A V%d.%d", 1, 0);
     printfln("Copyright (C) 2019 Design by Kangkang");
	printfln("%s", str);
#if 0
	printf("%c\r\n", ch3 + 0x30);
	printf("%lf\r\n", 3.14159265354);
	printf("%lg\r\n", 3.14159265354);
	printf("%le\r\n", 3.14159265354);
	printf("\r\nHello World!!\r\n\r\n");
#endif
#endif
	
#if 0
	if (sdramTest() == 0) S3C2440_Uart0_Puts(str);
#endif
	
#if 0
	S3C2440_Uart0_Puts("Hello World!!\r\n");
	
	memset((void *)buffer, 0x00, sizeof(buffer));
	memcpy((void *)buffer, (const void *)"Buffer Data.\r\n", sizeof("Buffer Data.\r\n"));
	
	S3C2440_Uart0_Puts(buffer);
#endif
	
#if 0
	while (true) {
		if (S3C2440_KEYS2_READ()) S3C2440_LED1_OFF(); else S3C2440_LED1_ON();
		if (S3C2440_KEYS3_READ()) S3C2440_LED2_OFF(); else S3C2440_LED2_ON();
		if (S3C2440_KEYS4_READ()) S3C2440_LED3_OFF(); else S3C2440_LED3_ON();
	}
#endif
	
#if 0
	S3C2440_NorFlashTest();
#endif
	
#if 0
	printf("Enter Value : ");
	it4 = S3C2440_StringGetSint();
	printf("Scan val = %d\r\n", it4);
#endif
	
#if 0
	S3C2440_LCDTest();
#endif
	
#if 0
	S3C2440_AdcInitialized();
#endif
#if 0
	S3C2440_TouchScreen_Init();
#endif
	
#if 0
	S3C2440_TouchScreenTest();
#endif
	
	while (true) {
		
#if 0
		int    val;
		double vol;
		val = S3C2440_AdcRead_AIN0();
		vol = (double)val/1023*3.3;
		printf("vol: %fv\r", vol);
#endif
		
#if 0
		/* -打印菜单,　选择测试内容- */
		printf("[a] Test Nor  Flash.\r\n");
		printf("[b] Test Nand Flash.\r\n");
		printf("[q] Quit  Menu.\r\n");
		
		printf("\r\nEnter selection: ");
		
		chscanf = S3C2440_Uart0_Getchar();
		printfln("%c\r\n", chscanf);
		
		switch (chscanf)
		{
			case 'a':
			case 'A':
				S3C2440_NorFlashTest();
				break;
			
			case 'b':
			case 'B':
				S3C2440_NandFlashTest();
				break;
			
			case 'q':
			case 'Q':
				break;
			
			default:
				break;
		}
#endif
		
#if 0
		S3C2440_LED1_ON();
		S3C2440_LED2_OFF();
		S3C2440_LED3_OFF();
		delay_simulate(0x0003FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_ON();
		S3C2440_LED3_OFF();
		delay_simulate(0x0003FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_OFF();
		S3C2440_LED3_ON();
		delay_simulate(0x0003FFFF);
#else
		delay_simulate(0x0003FFFF);
		delay_simulate(0x0003FFFF);
		delay_simulate(0x0003FFFF);
#endif
		
#if 0
#if 1
		printf("%c", ch2++);
		if (ch2 > 'z') ch2 = 'A';
#else
		S3C2440_Uart0_Putchar(ch2++);
#endif
#endif
	}
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
