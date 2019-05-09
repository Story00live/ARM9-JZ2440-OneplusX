/**
  *********************************************************************************************************
  * @file    nandflashdemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-04-07
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "nandflashdemo.h"
#include "nandflash.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doScanNandFlash(void)
 @Description			S3C2440_NandFlash_doScanNandFlash				: Nand Flash 读取信息
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doScanNandFlash(void)
{
	unsigned char deviceCode[5];
	
	/* -使能Nand Flash片选- */
	S3C2440_NandFlashSelect();
	/* -发送读ID命令- */
	S3C2440_NandFlash_Write_Command(0x90);
	/* -发送地址- */
	S3C2440_NandFlash_Write_Address(0x00);
	
	printfln("");
	
	deviceCode[0] = S3C2440_NandFlash_Read_Data();
	deviceCode[1] = S3C2440_NandFlash_Read_Data();
	deviceCode[2] = S3C2440_NandFlash_Read_Data();
	deviceCode[3] = S3C2440_NandFlash_Read_Data();
	deviceCode[4] = S3C2440_NandFlash_Read_Data();
	
	/* -禁止Nand Flash片选- */
	S3C2440_NandFlashDeselect();
	
	printfln("nandFlashMakerCode   = 0x%X",  deviceCode[0]);
	printfln("nandFlashDeviceCode  = 0x%X",  deviceCode[1]);
	printfln("nandFlash3rdCycle    = 0x%X",  deviceCode[2]);
	printfln("nandFlash4rdCycle    = 0x%X",  deviceCode[3]);
	printfln("nandFlash5rdCycle    = 0x%X",  deviceCode[4]);
	printfln("nandFlash Page  Size = %d kb", 1  << ((deviceCode[3] >> 0) & 0x03));
	printfln("nandFlash Block Size = %d kb", 64 << ((deviceCode[3] >> 4) & 0x03));
	
	printfln("");
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doEraseNandFlash(void)
 @Description			S3C2440_NandFlash_doEraseNandFlash				: Nand Flash 擦除
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doEraseNandFlash(void)
{
	unsigned int eraseAddress;
	unsigned int ret;
	
	printf("Enter the address to erase : ");
	eraseAddress = S3C2440_StringGetUint();
	
	printfln("Eraseing ... ");
	
	if ((ret = S3C2440_NandFlash_Erase(eraseAddress, 128*1024)) != 0) {
		printfln("Erase error, Check erase address");
	}
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doWriteNandFlash(void)
 @Description			S3C2440_NandFlash_doWriteNandFlash				: Nand Flash 写入
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doWriteNandFlash(void)
{
	unsigned int writeAddress;
	unsigned char writeStr[100];
	
	memset((void*)writeStr, 0x00, sizeof(writeStr));
	
	printf("Enter the address to write : ");
	writeAddress = S3C2440_StringGetUint();
	
	printf("Enter the string to write : ");
	S3C2440_StringGets(writeStr);
	
	printfln("Writing ... ");
	
	S3C2440_NandFlash_Write(writeAddress, writeStr, strlen(writeStr) + 1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doReadNandFlash(void)
 @Description			S3C2440_NandFlash_doReadNandFlash				: Nand Flash 读取
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doReadNandFlash(void)
{
	unsigned int readAddress;
	volatile unsigned char* pReadAddress;
	unsigned char readChar;
	unsigned char readBuf[64];
	unsigned char readStr[16];
	
	printf("Enter the address to read : ");
	readAddress = S3C2440_StringGetUint();
	
	S3C2440_NandFlash_Read(readAddress, readBuf, sizeof(readBuf));
	pReadAddress = (volatile unsigned char*)readBuf;
	
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
}

/**********************************************************************************************************
 @Function			void S3C2440_NandFlashTest(void)
 @Description			S3C2440_NandFlashTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlashTest(void)
{
	char chcode = 0;
	
	/* -打印菜单,　选择测试内容- */
	printf("[s] Scan  Nand Flash.\r\n");
	printf("[e] Erase Nand Flash.\r\n");
	printf("[w] Write Nand Flash.\r\n");
	printf("[r] Read  Nand Flash.\r\n");
	printf("[q] Quit  Menu.\r\n");
	
	printf("\r\nEnter selection: ");
	
	chcode = S3C2440_Uart0_Getchar();
	printf("%c\r\n", chcode);
	
	/* -测试内容 :-
	 * -1. 识别 Nand Flash-
	 * -2. 擦除 Nand Flash 某个扇区-
	 * -3. 编写 Nand Flash 某个扇区-
	 * -4. 读取 Nand Flash 某个地址数据-
	 */
	switch (chcode)
	{
		case 's':
		case 'S':
			S3C2440_NandFlash_doScanNandFlash();
			break;
		
		case 'e':
		case 'E':
			S3C2440_NandFlash_doEraseNandFlash();
			break;
		
		case 'w':
		case 'W':
			S3C2440_NandFlash_doWriteNandFlash();
			break;
		
		case 'r':
		case 'R':
			S3C2440_NandFlash_doReadNandFlash();
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
