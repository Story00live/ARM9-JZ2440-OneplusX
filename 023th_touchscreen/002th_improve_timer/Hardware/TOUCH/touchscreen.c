/**
  *********************************************************************************************************
  * @file    touchscreen.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-06-15
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *
  *********************************************************************************************************
  */

#include "touchscreen.h"
#include "interrupt.h"

#define DEBUG

#ifndef DEBUG
#define Debug_Printf(expr)		((void)0U)
#else
#define Debug_Printf			printfln
#endif

/* ADCTSC's bits */
#define WAIT_PEN_DOWN			(0<<8)
#define WAIT_PEN_UP				(1<<8)

#define YM_ENABLE				(1<<7)
#define YM_DISABLE				(0<<7)

#define YP_ENABLE				(0<<6)
#define YP_DISABLE				(1<<6)

#define XM_ENABLE				(1<<5)
#define XM_DISABLE				(0<<5)

#define XP_ENABLE				(0<<4)
#define XP_DISABLE				(1<<4)

#define PULLUP_ENABLE			(0<<3)
#define PULLUP_DISABLE			(1<<3)

#define AUTO_PST				(1<<2)

#define WAIT_INT_MODE			(3)
#define NO_OPR_MODE				(0)

/**********************************************************************************************************
 @Function			void TouchScreen_Enter_Wait_Pen_Down_Mode(void)
 @Description			TouchScreen_Enter_Wait_Pen_Down_Mode
 @Input				void
 @Return				void
**********************************************************************************************************/
static void TouchScreen_Enter_Wait_Pen_Down_Mode(void)
{
	ADCTSC = WAIT_PEN_DOWN | PULLUP_ENABLE | YM_ENABLE | YP_DISABLE | XP_DISABLE | XM_DISABLE | WAIT_INT_MODE;
}

/**********************************************************************************************************
 @Function			void TouchScreen_Enter_Wait_Pen_Down_Mode(void)
 @Description			TouchScreen_Enter_Wait_Pen_Down_Mode
 @Input				void
 @Return				void
**********************************************************************************************************/
static void TouchScreen_Enter_Wait_Pen_Up_Mode(void)
{
	ADCTSC = WAIT_PEN_UP | PULLUP_ENABLE | YM_ENABLE | YP_DISABLE | XP_DISABLE | XM_DISABLE | WAIT_INT_MODE;
}

/**********************************************************************************************************
 @Function			void TouchScreen_Enter_Auto_Measure_Mode(void)
 @Description			TouchScreen_Enter_Auto_Measure_Mode
 @Input				void
 @Return				void
**********************************************************************************************************/
static void TouchScreen_Enter_Auto_Measure_Mode(void)
{
	ADCTSC = AUTO_PST | NO_OPR_MODE;
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_ADC_Register_Init(void)
 @Description			S3C2440_TouchScreen_ADC_Register_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_TouchScreen_ADC_Register_Init(void)
{
	/* -[15:15] : ECFLG, 1 = End of A/D conversion 0 = A/D conversion in process-
	 * -[14:14] : PRSCEN, 1 = A/D converter prescaler enable 0 disable-
	 * -[13:06] : PRSCVL, adc clk = PCLK / (PRSCVL + 1)-
	 * -[05:03] : SEL_MUX, 000 = AIN 0-
	 * -[02:02] : STDBM, Standby mode select 0 = Normal operation mode 1 = Standby mode-
	 * -[01:01] : READ_ START, A/D conversion start by read-
	 * -[00:00] : ENABLE_START, A/D conversion starts by enable-
	 */
	ADCCON = (1<<14) | (49<<6) | (0<<3);
	
	/* -按下触摸屏, 延时之后再发出TC中断-
	 * -延时时间 = ADCDLY * 晶振周期 = ADCDLY * 1 / 12000000 = 5ms-
	 */
	ADCDLY = 60000;
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_ADC_Interrupt_Init(void)
 @Description			S3C2440_TouchScreen_ADC_Interrupt_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_TouchScreen_ADC_Interrupt_Init(void)
{
	/* -清除中断标志位- */
	SUBSRCPND = (1<<INT_ADC_S) | (1<<INT_TC);
	
	/* 注册中断处理函数 */
	S3C2440_IRQRegister(INT_ADC, S3C2440_TouchScreen_ADC_Irq);
	
	/* -使能ADC,TC中断: 0 = Service available, 1 = Masked- */
	INTSUBMSK &= ~((1<<INT_ADC_S) | (1<<INT_TC));
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_Init(void)
 @Description			S3C2440_TouchScreen_Init
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_TouchScreen_Init(void)
{
	/* -设置触摸屏接口:寄存器- */
	S3C2440_TouchScreen_ADC_Register_Init();
	
	/* -设置中断- */
	S3C2440_TouchScreen_ADC_Interrupt_Init();
	
	/* -让触摸屏控制器进入"等待中断模式"- */
	TouchScreen_Enter_Wait_Pen_Down_Mode();
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_ADC_Isr_Adc(void)
 @Description			S3C2440_TouchScreen_ADC_Isr_Adc
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_TouchScreen_ADC_Isr_Adc(void)
{
	int x = ADCDAT0;
	int y = ADCDAT1;
	
	if (!(x & (1<<15))) {
		x &= 0x03FF;
		y &= 0x03FF;
		
		Debug_Printf("[TouchScreen: X = %08d, Y = %08d.]", x, y);
	}
	
	TouchScreen_Enter_Wait_Pen_Up_Mode();
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_ADC_Isr_Tc(void)
 @Description			S3C2440_TouchScreen_ADC_Isr_Tc
 @Input				void
 @Return				void
**********************************************************************************************************/
static void S3C2440_TouchScreen_ADC_Isr_Tc(void)
{
	if (ADCDAT0 & (1<<15)) {
		Debug_Printf("[TouchScreen: Pen up.]");
		TouchScreen_Enter_Wait_Pen_Down_Mode();
	}
	else {
		Debug_Printf("[TouchScreen: Pen down.]");
#if 0
		TouchScreen_Enter_Wait_Pen_Up_Mode();
#else
		/* -进入"自动测量"模式- */
		TouchScreen_Enter_Auto_Measure_Mode();
		/* -启动ADC- */
		ADCCON |= (1<<0);
#endif
	}
}

/**********************************************************************************************************
 @Function			void S3C2440_TouchScreen_ADC_Irq(unsigned int irq)
 @Description			S3C2440_TouchScreen_ADC_Irq
 @Input				irq
 @Return				void
**********************************************************************************************************/
void S3C2440_TouchScreen_ADC_Irq(unsigned int irq)
{
	if (SUBSRCPND & (1<<INT_ADC_S)) {
		/* -ADC中断- */
		S3C2440_TouchScreen_ADC_Isr_Adc();
		
		/* -清除中断标志位- */
		SUBSRCPND = (1<<INT_ADC_S);
	}
	
	if (SUBSRCPND & (1<<INT_TC)) {
		/* -触摸屏中断- */
		S3C2440_TouchScreen_ADC_Isr_Tc();
		
		/* -清除中断标志位- */
		SUBSRCPND = (1<<INT_TC);
	}
}

/********************************************** END OF FLEE **********************************************/
