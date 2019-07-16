/**
  *********************************************************************************************************
  * @file    ds18b20.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-07-14
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *
  *********************************************************************************************************
  */

#include "ds18b20.h"
#include "timer.h"

/* -Rom Commands- */
#define SEARCH_ROM				0xF0
#define READ_ROM				0x33
#define MATCH_ROM				0x55
#define SKIP_ROM				0xCC
#define ALARM_ROM				0xEC

/* -Functions Commands- */
#define CONVERT_TEAMPERATURE		0x44
#define WRITE_SCRATCHPAD			0x4E
#define READ_SCRATCHPAD			0xBE
#define COPY_SCRATCHPAD			0x48
#define RECALL_EEPROM			0xB8
#define READ_POWER_SUPPLY		0xB4

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_DataConfig_as_Output(void)
 @Description			S3C2440_DS18B20_DataConfig_as_Output
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_DataConfig_as_Output(void)
{
	/* -GPG6: DS18B20 Data- */
	GPGCON &= ~(3<<(2*6));
	GPGCON |=  (1<<(2*6));
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_DataConfig_as_Input(void)
 @Description			S3C2440_DS18B20_DataConfig_as_Input
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_DataConfig_as_Input(void)
{
	/* -GPG6: DS18B20 Data- */
	GPGCON &= ~(3<<(2*6));
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Data_Set(int val)
 @Description			S3C2440_DS18B20_Data_Set
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Data_Set(int val)
{
	if (val)
		GPGDAT |=  (1<<6);
	else
		GPGDAT &= ~(1<<6);
}

/**********************************************************************************************************
 @Function			static int S3C2440_DS18B20_Data_Get(void)
 @Description			S3C2440_DS18B20_Data_Get
 @Input				void
 @Return				val
**********************************************************************************************************/
static int S3C2440_DS18B20_Data_Get(void)
{
	if (GPGDAT & (1<<6))
		return 1;
	else
		return 0;
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Data_SetVal_for_Time(int val, int us)
 @Description			S3C2440_DS18B20_Data_SetVal_for_Time
 @Input				val
					us
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Data_SetVal_for_Time(int val, int us)
{
	S3C2440_DS18B20_DataConfig_as_Output();
	S3C2440_DS18B20_Data_Set(val);
	S3C2440_TIMER0_uDelay(us);
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Data_Release(void)
 @Description			S3C2440_DS18B20_Data_Release
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Data_Release(void)
{
	S3C2440_DS18B20_DataConfig_as_Input();
}

/**********************************************************************************************************
 @Function			static int S3C2440_DS18B20_Initialization(void)
 @Description			S3C2440_DS18B20_Initialization
 @Input				void
 @Return				int
**********************************************************************************************************/
static int S3C2440_DS18B20_Initialization(void)
{
	int val;
	
	S3C2440_DS18B20_Data_SetVal_for_Time(0, 500);
	S3C2440_DS18B20_Data_Release();
	S3C2440_TIMER0_uDelay(80);
	
	val = S3C2440_DS18B20_Data_Get();
	S3C2440_TIMER0_uDelay(250);
	
	return val;
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Write_Bit(int val)
 @Description			S3C2440_DS18B20_Write_Bit
 @Input				val
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Write_Bit(int val)
{
	if (0 == val) {
		S3C2440_DS18B20_Data_SetVal_for_Time(0, 60);
		S3C2440_DS18B20_Data_Release();
		S3C2440_TIMER0_uDelay(2);
	}
	else {
		S3C2440_DS18B20_Data_SetVal_for_Time(0, 2);
		S3C2440_DS18B20_Data_Release();
		S3C2440_TIMER0_uDelay(60);
	}
}

/**********************************************************************************************************
 @Function			static int S3C2440_DS18B20_Read_Bit(void)
 @Description			S3C2440_DS18B20_Read_Bit
 @Input				void
 @Return				val
**********************************************************************************************************/
static int S3C2440_DS18B20_Read_Bit(void)
{
	int val;
	
	S3C2440_DS18B20_Data_SetVal_for_Time(0, 2);
	S3C2440_DS18B20_Data_Release();
	S3C2440_TIMER0_uDelay(10);
	val = S3C2440_DS18B20_Data_Get();
	S3C2440_TIMER0_uDelay(50);
	
	return val;
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Write_Byte(unsigned char data)
 @Description			S3C2440_DS18B20_Write_Byte
 @Input				data
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Write_Byte(unsigned char data)
{
	for (int i = 0; i < 8; i++) {
		S3C2440_DS18B20_Write_Bit(data & (1<<i));
	}
}

/**********************************************************************************************************
 @Function			static unsigned char S3C2440_DS18B20_Read_Byte(void)
 @Description			S3C2440_DS18B20_Read_Byte
 @Input				void
 @Return				data
**********************************************************************************************************/
static unsigned char S3C2440_DS18B20_Read_Byte(void)
{
	unsigned char data = 0;
	
	for (int i = 0; i < 8; i++) {
		if (S3C2440_DS18B20_Read_Bit() == 1) data |= (1<<i);
	}
	
	return data;
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Write_Rom_Cmd(unsigned char cmd)
 @Description			S3C2440_DS18B20_Write_Rom_Cmd
 @Input				cmd
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Write_Rom_Cmd(unsigned char cmd)
{
	S3C2440_DS18B20_Write_Byte(cmd);
}

/**********************************************************************************************************
 @Function			static void S3C2440_DS18B20_Write_Function_Cmd(unsigned char cmd)
 @Description			S3C2440_DS18B20_Write_Function_Cmd
 @Input				cmd
 @Return				void
**********************************************************************************************************/
static void S3C2440_DS18B20_Write_Function_Cmd(unsigned char cmd)
{
	S3C2440_DS18B20_Write_Byte(cmd);
}

/**********************************************************************************************************
 @Function			int S3C2440_DS18B20_Read_Rom(unsigned char rom[])
 @Description			S3C2440_DS18B20_Read_Rom
 @Input				rom[]
 @Return				int
**********************************************************************************************************/
int S3C2440_DS18B20_Read_Rom(unsigned char rom[])
{
	if (S3C2440_DS18B20_Initialization() != 0) {
		return -1;
	}
	
	S3C2440_DS18B20_Write_Rom_Cmd(READ_ROM);
	
	for (int i = 0; i < 8; i++) {
		rom[i] = S3C2440_DS18B20_Read_Byte();
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			int S3C2440_DS18B20_Wait_When_Processing(int timeoutUs)
 @Description			S3C2440_DS18B20_Wait_When_Processing
 @Input				timeoutUs
 @Return				int
**********************************************************************************************************/
int S3C2440_DS18B20_Wait_When_Processing(int timeoutUs)
{
	while (timeoutUs--) {
		if (S3C2440_DS18B20_Read_Bit() == 1) return 0;
		S3C2440_TIMER0_uDelay(1);
	}
	
	return -1;
}

/**********************************************************************************************************
 @Function			int S3C2440_DS18B20_Start_Convert(void)
 @Description			S3C2440_DS18B20_Start_Convert
 @Input				void
 @Return				int
**********************************************************************************************************/
int S3C2440_DS18B20_Start_Convert(void)
{
	if (S3C2440_DS18B20_Initialization() != 0) {
		return -1;
	}
	
	S3C2440_DS18B20_Write_Rom_Cmd(SKIP_ROM);
	S3C2440_DS18B20_Write_Function_Cmd(CONVERT_TEAMPERATURE);
	
	/* -等待/判断转换成功- */
	if (0 != S3C2440_DS18B20_Wait_When_Processing(1000000)) {
		return -1;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			int S3C2440_DS18B20_Read_Ram(unsigned char ram[])
 @Description			S3C2440_DS18B20_Read_Ram
 @Input				ram[]
 @Return				int
**********************************************************************************************************/
int S3C2440_DS18B20_Read_Ram(unsigned char ram[])
{
	if (S3C2440_DS18B20_Initialization() != 0) {
		return -1;
	}
	
	S3C2440_DS18B20_Write_Rom_Cmd(SKIP_ROM);
	S3C2440_DS18B20_Write_Function_Cmd(READ_SCRATCHPAD);
	
	for (int i = 0; i < 9; i++) {
		ram[i] = S3C2440_DS18B20_Read_Byte();
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			void S3C2440_DS18B20Initialized(void)
 @Description			S3C2440_DS18B20Initialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_DS18B20Initialized(void)
{
	S3C2440_DS18B20_Data_Release();
}

/**********************************************************************************************************
 @Function			int S3C2440_DS18B20ReadTemperature(double* temp)
 @Description			S3C2440_DS18B20ReadTemperature
 @Input				temp
 @Return				int
**********************************************************************************************************/
int S3C2440_DS18B20ReadTemperature(double* temp)
{
	int err = 0;
	unsigned char ram[9];
	int i;
	double sum = 0;
	double val[] = {0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64};
	
	err = S3C2440_DS18B20_Start_Convert();
	if (err) return err;
	
	err = S3C2440_DS18B20_Read_Ram(ram);
	if (err) return err;
	
	if (ram[4] & (3<<5) == 0) {
		i = 3;
	}
	else if (ram[4] & (3<<5) == (1<<5)) {
		i = 2;
	}
	else if (ram[4] & (3<<5) == (2<<5)) {
		i = 1;
	}
	else {
		i = 0;
	}
	
	for (; i < 8; i++) {
		if (ram[0] & (1<<i)) sum += val[i];
	}
	
	for (i = 0; i < 3; i++) {
		if (ram[1] & (1<<i)) sum += val[8+i];
	}
	
	if (ram[1] & (1<<3)) sum = 0 - sum;
	
	*temp = sum;
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
