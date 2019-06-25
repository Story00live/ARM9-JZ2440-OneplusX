/**
  *********************************************************************************************************
  * @file    s3c2440_i2c_controller.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-22
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 GPE14 --> IIC SCL
  *					 GPE15 --> IIC SDA
  *********************************************************************************************************
  */

#include "s3c2440_i2c_controller.h"
#include "interrupt.h"

static p_i2c_msg p_cur_msg;

void S3C2440_I2C_Irq(unsigned int irqBit);

/**********************************************************************************************************
 @Function			int S3C2440_I2C_Controller_Init(void)
 @Description			S3C2440_I2C_Controller_Init			: I2C控制初始化
 @Input				void
 @Return				-1								: Error
					0								: Success
**********************************************************************************************************/
int S3C2440_I2C_Controller_Init(void)
{
	/* -配置I2C引脚- */
	GPECON &= ~((3<<(14*2)) | (3<<(15*2)));
	GPECON |=  ((2<<(14*2)) | (2<<(15*2)));
	
	/* -配置I2C时钟- */
	/* -[7:7] : IIC-bus acknowledge enable bit, 1-enable In Rx mode, the IICSDA is L in the ack time-
	 * -[6:6] : Source clock of IIC-bus transmit clock prescaler selection bit, 0: IICCLK = fPCLK /16 1: IICCLK = fPCLK /512-
	 * -[5:5] : IIC-Bus Tx/Rx interrupt enable/disable bit, 0: Disable, 1: Enable-
	 * -[4:4] : IIC-bus Tx/Rx interrupt pending flag, 读取时1为中断发生, 写0清除中断恢复I2C-
	 * -[3:0] : Tx clock = IICCLK/(IICCON[3:0]+1)-
	 * -Tx Clock = 50Mhz / 16 / (IICCON[3:0]+1) ~= 100Khz-
	 */
	IICCON = (1<<7) | (0<<6) | (1<<5) | (30<<0);
	
	/* -注册I2C中断处理函数- */
	S3C2440_IRQRegister(INT_IIC, S3C2440_I2C_Irq);
	
	return 0;
}

/**********************************************************************************************************
 @Function			static int S3C2440_I2C_Do_Master_Tx(p_i2c_msg msg)
 @Description			S3C2440_I2C_Do_Master_Tx				: I2C发送数据
 @Input				msg								: 消息
 @Return				-1								: Error
					0								: Success
**********************************************************************************************************/
static int S3C2440_I2C_Do_Master_Tx(p_i2c_msg msg)
{
	p_cur_msg = msg;
	
	msg->cnt_transferred = -1;
	msg->err = 0;
	
	/* -设置寄存器启动传输- */
	
	/* -1.配置为 master tx mode- */
	IICCON |= (1<<7);
	IICSTAT = (1<<4);
	
	/* -2.从设备地址写入IICDS- */
	IICDS = (msg->addr<<1) | (I2C_CONTROLLER_WRITE<<0);
	
	/* -3.数据即被传输, 将导致中断产生- */
	IICSTAT = (3<<6) | (1<<5) | (1<<4);
	
	/* -循环等待中断处理完成- */
	while ((!msg->err) && (msg->cnt_transferred != msg->len));
	
	return (msg->err) ? -1 : 0;
}

/**********************************************************************************************************
 @Function			static int S3C2440_I2C_Do_Master_Rx(p_i2c_msg msg)
 @Description			S3C2440_I2C_Do_Master_Rx				: I2C接收数据
 @Input				msg								: 消息
 @Return				-1								: Error
					0								: Success
**********************************************************************************************************/
static int S3C2440_I2C_Do_Master_Rx(p_i2c_msg msg)
{
	p_cur_msg = msg;
	
	msg->cnt_transferred = -1;
	msg->err = 0;
	
	/* -设置寄存器启动传输- */
	
	/* -1.配置为 master rx mode- */
	IICCON |= (1<<7);
	IICSTAT = (1<<4);
	
	/* -2.从设备地址写入IICDS- */
	IICDS = (msg->addr<<1) | (I2C_CONTROLLER_READ<<0);
	
	/* -3.数据即被传输, 将导致中断产生- */
	IICSTAT = (2<<6) | (1<<5) | (1<<4);
	
	/* -循环等待中断处理完成- */
	while ((!msg->err) && (msg->cnt_transferred != msg->len));
	
	return (msg->err) ? -1 : 0;
}

/**********************************************************************************************************
 @Function			int S3C2440_I2C_Master_Xfer(p_i2c_msg msgs, int num)
 @Description			S3C2440_I2C_Master_Xfer				: I2C输出处理
 @Input				msg								: 消息
					num								: 消息号
 @Return				-1								: Error
					0								: Success
**********************************************************************************************************/
int S3C2440_I2C_Master_Xfer(p_i2c_msg msgs, int num)
{
	for (int i = 0; i < num; i++) {
		int rc;
		rc = (msgs[i].flags == I2C_CONTROLLER_WRITE) ? S3C2440_I2C_Do_Master_Tx(&msgs[i]) : S3C2440_I2C_Do_Master_Rx(&msgs[i]);
		if (rc) return rc;
	}
	
	return 0;
}

struct i2c_controller s3c2440_i2c_controller = {
	.name			= "s3c2440",
	.init			= S3C2440_I2C_Controller_Init,
	.master_xfer		= S3C2440_I2C_Master_Xfer
};

