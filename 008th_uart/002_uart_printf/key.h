#ifndef __S3C2440_KEY_H
#define   __S3C2440_KEY_H

#include "s3c2440_soc.h"

#define	S3C2440_KEYS2_READ()			(GPFDAT & (1<<0))
#define	S3C2440_KEYS3_READ()			(GPFDAT & (1<<2))
#define	S3C2440_KEYS4_READ()			(GPGDAT & (1<<3))

void S3C2440_KeyInitialized(void);											//Key初始化

#endif /* __S3C2440_KEY_H */
