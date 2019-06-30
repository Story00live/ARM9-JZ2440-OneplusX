#ifndef __SPI_GPIO_H
#define   __SPI_GPIO_H

#include "s3c2440_soc.h"

void S3C2440_SPI_GPIOInitialized(void);

unsigned char S3C2440_SPI_GPIOSendByte(unsigned char val);
unsigned char S3C2440_SPI_GPIORecvByte(void);

#endif /* __SPI_GPIO_H */
