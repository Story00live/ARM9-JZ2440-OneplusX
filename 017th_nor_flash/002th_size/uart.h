#ifndef __S3C2440_UART_H
#define   __S3C2440_UART_H

#include "s3c2440_soc.h"

void S3C2440_Uart0Initialized(void);										//串口0初始化

int S3C2440_Uart0_Putchar(int cByte);										//打印一个字符
int S3C2440_Uart0_Getchar(void);											//读取一个字符

int S3C2440_Uart0_Puts(const char* pString);									//输出一串字符串
int S3C2440_Uart0_Gets(char* pString);										//输入一串字符串

#endif /* __S3C2440_UART_H */
