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
#include "uart.h"
#include "norflash.h"
#include "sdram.h"
#include "interrupt.h"

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
void printException(unsigned int cpsr, char* str)
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
void printSWIVal(unsigned int *pSWI)
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
void debugPrint1(void)
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
void debugPrint2(void)
{
	S3C2440_Uart0Initialized();
	
	S3C2440_Uart0_Puts("Debug : 2\r\n");
}

char  ch1;
char  ch2 = 'A';
char  ch3 = 0;
char* str = "JZ2440 EXT Interrupt Test OK!!\r\n";

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
	S3C2440_Uart0Initialized();
	
	/* -初始化中断控制器- */
	S3C2440_IRQInitialized();
	/* -初始化外部中断- */
	S3C2440_EXTI_Init();
	
	standard_putchar_callback(S3C2440_Uart0_Putchar);
	
#if 0
	S3C2440_Uart0_Puts(str);
#endif
	
#if 1
	printf("%s", str);
	printf("%c", ch3 + 0x30);
	printf("\r\nHello World!!\r\n");
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
	
	while (true) {
		
#if 0
		S3C2440_LED1_ON();
		S3C2440_LED2_OFF();
		S3C2440_LED3_OFF();
		delay(0x0003FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_ON();
		S3C2440_LED3_OFF();
		delay(0x0003FFFF);
		S3C2440_LED1_OFF();
		S3C2440_LED2_OFF();
		S3C2440_LED3_ON();
		delay(0x0003FFFF);
#else
		delay(0x0003FFFF);
		delay(0x0003FFFF);
		delay(0x0003FFFF);
#endif
		
#if 1
		printf("%c", ch2++);
		if (ch2 > 'z') ch2 = 'A';
#else
		S3C2440_Uart0_Putchar(ch2++);
#endif
		
	}
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
