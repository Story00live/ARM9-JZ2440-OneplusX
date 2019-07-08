#ifndef __SPI_S3C2440_H
#define   __SPI_S3C2440_H

#include "s3c2440_soc.h"

void S3C2440_SPI_CtrlInitialized(void);

unsigned char S3C2440_SPI_CtrlSendByte(unsigned char val);
unsigned char S3C2440_SPI_CtrlRecvByte(void);

#endif /* __SPI_S3C2440_H */
