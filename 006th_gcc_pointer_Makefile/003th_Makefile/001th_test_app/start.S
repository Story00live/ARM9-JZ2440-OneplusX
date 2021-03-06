/**
  *********************************************************************************************************
  * @File               : start.S
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-02-018 21:00:00
  * @Brief              : start
  *********************************************************************************************************
  * @attention
  *
  *********************************************************************************************************
  */

.text
.global _start

_start:
		/* -关闭看门狗- */
		ldr		r0,		=0x53000000
		ldr		r1,		=0x00000000
		str		r1,		[r0]
		
		/* -设置内存: 栈SP- 
		 * -分辨是nor/nand启动-
		 * -写0到0地址, 再读出来-
		 * -如果读到0, 表示0地址上的内容被修改了, 对应片上ram, 为nand启动-
		 * -如果读到非0, 表示0地址上的内容没被修改, 对应片外nor, 为nor启动-
		 */
		mov		r1,		#0
		ldr		r0,		[r1]											/* 读取0地址原数据备份 */
		str		r1,		[r1]											/* 写0到0地址 */
		ldr		r2,		[r1]											/* 读取0地址当前数据 */
		cmp		r1,		r2											/* 比较读取r2值是否与写入r1值相同 */
		ldr		sp,		=0x40000000+4096								/* 设置sp栈地址为0x40000000+4096, 此地址为nor启动sram地址 */
		ldreq	sp,		=4096										/* 当r2值与r1值相同, 设置sp栈地址为4096, 此地址为nand启动sram地址 */
		streq	r0,		[r1]											/* 当r2值与r1值相同, 将读取的备份数据写回0地址 */
		
		@ /* -传递参数给执行函数- */
		@ mov		r0,		#1
		@ /* -调用led_on函数- */
		@ bl		led_on
		
		@ /* -传递参数给执行函数- */
		@ ldr		r0,		=0x0005FFFF
		@ /* -简单延时函数- */
		@ bl		delay
		
		@ /* -传递参数给执行函数- */
		@ mov		r0,		#0
		@ /* -调用led_on函数- */
		@ bl		led_on
		
		/* -调用main函数- */
		bl		main
halt:
		/* -死循环- */
		b		halt
		
/********************************************** END OF FLEE **********************************************/
