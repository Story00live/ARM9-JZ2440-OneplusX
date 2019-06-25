/**
  *********************************************************************************************************
  * @file    at24cxx.c
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

#include "at24cxx.h"
#include "i2c.h"

/**********************************************************************************************************
 @Function			int S3C2440_AT24CXX_Write(unsigned int addr, unsigned char* data, int len)
 @Description			S3C2440_AT24CXX_Write			: AT24CXX写入
 @Input				addr							: 写地址
					data							: 数据
					len							: 长度
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int S3C2440_AT24CXX_Write(unsigned int addr, unsigned char* data, int len)
{
	int rc;
	int writeLen;
	unsigned char twbuf[2];
	i2c_msg msgw;
	
	if ((addr >= AT24C02_BYTE_SIZE) || (len <= 0) || (data == NULL)) return -1;
	
	writeLen = AT24C02_BYTE_SIZE - addr;
	
	if (writeLen > len) writeLen = len;
	
	for (int i = 0; i < writeLen; i++) {
		twbuf[0] = addr++;
		twbuf[1] = data[i];
		
		msgw.addr				= AT24CXX_DEVICE_ADDR;
		msgw.flags			= I2C_CONTROLLER_WRITE;
		msgw.len				= sizeof(twbuf);
		msgw.buf				= twbuf;
		msgw.err				= 0;
		msgw.cnt_transferred	= -1;
		
		rc = I2C_Transfer(&msgw, 1);
		if (rc) return rc;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			int S3C2440_AT24CXX_Read(unsigned int addr, unsigned char* data, int len)
 @Description			S3C2440_AT24CXX_Read			: AT24CXX读取
 @Input				addr							: 读地址
					data							: 数据
					len							: 长度
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int S3C2440_AT24CXX_Read(unsigned int addr, unsigned char* data, int len)
{
	int rc;
	int readLen;
	i2c_msg msgr[2];
	
	if ((addr >= AT24C02_BYTE_SIZE) || (len <= 0) || (data == NULL)) return -1;
	
	readLen = AT24C02_BYTE_SIZE - addr;
	
	if (readLen > len) readLen = len;
	
	msgr[0].addr				= AT24CXX_DEVICE_ADDR;
	msgr[0].flags				= I2C_CONTROLLER_WRITE;
	msgr[0].len				= 1;
	msgr[0].buf				= (unsigned char*)&addr;
	msgr[0].err				= 0;
	msgr[0].cnt_transferred		= -1;
	
	msgr[1].addr				= AT24CXX_DEVICE_ADDR;
	msgr[1].flags				= I2C_CONTROLLER_READ;
	msgr[1].len				= readLen;
	msgr[1].buf				= data;
	msgr[1].err				= 0;
	msgr[1].cnt_transferred		= -1;
	
	rc = I2C_Transfer(&msgr[0], 2);
	if (rc) return rc;
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
