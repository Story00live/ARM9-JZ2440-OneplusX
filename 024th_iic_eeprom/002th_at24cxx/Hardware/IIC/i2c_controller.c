/**
  *********************************************************************************************************
  * @file    i2c_controller.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-22
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *
  *********************************************************************************************************
  */

#include "i2c_controller.h"
#include "s3c2440_i2c_controller.h"

static p_i2c_controller p_i2c_controller_array[I2C_CONTROLLER_NUM];
static p_i2c_controller p_i2c_controller_selected;

/**********************************************************************************************************
 @Function			int I2C_Controller_register(p_i2c_controller pi2ccon)
 @Description			I2C_Controller_register			: I2C控制注册
 @Input				pi2ccon						: i2c控制参数
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int I2C_Controller_register(p_i2c_controller pi2ccon)
{
	for (int i = 0; i < I2C_CONTROLLER_NUM; i++) {
		if (!p_i2c_controller_array[i]) {
			p_i2c_controller_array[i] = pi2ccon;
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int I2C_Controller_select(char* name)
 @Description			I2C_Controller_select			: I2C选择控制
 @Input				name							: i2c控制名
 @Return				-1							: Error
					!-1							: Success
**********************************************************************************************************/
int I2C_Controller_select(char* name)
{
	for (int i = 0; i < I2C_CONTROLLER_NUM; i++) {
		if (p_i2c_controller_array[i] && !strcmp(p_i2c_controller_array[i]->name, name)) {
			p_i2c_controller_selected = p_i2c_controller_array[i];
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int I2C_Controller_init(void)
 @Description			I2C_Controller_init				: I2C控制初始化
 @Input				void
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int I2C_Controller_init(void)
{
	if (p_i2c_controller_selected) {
		return p_i2c_controller_selected->init();
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int I2C_Controller_transfer(p_i2c_msg msgs, int num)
 @Description			I2C_Controller_transfer			: I2C控制数据传输
 @Input				msgs							: 消息
					num							: 消息号
 @Return				-1							: Error
					0							: Success
**********************************************************************************************************/
int I2C_Controller_transfer(p_i2c_msg msgs, int num)
{
	if (p_i2c_controller_selected) {
		return p_i2c_controller_selected->master_xfer(msgs, num);
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			void I2C_Controller_add(void)
 @Description			I2C_Controller_add				: I2C控制器增加
 @Input				void
 @Return				void
**********************************************************************************************************/
void I2C_Controller_add(void)
{
	S3C2440_I2C_Controller_Register();
}

/********************************************** END OF FLEE **********************************************/
