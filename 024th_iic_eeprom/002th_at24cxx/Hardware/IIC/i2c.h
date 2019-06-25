#ifndef __I2C_H
#define   __I2C_H

#include "s3c2440_soc.h"
#include "s3c2440_i2c_controller.h"

int I2C_Init(void);														//I2C初始化
int I2C_Transfer(p_i2c_msg msgs, int num);									//I2C数据传输

#endif /* __I2C_H */
