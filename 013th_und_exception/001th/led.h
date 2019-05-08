#ifndef __S3C2440_LED_H
#define   __S3C2440_LED_H

#include "s3c2440_soc.h"

#define	S3C2440_LED1_ON()				GPFDAT &= ~(1<<4)
#define	S3C2440_LED1_OFF()				GPFDAT |=  (1<<4)

#define	S3C2440_LED2_ON()				GPFDAT &= ~(1<<5)
#define	S3C2440_LED2_OFF()				GPFDAT |=  (1<<5)

#define	S3C2440_LED3_ON()				GPFDAT &= ~(1<<6)
#define	S3C2440_LED3_OFF()				GPFDAT |=  (1<<6)

void S3C2440_LedInitialized(void);											//Led初始化

#endif /* __S3C2440_LED_H */