/**********************************************************************************************************
 @Function			void S3C2440_I2C_Controller_Register(void)
 @Description			S3C2440_I2C_Controller_Register		: I2C控制注册
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_I2C_Controller_Register(void)
{
	I2C_Controller_register(&s3c2440_i2c_controller);
}

/**********************************************************************************************************
 @Function			static int S3C2440_I2C_is_LastData(void)
 @Description			S3C2440_I2C_is_LastData				: I2C检测是否为最后一个数据
 @Input				void
 @Return				1								: is
					0								: no
**********************************************************************************************************/
static int S3C2440_I2C_is_LastData(void)
{
	return (p_cur_msg->cnt_transferred == (p_cur_msg->len - 1)) ? 1 : 0;
}

/**********************************************************************************************************
 @Function			static void S3C2440_I2C_Resume_With_Ack(void)
 @Description			S3C2440_I2C_Resume_With_Ack			: I2C应答ACK
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_I2C_Resume_With_Ack(void)
{
	unsigned int i2cIICCON = IICCON;
	/* -有应答ACK- */
	i2cIICCON |=  (1<<7);
	/* -Clear Interrupt Pending Flag- */
	i2cIICCON &= ~(1<<4);
	
	IICCON = i2cIICCON;
}

/**********************************************************************************************************
 @Function			static void S3C2440_I2C_Resume_Without_Ack(void)
 @Description			S3C2440_I2C_Resume_Without_Ack		: I2C不应答ACK
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_I2C_Resume_Without_Ack(void)
{
	unsigned int i2cIICCON = IICCON;
	/* -无应答ACK- */
	i2cIICCON &= ~(1<<7);
	/* -Clear Interrupt Pending Flag- */
	i2cIICCON &= ~(1<<4);
	
	IICCON = i2cIICCON;
}

/**********************************************************************************************************
 @Function			void S3C2440_I2C_Irq(unsigned int irqBit)
 @Description			S3C2440_I2C_Irq
 @Input				irqBit
 @Return				void
 @attention			每传输一个数据产生一个中断
					对于每次传输, 第1个中断是"已经发出了设备地址"
**********************************************************************************************************/
void S3C2440_I2C_Irq(unsigned int irqBit)
{
	unsigned int i2cIICSTAT = IICSTAT;
	
	p_cur_msg->cnt_transferred++;
	
	/* Write */
	if (p_cur_msg->flags == I2C_CONTROLLER_WRITE) {
		/* -对于第1个中断, 是发送出设备地址后产生的-
		 * -需判断是否有ACK-
		 * -有ACK : 设备存在, 发送下一个数据-
		 * -无ACK : 无设备, 出错, 结束传输-
		 */
		if (p_cur_msg->cnt_transferred == 0) {
			if (i2cIICSTAT & (1<<0)) {
				/* -No ACK- */
				p_cur_msg->err = -1;
				/* -Stop I2C- */
				IICSTAT =  (3<<6) | (0<<5) | (1<<4);
				/* -Clear Interrupt Pending Flag- */
				IICCON &= ~(1<<4);
				/* -Wait until the stop condition takes effect- */
				delay_simulate(1000);
				return;
			}
		}
		
		/* -对于其他中断, 继续发送下一个数据- */
		if (p_cur_msg->cnt_transferred < p_cur_msg->len) {
			/* Write Next Data */
			IICDS = p_cur_msg->buf[p_cur_msg->cnt_transferred];
			/* -Clear Interrupt Pending Flag- */
			IICCON &= ~(1<<4);
		}
		else {
			/* Write Over */
			IICSTAT =  (3<<6) | (0<<5) | (1<<4);
			/* -Clear Interrupt Pending Flag- */
			IICCON &= ~(1<<4);
			/* -Wait until the stop condition takes effect- */
			delay_simulate(1000);
		}
	}
	
	/* Read */
	if (p_cur_msg->flags == I2C_CONTROLLER_READ) {
		/* -对于第1个中断, 是发送出设备地址后产生的-
		 * -需判断是否有ACK-
		 * -有ACK : 设备存在, 恢复I2C传输, 在下一个中断才可读取第一个数据-
		 * -无ACK : 无设备, 出错, 结束传输-
		 */
		if (p_cur_msg->cnt_transferred == 0) {
			if (i2cIICSTAT & (1<<0)) {
				/* -No ACK- */
				p_cur_msg->err = -1;
				/* -Stop I2C- */
				IICSTAT =  (2<<6) | (0<<5) | (1<<4);
				/* -Clear Interrupt Pending Flag- */
				IICCON &= ~(1<<4);
				/* -Wait until the stop condition takes effect- */
				delay_simulate(1000);
				return;
			}
			else {
				/* -Is ACK- */
				/* -如果为最后一个数据, 启动传输时需设置为不回应ACK- */
				/* -恢复I2C传输- */
				if (S3C2440_I2C_is_LastData())
					S3C2440_I2C_Resume_Without_Ack();
				else
					S3C2440_I2C_Resume_With_Ack();
				return;
			}
		}
		
		/* -对于非第1个中断, 表示读取到一个新数据- */
		if (p_cur_msg->cnt_transferred < p_cur_msg->len) {
			/* Read Now Data */
			int index = p_cur_msg->cnt_transferred - 1;
			p_cur_msg->buf[index] = IICDS;
			/* -如果为最后一个数据, 启动传输时需设置为不回应ACK- */
			/* -恢复I2C传输- */
			if (S3C2440_I2C_is_LastData())
				S3C2440_I2C_Resume_Without_Ack();
			else
				S3C2440_I2C_Resume_With_Ack();
		}
		else {
			/* Read Over */
			IICSTAT =  (2<<6) | (0<<5) | (1<<4);
			/* -Clear Interrupt Pending Flag- */
			IICCON &= ~(1<<4);
			/* -Wait until the stop condition takes effect- */
			delay_simulate(1000);
		}
	}
}

/********************************************** END OF FLEE **********************************************/
