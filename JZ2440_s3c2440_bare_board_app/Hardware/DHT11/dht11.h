#ifndef __S3C2440_DHT11_H
#define   __S3C2440_DHT11_H

#include "s3c2440_soc.h"

void S3C2440_DHT11Initialized(void);

int  S3C2440_DHT11Read(int* pthum, int* ptemp);

#endif /* __S3C2440_DHT11_H */
