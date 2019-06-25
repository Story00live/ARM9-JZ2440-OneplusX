#ifndef __I2C_CONTROLLER_H
#define   __I2C_CONTROLLER_H

#include "s3c2440_soc.h"

#define I2C_CONTROLLER_NUM		32

#define I2C_CONTROLLER_WRITE		0
#define I2C_CONTROLLER_READ		1

/* I2C 消息数据 */
typedef struct i2c_msg
{
	unsigned int		addr;
	unsigned int		flags;
	int				len;
	int				cnt_transferred;
	int				err;
	unsigned char*		buf;
}i2c_msg, *p_i2c_msg;

/* I2C 控制器 */
typedef struct i2c_controller
{
	char*			name;
	int				(*init)(void);
	int				(*master_xfer)(p_i2c_msg msgs, int num);
}i2c_controller, *p_i2c_controller;

int I2C_Controller_register(p_i2c_controller pi2ccon);							//I2C控制注册
int I2C_Controller_select(char* name);										//I2C选择控制

int I2C_Controller_init(void);											//I2C控制初始化
int I2C_Controller_transfer(p_i2c_msg msgs, int num);							//I2C控制数据传输

void I2C_Controller_add(void);											//I2C控制器增加

#endif /* __I2C_CONTROLLER_H */
