/**
  *********************************************************************************************************
  * @file    norflash.c
  * @author  Devilhart -- KK
  * @version V0.0.2
  * @date    2019-03-14
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					Nor Flash 数据总线16位
  *					Nor Flash 地址总线20位
  *
  *********************************************************************************************************
  */

#include "norflash.h"
#include "led.h"
#include "key.h"
#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_NorFlashInitialized(void)
 @Description			S3C2440_NorFlashInitialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_NorFlashInitialized(void)
{
	BANKCON0 |=  (7<<8);
	BANKCON0 &= ~(2<<8);
}





/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_ChipWrite_HalfWord(unsigned int base, unsigned int offset, unsigned short val)
 @Description			S3C2440_NorFlash_ChipWrite_HalfWord			: Nor Flash Chip 写1个半字
 @Input				base										: 基地址
					offset									: 偏移量
					val										: 写入值
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_ChipWrite_HalfWord(unsigned int base, unsigned int offset, unsigned short val)
{
	/* -offset << 1 : Nor Flash 地址0位对应 S3C2440 地址1位, 所以 S3C2440 偏移地址左移1位- */
	volatile unsigned short* pNorFlashAddr = (volatile unsigned short*)(base + (offset << 1));
	
	*pNorFlashAddr = val;
}

/**********************************************************************************************************
 @Function			static unsigned short S3C2440_NorFlash_ChipRead_HalfWord(unsigned int base, unsigned int offset)
 @Description			S3C2440_NorFlash_ChipRead_HalfWord				: Nor Flash Chip 读1个半字
 @Input				base										: 基地址
					offset									: 偏移量
 @Return				val										: 读取值
**********************************************************************************************************/
static unsigned short S3C2440_NorFlash_ChipRead_HalfWord(unsigned int base, unsigned int offset)
{
	/* -offset << 1 : Nor Flash 地址0位对应 S3C2440 地址1位, 所以 S3C2440 偏移地址左移1位- */
	volatile unsigned short* pNorFlashAddr = (volatile unsigned short*)(base + (offset << 1));
	
	return *pNorFlashAddr;
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_Write_Command(unsigned int offset, unsigned short cmd)
 @Description			S3C2440_NorFlash_Write_Command				: Nor Flash 写入指令
 @Input				offset									: 偏移地址
					cmd										: 指令值
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_Write_Command(unsigned int offset, unsigned short cmd)
{
	S3C2440_NorFlash_ChipWrite_HalfWord(NOR_FLASH_BASE_ADDR, offset, cmd);
}

/**********************************************************************************************************
 @Function			static unsigned short S3C2440_NorFlash_Read_Data(unsigned int offset)
 @Description			S3C2440_NorFlash_Read_Data					: Nor Flash 读取数据
 @Input				offset									: 偏移地址
 @Return				val										: 数据值
**********************************************************************************************************/
static unsigned short S3C2440_NorFlash_Read_Data(unsigned int offset)
{
	return S3C2440_NorFlash_ChipRead_HalfWord(NOR_FLASH_BASE_ADDR, offset);
}







/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doScanNorFlash(void)
 @Description			S3C2440_NorFlash_doScanNorFlash				: Nor Flash 读取信息
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doScanNorFlash(void)
{
	char qryAsciiStr[4];
	
	/* -进入CFI模式- */
	S3C2440_NorFlash_Write_Command(0x55, 0x98);
	
	/* -读取QRY标准字符值- */
	qryAsciiStr[0] = S3C2440_NorFlash_Read_Data(0x10);
	qryAsciiStr[1] = S3C2440_NorFlash_Read_Data(0x11);
	qryAsciiStr[2] = S3C2440_NorFlash_Read_Data(0x12);
	qryAsciiStr[3] = '\0';
	
	printf("\r\nqryAsciiStr = %s\r\n\r\n", qryAsciiStr);
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doEraseNorFlash(void)
 @Description			S3C2440_NorFlash_doEraseNorFlash				: Nor Flash 擦除
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doEraseNorFlash(void)
{
	
	
	
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doWriteNorFlash(void)
 @Description			S3C2440_NorFlash_doWriteNorFlash				: Nor Flash 写入
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doWriteNorFlash(void)
{
	
	
	
}

/**********************************************************************************************************
 @Function			static void S3C2440_NorFlash_doReadNorFlash(void)
 @Description			S3C2440_NorFlash_doReadNorFlash				: Nor Flash 读取
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_NorFlash_doReadNorFlash(void)
{
	
	
	
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
