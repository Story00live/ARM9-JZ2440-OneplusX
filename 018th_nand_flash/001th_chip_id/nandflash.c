/**
  *********************************************************************************************************
  * @file    nandflash.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-04-05
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					
  *					
  *
  *********************************************************************************************************
  */

#include "nandflash.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_NandFlashInitialized(void)
 @Description			S3C2440_NandFlashInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NandFlashInitialized(void)
{
	/* -设置NAND FLASH时序- */
	NFCONF &= ~((7<<4) | (7<<8) | (3<<12));
	NFCONF |=  ((0<<4) | (1<<8) | (0<<12));
	
	/* -使能NAND FLASH控制器, 初始化ECC, 禁止片选- */
	NFCONT &= ~((1<<0) | (0<<1) | (1<<4));
	NFCONT |=  ((1<<0) | (1<<1) | (1<<4));
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlashSelect(void)
 @Description			S3C2440_NandFlashSelect						: 使能Nand Flash片选
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlashSelect(void)
{
	NFCONT &= ~(1<<1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlashDeselect(void)
 @Description			S3C2440_NandFlashDeselect					: 禁止Nand Flash片选
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlashDeselect(void)
{
	NFCONT |=  (1<<1);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_Write_Command(unsigned char cmmd)
 @Description			S3C2440_NandFlash_Write_Command				: Nand Flash 写入指令
 @Input				cmmd										: 指令值
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_Write_Command(unsigned char cmmd)
{
	NFCMMD = cmmd;
	for (volatile int i = 0; i < 10; i++);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_Write_Address(unsigned char addr)
 @Description			S3C2440_NandFlash_Write_Address				: Nand Flash 写入地址
 @Input				addr										: 地址值
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_Write_Address(unsigned char addr)
{
	NFADDR = addr;
	for (volatile int i = 0; i < 10; i++);
}

/**********************************************************************************************************
 @Function			static unsigned char S3C2440_NandFlash_Read_Data(void)
 @Description			S3C2440_NandFlash_Read_Data					: Nand Flash 读取数据
 @Input				void
 @Return				val										: 数据值
**********************************************************************************************************/
static unsigned char S3C2440_NandFlash_Read_Data(void)
{
	return NFDATA;
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_Write_Data(unsigned char val)
 @Description			S3C2440_NandFlash_Write_Data					: Nand Flash 写入数据
 @Input				val										: 数据值
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_Write_Data(unsigned char val)
{
	NFDATA = val;
}



































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
	
	printfln("nandFlashMakerCode   = 0x%x",  deviceCode[0]);
	printfln("nandFlashDeviceCode  = 0x%x",  deviceCode[1]);
	printfln("nandFlash3rdCycle    = 0x%x",  deviceCode[2]);
	printfln("nandFlash4rdCycle    = 0x%x",  deviceCode[3]);
	printfln("nandFlash5rdCycle    = 0x%x",  deviceCode[4]);
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
	
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doWriteNandFlash(void)
 @Description			S3C2440_NandFlash_doWriteNandFlash				: Nand Flash 写入
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doWriteNandFlash(void)
{
	
}

/**********************************************************************************************************
 @Function			static void S3C2440_NandFlash_doReadNandFlash(void)
 @Description			S3C2440_NandFlash_doReadNandFlash				: Nand Flash 读取
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NandFlash_doReadNandFlash(void)
{
	
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
