/**
  *********************************************************************************************************
  * @file    dht11.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-07-14
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 1. 主机发出至少18MS的低脉冲: start信号
  *					 2. start信号变为高, 20-40us之后, dht11会拉低总线维持80us然后拉高80us: 回应信号
  *					 3. 之后就是数据, 逐位发送
  *					 	bit0 : 50us低脉冲, 26-28us高脉冲
  *						bit1 : 50us低脉冲, 70us高脉冲
  *					 4. 数据有40bit: 8bit湿度整数数据+8bit湿度小数数据
  *								+8bit温度整数数据+8bit温度小数数据
  *								+8bit校验和
  *
  *********************************************************************************************************
  */

#include "dht11.h"
#include "timer.h"

/**********************************************************************************************************
 @Function			static void S3C2440_DHT11_DataConfig_as_Output(void)
 @Description			S3C2440_DHT11_DataConfig_as_Output
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DHT11_DataConfig_as_Output(void)
{
	/* -GPG5: DHT11 Data- */
	GPGCON &= ~(3<<(2*5));
	GPGCON |=  (1<<(2*5));
}

/**********************************************************************************************************
 @Function			static void S3C2440_DHT11_DataConfig_as_Input(void)
 @Description			S3C2440_DHT11_DataConfig_as_Input
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DHT11_DataConfig_as_Input(void)
{
	/* -GPG5: DHT11 Data- */
	GPGCON &= ~(3<<(2*5));
}

/**********************************************************************************************************
 @Function			static void S3C2440_DHT11_Data_Set(int val)
 @Description			S3C2440_DHT11_Data_Set
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_DHT11_Data_Set(int val)
{
	if (val)
		GPGDAT |=  (1<<5);
	else
		GPGDAT &= ~(1<<5);
}

/**********************************************************************************************************
 @Function			static int S3C2440_DHT11_Data_Get(void)
 @Description			S3C2440_DHT11_Data_Get
 @Input				void
 @Return				val
**********************************************************************************************************/
static int S3C2440_DHT11_Data_Get(void)
{
	if (GPGDAT & (1<<5))
		return 1;
	else
		return 0;
}

/**********************************************************************************************************
 @Function			void S3C2440_DHT11Initialized(void)
 @Description			S3C2440_DHT11Initialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_DHT11Initialized(void)
{
	S3C2440_DHT11_DataConfig_as_Output();
	S3C2440_DHT11_Data_Set(1);
	S3C2440_TIMER0_mDelay(2000);
}

/**********************************************************************************************************
 @Function			static void S3C2440_DHT11_Start(void)
 @Description			S3C2440_DHT11_Start
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DHT11_Start(void)
{
	S3C2440_DHT11_DataConfig_as_Output();
	S3C2440_DHT11_Data_Set(0);
	S3C2440_TIMER0_mDelay(20);
	S3C2440_DHT11_DataConfig_as_Input();
}

/**********************************************************************************************************
 @Function			static int S3C2440_DHT11_WaitAck(void)
 @Description			S3C2440_DHT11_WaitAck
 @Input				void
 @Return				Ack
**********************************************************************************************************/
static int S3C2440_DHT11_WaitAck(void)
{
	S3C2440_TIMER0_uDelay(60);
	return S3C2440_DHT11_Data_Get();
}

/**********************************************************************************************************
 @Function			static int S3C2440_DHT11_WaitVal(int val, int timeoutUs)
 @Description			S3C2440_DHT11_WaitVal
 @Input				val
					timeoutUs
 @Return				Val
**********************************************************************************************************/
static int S3C2440_DHT11_WaitVal(int val, int timeoutUs)
{
	while (timeoutUs--) {
		if (S3C2440_DHT11_Data_Get() == val) return 0;
		S3C2440_TIMER0_uDelay(1);
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			static int S3C2440_DHT11_RecvByte(void)
 @Description			S3C2440_DHT11_RecvByte
 @Input				void
 @Return				Byte
**********************************************************************************************************/
static int S3C2440_DHT11_RecvByte(void)
{
	int data = 0;
	
	for (int i = 0; i < 8; i++) {
		if (S3C2440_DHT11_WaitVal(1, 1000)) {
			return -1;
		}
		
		S3C2440_TIMER0_uDelay(40);
		data <<= 1;
		if (S3C2440_DHT11_Data_Get() == 1) data |= 1;
		
		if (S3C2440_DHT11_WaitVal(0, 1000)) {
			return -1;
		}
	}
	
	return data;
}

/**********************************************************************************************************
 @Function			int S3C2440_DHT11Read(int* pthum, int* ptemp)
 @Description			S3C2440_DHT11Read
 @Input				pthum
					ptemp
 @Return				rc
**********************************************************************************************************/
int S3C2440_DHT11Read(int* pthum, int* ptemp)
{
	unsigned char humH, humL;
	unsigned char tmpH, tmpL;
	unsigned char checkcode;
	
	S3C2440_DHT11_Start();
	
	if (0 != S3C2440_DHT11_WaitAck()) {
		*pthum = 0;
		*ptemp = 0;
		return -1;
	}
	
	if (0 != S3C2440_DHT11_WaitVal(1, 1000)) {
		*pthum = 0;
		*ptemp = 0;
		return -1;
	}
	
	if (0 != S3C2440_DHT11_WaitVal(0, 1000)) {
		*pthum = 0;
		*ptemp = 0;
		return -1;
	}
	
	humH = S3C2440_DHT11_RecvByte();
	humL = S3C2440_DHT11_RecvByte();
	tmpH = S3C2440_DHT11_RecvByte();
	tmpL = S3C2440_DHT11_RecvByte();
	checkcode = S3C2440_DHT11_RecvByte();
	
	S3C2440_DHT11_DataConfig_as_Output();
	S3C2440_DHT11_Data_Set(1);
	
	if (humH + humL + tmpH + tmpL == checkcode) {
		*pthum = humH;
		*ptemp = tmpH;
		S3C2440_TIMER0_mDelay(2000);
		return 0;
	}
	else {
		*pthum = 0;
		*ptemp = 0;
		return -1;
	}
}

/********************************************** END OF FLEE **********************************************/
