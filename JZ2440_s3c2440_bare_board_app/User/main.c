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
#include "i2c.h"
#include "oled.h"
#include "w25qxx.h"
#include "dht11.h"
#include "ds18b20.h"
#include "norflash.h"
#include "nandflash.h"
#include "sdram.h"
#include "mmu.h"
#include "interrupt.h"
#include "norflashdemo.h"
#include "nandflashdemo.h"
#include "lcddemo.h"
#include "touchscreendemo.h"
#include "at24cxxdemo.h"
#include "photoresistordemo.h"

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
char* str = "JZ2440 Bare Board APP Test : Boot OK!!\r\n";

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
	
#if 0
	S3C2440_OLEDInitialized();
	S3C2440_W25QxxInitialized();
#endif
	
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
	S3C2440_TIMER_Register("SYST_TIMER", INT_TIMER0, S3C2440_TIMER0_System_10ms_Irq);
	/* -注册Timer0中断执行函数- */
	S3C2440_TIMER_Register("LED0_TIMER", INT_TIMER0, S3C2440_TIMER0_LED_Irq_Demo);
	/* -注册Timer1中断执行函数- */
	S3C2440_TIMER_Register("LED1_TIMER", INT_TIMER1, S3C2440_TIMER1_LED_Irq_Demo);
#endif
	
#if 0
	S3C2440_Uart0_Puts(str);
#endif
	
#if 1
	printfln("JZ2440 ARM920T S3C2440A V%d.%d", 0, 1);
     printfln("Copyright (C) 2019 Design by Kangkang\r\n");
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
	S3C2440_AdcInitialized(S3C2440_ADC_AIN0);
#endif
#if 0
	S3C2440_TouchScreen_Init();
#endif
	
#if 0
	S3C2440_TouchScreenTest();
#endif
	
#if 0
	I2C_Init();
#endif
	
#if 0
	int ManufacturerID = 0, DeviceID = 0;
	char OLEDStr[16];
	char W25QStr[16];
	S3C2440_W25QxxReadID(&ManufacturerID, &DeviceID);
	printfln("W25QXX ManufacturerID : 0x%X, DeviceID : 0x%X\r\n", ManufacturerID, DeviceID);
	sprintf(OLEDStr, "MID: %X DID: %X", ManufacturerID, DeviceID);
	S3C2440_W25QxxEraseSector(4096);
	S3C2440_W25QxxProgram(4096, (unsigned char*)"W25QXX Demo!", 13);
	S3C2440_W25QxxRead(4096, (unsigned char*)W25QStr, 13);
#endif
	
#if 0
	S3C2440_OLEDPrint(0, 0, "JZ2440 Board V20");
	S3C2440_OLEDPrint(2, 0, "SPI FLASH");
	S3C2440_OLEDPrint(4, 0, W25QStr);
	S3C2440_OLEDPrint(6, 0, OLEDStr);
#endif
	
#if 0
	S3C2440_OLEDPrint(0, 0, "JZ2440 Board V31");
	S3C2440_OLEDPrint(2, 0, "OLED ADC");
#endif
	
#if 0
	S3C2440_PhotoresistorTest();
#endif
	
#if 0
	S3C2440_DHT11Initialized();
#endif
	
#if 0
	S3C2440_DS18B20Initialized();
#endif
	
	while (true) {
		
#if 0
		int    val;
		double vol;
		val = S3C2440_AdcRead(S3C2440_ADC_AIN0);
		vol = (double)val/1023*3.3;
		printf("vol: %fv\r", vol);
#endif
		
#if 0
		char OLEDADCStr[16];
		sprintf(OLEDADCStr, "ADC: %.3fV", vol);
		S3C2440_OLEDPrint(4, 0, "ADC Read:");
		S3C2440_OLEDPrint(6, 0, OLEDADCStr);
#endif
		
#if 0
		int thum, temp;
		if (S3C2440_DHT11Read(&thum, &temp) != 0) {
			printf("\r\nDHT11 Read Err!\r\n");
		}
		else {
			printf("\r\nDHT11 : %d humidity, %d temperature\r\n", thum, temp);
		}
#endif
		
#if 0
		double temp;
		if (S3C2440_DS18B20ReadTemperature(&temp) != 0) {
			printf("\r\nDS18B20 Read Err!\r\n");
		}
		else {
			printf("\r\nDS18B20 Temperature: %.4f\r\n", temp);
		}
#endif
		
#if 0
		/* -打印菜单,　选择测试内容- */
		printf("[a] Test Nor  Flash.\r\n");
		printf("[b] Test Nand Flash.\r\n");
		printf("[c] Test AT24C02 EEPROM.\r\n");
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
			
			case 'c':
			case 'C':
				S3C2440_AT24CXXTest();
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
		S3C2440_TIMER0_mDelay(1000);
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
