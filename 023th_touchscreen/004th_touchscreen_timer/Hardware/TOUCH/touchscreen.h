#ifndef __S3C2440_TOUCHSCREEN_H
#define   __S3C2440_TOUCHSCREEN_H

#include "s3c2440_soc.h"

void S3C2440_TouchScreen_Init(void);
void S3C2440_TouchScreen_ADC_Irq(unsigned int irq);
void S3C2440_TouchScreen_Timer_Irq(void);

#endif /* __S3C2440_TOUCHSCREEN_H */
