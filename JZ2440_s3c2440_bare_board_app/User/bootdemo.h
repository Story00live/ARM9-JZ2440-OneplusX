#ifndef __BOOT_DEMO_H
#define   __BOOT_DEMO_H

#include "s3c2440_soc.h"

void S3C2440_Boot_Setup_startTag(void);
void S3C2440_Boot_Setup_memoryTag(void);
void S3C2440_Boot_Setup_commandlineTag(char* cmdline);
void S3C2440_Boot_Setup_endTag(void);

#endif /* __BOOT_DEMO_H */
