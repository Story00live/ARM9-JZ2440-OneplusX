#ifndef __OLED_H
#define   __OLED_H

#include "s3c2440_soc.h"

void S3C2440_OLEDInitialized(void);

void S3C2440_OLEDPutChar(int page, int col, char ch);
void S3C2440_OLEDPrint(int page, int col, char* str);

void S3C2440_OLEDClearPage(int page);

#endif /* __OLED_H */
