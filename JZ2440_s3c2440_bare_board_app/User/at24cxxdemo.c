/**
  *********************************************************************************************************
  * @file    at24cxxdemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-23
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "at24cxxdemo.h"
#include "at24cxx.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			static void S3C2440_AT24CXX_doWrite(void)
 @Description			S3C2440_AT24CXX_doWrite				: AT24CXX 写入
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_AT24CXX_doWrite(void)
{
	int rc;
	unsigned int writeAddress;
	unsigned char writeStr[100];
	
	memset((void*)writeStr, 0x00, sizeof(writeStr));
	
	printf("Enter the address to write : ");
	writeAddress = S3C2440_StringGetUint();
	
	printf("Enter the string to write : ");
	S3C2440_StringGets(writeStr);
	
	printfln("Writing ... ");
	
	rc = S3C2440_AT24CXX_Write(writeAddress, writeStr, strlen(writeStr) + 1);
	printfln("AT24CXX Write ret = %d\r\n", rc);
	
	if (rc) return;
}

/**********************************************************************************************************
 @Function			static void S3C2440_AT24CXX_doRead(void)
 @Description			S3C2440_AT24CXX_doRead				: AT24CXX 读取
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_AT24CXX_doRead(void)
{
	int rc;
	unsigned int readAddress;
	volatile unsigned char* pReadAddress;
	unsigned char readChar;
	unsigned char readBuf[64];
	unsigned char readStr[16];
	
	memset((void*)readBuf, 0x00, sizeof(readBuf));
	memset((void*)readStr, 0x00, sizeof(readStr));
	
	printf("Enter the address to read : ");
	readAddress = S3C2440_StringGetUint();
	
	rc = S3C2440_AT24CXX_Read(readAddress, readBuf, sizeof(readBuf));
	printfln("AT24CXX Read ret = %d\r\n", rc);
	
	pReadAddress = (volatile unsigned char*)readBuf;
	
	if (rc) return;
	
	printfln("Data : ");
	for (int row = 0; row < 4; row++) {
		/* -打印数值- */
		for (int column = 0; column < 16; column++) {
			readChar = *pReadAddress++;
			readStr[column] = readChar;
			printf("%02X ", readChar);
		}
		
		printf("     ; ");
		
		/* -打印字符- */
		for (int column = 0; column < 16; column++) {
			if (readStr[column] < 0x20 || readStr[column] > 0x7E)
				printf(".");
			else
				printf("%c", readStr[column]);
		}
		
		printfln("");
	}
	
	printfln("");
}

/**********************************************************************************************************
 @Function			void S3C2440_AT24CXXTest(void)
 @Description			S3C2440_AT24CXXTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_AT24CXXTest(void)
{
	char chcode = 0;
	
	/* -打印菜单,　选择测试内容- */
	printf("AT24C02 Size : %d Byte.\r\n", AT24C02_BYTE_SIZE);
	printf("[w] Write AT24CXX.\r\n");
	printf("[r] Read  AT24CXX.\r\n");
	printf("[q] Quit  Menu.\r\n");
	
	printf("\r\nEnter selection: ");
	
	chcode = S3C2440_Uart0_Getchar();
	printf("%c\r\n", chcode);
	
	/* -测试内容 :-
	 * -1. 编写 AT24CXX-
	 * -2. 读取 AT24CXX-
	 */
	switch (chcode)
	{
		case 'w':
		case 'W':
			S3C2440_AT24CXX_doWrite();
			break;
		
		case 'r':
		case 'R':
			S3C2440_AT24CXX_doRead();
			break;
			
		case 'q':
		case 'Q':
			return;
			break;
		
		default:
			return;
			break;
	}
}

/********************************************** END OF FLEE **********************************************/
