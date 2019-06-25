#ifndef __AT24CXX_H
#define   __AT24CXX_H

#include "s3c2440_soc.h"

#define AT24CXX_DEVICE_ADDR		0x0050

#define AT24C02_BASE_ADDR		0x0000
#define AT24C02_BYTE_SIZE		0x0100

int S3C2440_AT24CXX_Write(unsigned int addr, unsigned char* data, int len);		//AT24CXX写入
int S3C2440_AT24CXX_Read(unsigned int addr, unsigned char* data, int len);			//AT24CXX读取

#endif /* __AT24CXX_H */
