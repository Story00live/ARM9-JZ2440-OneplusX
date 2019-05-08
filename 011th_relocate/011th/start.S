/**
  *********************************************************************************************************
  * @File               : start.S
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-02-18 21:00:00
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
		
		/* -设置MPLL, FCLK : HCLK : PCLK = 400M : 100M : 50M- */
		/* -LOCKTIME(0x4C000000) = 0xFFFFFFFF- */
		ldr		r0,		=0x4C000000
		ldr		r1,		=0xFFFFFFFF
		str		r1,		[r0]
		
		/* -CLKDIVN(0x4C000014) = 0x05, tFCLK : tHCLK : tPCLK = 1 : 4 : 8- */
		ldr		r0,		=0x4C000014
		ldr		r1,		=0x00000005
		str		r1,		[r0]
		
		/* -设置CPU工作于异步模式- */
		mrc		p15,		0,	r0,	c1,	c0,	0
		orr		r0,		r0,	#0xc0000000
		mcr		p15,		0,	r0,	c1,	c0,	0
		
		/* -设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0)-
		 * -m = MDIV + 8 = 92 + 8 = 100-
		 * -p = PDIV + 2 = 1 + 2 = 3-
		 * -s = SDIV = 1-
		 * -FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1) = 400M-
		 */
		ldr		r0,		=0x4C000004
		ldr		r1,		=(92<<12)|(1<<4)|(1<<0)
		str		r1,		[r0]
		
		/* -一旦设置PLL,就会锁定lock time直到PLL输出稳定,然后CPU工作于新的频率FCLK- */
		
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
		
		/* -调用S3C2440_NorFlashInitialized函数- */
		bl		S3C2440_NorFlashInitialized
		
		/* -调用S3C2440_SDRAMInitialized函数- */
		bl		S3C2440_SDRAMInitialized
		
		/* -重定位text, rodata, data段- */
		bl		S3C2440_SDRAM_CopyCodeData
		
		/* -清除bss段- */
		bl		S3C2440_SDRAM_CleanBssData
		
		/* -调用main函数- */
		@ bl		main													/* 使用BL命令相对跳转, 跳转地址为当前地址PC+偏移量OFFSET, 不为SDRAM中实际运行地址 */
		
		/* -调用main函数- */
		ldr		pc,		=main										/* main地址载入PC, PC为下一条程序运行地址, 绝对跳转, 跳转到SDRAM */
halt:
		/* -死循环- */
		b		halt
		
/********************************************** END OF FLEE **********************************************/
