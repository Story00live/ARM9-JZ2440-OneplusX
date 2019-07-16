#ifndef __S3C2440_ADC_H
#define   __S3C2440_ADC_H

#include "s3c2440_soc.h"

#define S3C2440_ADC_AIN0			0
#define S3C2440_ADC_AIN1			1

void S3C2440_AdcInitialized(int channel);
int  S3C2440_AdcRead(int channel);

#endif /* __S3C2440_ADC_H */
