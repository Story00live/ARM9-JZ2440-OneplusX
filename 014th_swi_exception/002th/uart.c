/**
  *********************************************************************************************************
  * @file    uart.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-01
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "uart.h"

/**********************************************************************************************************
 @Function			void S3C2440_Uart0Initialized(void)
 @Description			S3C2440_Uart0Initialized
 @Input				void
 @Return				void
 @attention			115200, 8, n, 1
					TXD0 : GPH2
					RXD0 : GPH3
**********************************************************************************************************/
void S3C2440_Uart0Initialized(void)
{
	/* 设置串口0使用GPIO引脚 */
	GPHCON &= ~((3<<4) | (3<<6));
	GPHCON |=  ((2<<4) | (2<<6));
	
	/* 设置内部上拉 */
	GPHUP &= ~((1<<2) | (1<<3));
	
	/* 设置波特率
	 * UBRDIVn = (int)( UART clock / ( buad rate x 16) ) –1
	 * UART clock = 50M
	 * UBRDIVn = (int)( 50000000 / ( 115200 x 16) ) –1 = 26
	 * PCLK, 中断/查询模式
	 */
	UCON0 &= ~((3<<0) | (3<<2));
	UCON0 |=  ((1<<0) | (1<<2));
	
	UBRDIV0 = 26;
	
	/* 设置数据格式 8n1: 8位数据位, 无校验位, 1位停止位 */
	ULCON0 &= ~(3<<0);
	ULCON0 |=  (3<<0);
}

/**********************************************************************************************************
 @Function			int S3C2440_Uart0_Putchar(int cByte)
 @Description			S3C2440_Uart0_Putchar
 @Input				cByte
 @Return				int
**********************************************************************************************************/
int S3C2440_Uart0_Putchar(int cByte)
{
	/* 等待发送缓存和移位寄存器空 */
	while (!(UTRSTAT0 & (1<<2)));
	
	/* 数据写入发送缓冲寄存器 */
	UTXH0 = (unsigned char)cByte;
	
	return cByte;
}

/**********************************************************************************************************
 @Function			int S3C2440_Uart0_Getchar(void)
 @Description			S3C2440_Uart0_Getchar
 @Input				void
 @Return				cByte
**********************************************************************************************************/
int S3C2440_Uart0_Getchar(void)
{
	/* 等待接收缓存收到数据 */
	while (!(UTRSTAT0 & (1<<0)));
	
	return URXH0;
}

/**********************************************************************************************************
 @Function			int S3C2440_Uart0_Puts(const char* pString)
 @Description			S3C2440_Uart0_Puts
 @Input				pString
 @Return				strlength
**********************************************************************************************************/
int S3C2440_Uart0_Puts(const char* pString)
{
	int strLength = 0;
	
	while (*pString) {
		S3C2440_Uart0_Putchar(*pString++);
		strLength++;
	}
	
	return strLength;
}

/**********************************************************************************************************
 @Function			int S3C2440_Uart0_Gets(char* pString)
 @Description			S3C2440_Uart0_Gets
 @Input				pString
 @Return				strlength
**********************************************************************************************************/
int S3C2440_Uart0_Gets(char* pString)
{
	int strLength = 0;
	char c;
	
	while ((c = S3C2440_Uart0_Getchar()) != '\n') {
		*pString++ = c;
		strLength++;
	}
	
	return strLength;
}

#if 0
/**********************************************************************************************************
 @Function			int main(int argc, char const *argv[])
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int main(int argc, char const *argv[])
{
	
	return 0;
}
#endif

/********************************************** END OF FLEE **********************************************/
