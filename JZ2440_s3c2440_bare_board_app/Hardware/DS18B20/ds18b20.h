#ifndef __S3C2440_DS18B20_H
#define   __S3C2440_DS18B20_H

#include "s3c2440_soc.h"

void S3C2440_DS18B20Initialized(void);

int S3C2440_DS18B20ReadTemperature(double* temp);

#endif /* __S3C2440_DS18B20_H */
