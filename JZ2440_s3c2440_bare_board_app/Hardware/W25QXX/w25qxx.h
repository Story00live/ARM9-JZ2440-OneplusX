#ifndef __W25QXX_H
#define   __W25QXX_H

#include "s3c2440_soc.h"

void S3C2440_W25QxxInitialized(void);

void S3C2440_W25QxxReadID(int* pMID, int* pDID);
void S3C2440_W25QxxEraseSector(unsigned int addr);
void S3C2440_W25QxxProgram(unsigned int addr, unsigned char* buf, int len);
void S3C2440_W25QxxRead(unsigned int addr, unsigned char* buf, int len);

#endif /* __W25QXX_H */
