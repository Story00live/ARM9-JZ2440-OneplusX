/**
  *********************************************************************************************************
  * @file    norflashdemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-04-07
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					Nor Flash 数据总线16位
  *					Nor Flash 地址总线20位
  *
  *********************************************************************************************************
  */

#include "norflashdemo.h"
#include "norflash.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doScanNorFlash(void)
 @Description			S3C2440_NorFlash_doScanNorFlash				: Nor Flash 读取信息
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doScanNorFlash(void)
{
	char qryAsciiStr[4];
	unsigned int norFLashSize;
	unsigned int norFlashVendorID, norFLashDeviceID;
	unsigned int norFlashRegions;
	unsigned int norFlashRegionInfoBase = 0x2D;
	unsigned int norFlashBlocks;
	unsigned int norFlashBlockSize;
	unsigned int norFlashBlockBaseAddr = NOR_FLASH_BASE_ADDR;
	
	printfln("");
	
	/* -解锁- */
	S3C2440_NorFlash_Write_Command(0x0555, 0xAA);
	S3C2440_NorFlash_Write_Command(0x02AA, 0x55);
	S3C2440_NorFlash_Write_Command(0x0555, 0x90);
	
	/* -读取 Manifacture ID Device ID - */
	norFlashVendorID = S3C2440_NorFlash_Read_Data(0x00);
	norFLashDeviceID = S3C2440_NorFlash_Read_Data(0x01);
	
	printfln("norFlashVendorID   = 0x%x", norFlashVendorID);
	printfln("norFLashDeviceID   = 0x%x", norFLashDeviceID);
	
	/* -Reset Nor Flash- */
	S3C2440_NorFlash_Write_Command(0x00, 0xF0);
	
	/* -进入CFI模式- */
	S3C2440_NorFlash_Write_Command(0x55, 0x98);
	
	/* -读取QRY标准字符值- */
	qryAsciiStr[0] = S3C2440_NorFlash_Read_Data(0x10);
	qryAsciiStr[1] = S3C2440_NorFlash_Read_Data(0x11);
	qryAsciiStr[2] = S3C2440_NorFlash_Read_Data(0x12);
	qryAsciiStr[3] = '\0';
	
	printfln("QryAsciiStr        = %s", qryAsciiStr);
	
	/* -读取容量值- */
	norFLashSize = 1 << S3C2440_NorFlash_Read_Data(0x27);
	
	printfln("Nor FLash Size     = 0x%x, %dM", norFLashSize, norFLashSize / (1024*1024));
	
	/* -读取各个扇区起始地址-
	 *
	 * -一个 nor flash 含有1个或多个 region-
	 * -一个 region 含有1个或多个 block(扇区)-
	 * 
	 * -Erase block region information:-
	 * -前2字节+1    : 表示该region有多少个block-
	 * -后2字节*256  : 表示block的大小-
	 * 
	 */
	unsigned char newLineCnt = 0;
	printfln("\r\nSector Start Addresses:");
	norFlashRegions = S3C2440_NorFlash_Read_Data(0x2C);
	for (int regionIndex = 0; regionIndex < norFlashRegions; regionIndex++) {
		norFlashBlocks = ((S3C2440_NorFlash_Read_Data(norFlashRegionInfoBase+0x00) << 0) | (S3C2440_NorFlash_Read_Data(norFlashRegionInfoBase+0x01) << 8)) + 1;
		norFlashBlockSize = ((S3C2440_NorFlash_Read_Data(norFlashRegionInfoBase+0x02) << 0) | (S3C2440_NorFlash_Read_Data(norFlashRegionInfoBase+0x03) << 8)) * 256;
		norFlashRegionInfoBase += 4;
#if 0
		printfln("\r\nRegion: %d, Block: %d, BlockSize: 0x%x", regionIndex, norFlashBlocks, norFlashBlockSize);
#else
		for (int blockIndex = 0; blockIndex < norFlashBlocks; blockIndex++) {
			printf("%08X  ", norFlashBlockBaseAddr);
			norFlashBlockBaseAddr += norFlashBlockSize;
			newLineCnt++;
			if (newLineCnt % 5 == 0) printfln("");
		}
#endif
	}
	
	/* -退出CFI模式- */
	S3C2440_NorFlash_Write_Command(0x00, 0xF0);
	
	printfln("");
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doEraseNorFlash(void)
 @Description			S3C2440_NorFlash_doEraseNorFlash				: Nor Flash 擦除
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doEraseNorFlash(void)
{
	unsigned int eraseAddress;
	
	printf("Enter the address to erase : ");
	eraseAddress = S3C2440_StringGetUint();
	
	printfln("Eraseing ... ");
	
	S3C2440_NorFlash_Erase(eraseAddress);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doWriteNorFlash(void)
 @Description			S3C2440_NorFlash_doWriteNorFlash				: Nor Flash 写入
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doWriteNorFlash(void)
{
	unsigned int writeAddress;
	unsigned char writeStr[100];
	
	memset((void*)writeStr, 0x00, sizeof(writeStr));
	
	printf("Enter the address to write : ");
	writeAddress = S3C2440_StringGetUint();
	
	printf("Enter the string to write : ");
	S3C2440_StringGets(writeStr);
	
	printfln("Writing ... ");
	
	S3C2440_NorFlash_Write(writeAddress, writeStr, strlen(writeStr) + 1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doReadNorFlash(void)
 @Description			S3C2440_NorFlash_doReadNorFlash				: Nor Flash 读取
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doReadNorFlash(void)
{
	unsigned int readAddress;
	volatile unsigned char* pReadAddress;
	unsigned char readChar;
	unsigned char readBuf[64];
	unsigned char readStr[16];
	
	printf("Enter the address to read : ");
	readAddress = S3C2440_StringGetUint();
	
	S3C2440_NorFlash_Read(readAddress, readBuf, sizeof(readBuf));
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
 @Function			void S3C2440_NorFlashTest(void)
 @Description			S3C2440_NorFlashTest
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NorFlashTest(void)
{
	char chcode = 0;
	
	/* -打印菜单,　选择测试内容- */
	printf("[s] Scan  Nor Flash.\r\n");
	printf("[e] Erase Nor Flash.\r\n");
	printf("[w] Write Nor Flash.\r\n");
	printf("[r] Read  Nor Flash.\r\n");
	printf("[q] Quit  Menu.\r\n");
	
	printf("\r\nEnter selection: ");
	
	chcode = S3C2440_Uart0_Getchar();
	printf("%c\r\n", chcode);
	
	/* -测试内容 :-
	 * -1. 识别 Nor Flash-
	 * -2. 擦除 Nor Flash 某个扇区-
	 * -3. 编写 Nor Flash 某个扇区-
	 * -4. 读取 Nor Flash 某个地址数据-
	 */
	switch (chcode)
	{
		case 's':
		case 'S':
			S3C2440_NorFlash_doScanNorFlash();
			break;
		
		case 'e':
		case 'E':
			S3C2440_NorFlash_doEraseNorFlash();
			break;
		
		case 'w':
		case 'W':
			S3C2440_NorFlash_doWriteNorFlash();
			break;
		
		case 'r':
		case 'R':
			S3C2440_NorFlash_doReadNorFlash();
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
