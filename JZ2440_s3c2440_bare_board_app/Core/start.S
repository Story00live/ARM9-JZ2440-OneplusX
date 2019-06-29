/**
  *********************************************************************************************************
  * @File               : start.S
  * @Author             : kang.yunjie
  * @version            : V1.2
  * @Date               : 2019-02-18 21:00:00
  * @Brief              : start
  *********************************************************************************************************
  * @attention
  *
  *********************************************************************************************************
  */

.text
.global _start

/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-09 22:00:00
  * @attention          : Jump vector table as in table 3.1 in [1]
  *********************************************************************************************************
  */
		.align	4
_start:
		b		reset												/* Vector Address 0x00000000 : Reset */
		ldr		pc,		_undefined_instruction							/* Vector Address 0x00000004 : Undefined instruction */
		ldr		pc,		_software_interrupt								/* Vector Address 0x00000008 : Software Interrupt */
		ldr		pc,		_prefetch_abort								/* Vector Address 0x0000000C : Prefetch Abort */
		ldr		pc,		_data_abort									/* Vector Address 0x00000010 : Data Abort */
		ldr		pc,		_not_used										/* Vector Address 0x00000014 : Not Used */
		ldr		pc,		_irq											/* Vector Address 0x00000018 : IRQ */
		ldr		pc,		_fiq											/* Vector Address 0x0000001C : FIQ */
		
_undefined_instruction:
		.word	undefined_instruction
_software_interrupt:
		.word	software_interrupt
_prefetch_abort:
		.word	halt
_data_abort:
		.word	halt
_not_used:
		.word	halt
_irq:
		.word	irq
_fiq:
		.word	halt

/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-09 22:00:00
  * @attention          : Startup Code (reset vector)
  *********************************************************************************************************
  */
		.align	4
reset:
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
		
		/* -使能ICache 指令缓存- */
		bl		S3C2440_ICache_Enable
		
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
		
		/* -调用S3C2440_Initialized_NandFlash函数, 初始化Nand Flash- */
		bl		S3C2440_Initialized_NandFlash
		
		/* -调用S3C2440_Initialized_NorFlash函数, 初始化Nor Flash- */
		bl		S3C2440_Initialized_NorFlash
		
		/* -调用S3C2440_Initialized_SDRAM函数, 初始化SDRAM- */
		bl		S3C2440_Initialized_SDRAM
		
		/* -创建页表- */
		bl		S3C2440_MMU_CreatPageTable
		
		/* -启动MMU- */
		bl		S3C2440_MMU_Enable
		
		/* -重定位text, rodata, data段- */
		bl		S3C2440_Initialized_CopyCodeData
		
		/* -清除bss段- */
		bl		S3C2440_Initialized_CleanBssData
		
		/* -复位之后CPU处于SCV管理模式-
		 * -切换到USR用户模式-
		 */
		mrs		r0,		cpsr											/* 读取CPSR值 */
		bic		r0,		r0,	#0x0000001F								/* 低5位清0, M4~M0:00000, 去除当前处理器模式 */
		orr		r0,		r0,	#0x00000010								/* M4~M0:10000, 设置为用户模式 */
		bic		r0,		r0,	#0x00000080								/* 第7位清0, 开启IRQ中断 */
		msr		cpsr_c,	r0											/* 修改CPSR中的控制位 */
		
		/* -设置USR用户模式: 栈SP- */
		ldr		sp,		=0x34000000
		
		/* -跳转到SDRAM程序中运行- */
		ldr		pc,		=sdramCode
sdramCode:
		
/********************************************* und exception *********************************************/
		/* -故意加入一条未定义指令, 使CPU触发Undefined instruction未定义异常- */
		@ .word	0xE3000000
/********************************************* und exception *********************************************/
		
/********************************************* swi exception *********************************************/
		/* -触发SWI异常, 使CPU触发Software Interrupt软件中断异常, 进入0x00000008执行- */
		@ swi		0x00000123
/********************************************* swi exception *********************************************/
		
		/* -调用main函数- */
		@ bl		main													/* 使用BL命令相对跳转, 跳转地址为当前地址PC+偏移量OFFSET, 不为SDRAM中实际运行地址 */
		
		/* -调用main函数- */
		ldr		pc,		=main										/* main地址载入PC, PC为下一条程序运行地址, 绝对跳转, 跳转到SDRAM */
		
halt:
		/* -死循环- */
		b		halt
		
/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-06-29 16:00:00
  * @attention          : Enable ICache
  *********************************************************************************************************
  */
		.align	4
S3C2440_ICache_Enable:
		/* -设置协处理器, 使能ICache- */
		mrc		p15,		0,	r0,	c1,	c0,	0
		orr		r0,		r0,	#(1<<12)
		mcr		p15,		0,	r0,	c1,	c0,	0
		mov		pc,		lr
		
/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-06-29 21:00:00
  * @attention          : Enable MMU
  *********************************************************************************************************
  */
		.align	4
S3C2440_MMU_Enable:
		/* -把页表基地址告诉CP15- */
		ldr		r0,		=0x32000000
		mcr		p15,		0,	r0,	c2,	c0,	0
		
		/* -设置域为0xFFFFFFFF, 不进行权限检查- */
		ldr		r0,		=0xFFFFFFFF
		mcr		p15,		0,	r0,	c3,	c0,	0
		
		/* -使能ICache DCache MMU- */
		mrc		p15,		0,	r0,	c1,	c0,	0
		orr		r0,		r0,	#(1<<12)									/* Enable ICache */
		orr		r0,		r0,	#(1<< 2)									/* Enable DCache */
		orr		r0,		r0,	#(1<< 0)									/* Enable MMU */
		mcr		p15,		0,	r0,	c1,	c0,	0
		
		mov		pc,		lr
		
/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-09 22:00:00
  * @attention          : Undefined Instruction Exception handlers
  *********************************************************************************************************
  */
		.align	4
undefined_instruction:
		/* -执行到这里之前- 
		 * -1. undef模式中 LR 保存有被中断模式中下一调即将执行指令的地址-
		 * -2. undef模式中 SPSR 保存有被中断模式中的CPSR-
		 * -3. 被中断模式中 CPSR 的 M4~M0 被设置为11011, 进入到undef模式-
		 * -4. 跳转到 0x00000004 地址执行程序-
		 */
		
		/* -设置Undefined Instruction异常: 栈SP- */
		ldr		sp,		=0x33F00000
		
		/* -保存现场- */
		stmdb	sp!,		{r0-r12, lr}									/* 在undf异常处理函数中有可能会修改r0~r12, 所以先保存 */
		
		/* -处理Undefined Instruction异常- */
		mrs		r0,		cpsr
		ldr		r1,		=undefInstructionString
		bl		printException
		
		/* -恢复现场- */
		ldmia	sp!,		{r0-r12, pc}^									/* ^:会把undef模式中的spsr值恢复到被中断模式中的cpsr里 */
		
undefInstructionString:
		.string	"Undefined instruction Execption"
		
/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-10 13:00:00
  * @attention          : Software Interrupt Exception handlers
  *********************************************************************************************************
  */
		.align	4
software_interrupt:
		/* -执行到这里之前- 
		 * -1. SCV管理模式中 LR 保存有被中断模式中下一调即将执行指令的地址-
		 * -2. SCV管理模式中 SPSR 保存有被中断模式中的CPSR-
		 * -3. 被中断模式中 CPSR 的 M4~M0 被设置为10011, 进入到SCV管理模式-
		 * -4. 跳转到 0x00000008 地址执行程序-
		 */
		
		/* -设置Software Interrupt异常: 栈SP- */
		ldr		sp,		=0x33E00000
		
		/* -保存现场- */
		stmdb	sp!,		{r0-r12, lr}									/* 在swi异常处理函数中有可能会修改r0~r12, 所以先保存 */
		
		/* 保存 LR 寄存器值 */
		mov		r4,		lr
		
		/* -处理Software Interrupt异常- */
		mrs		r0,		cpsr
		ldr		r1,		=softwareInterruptString
		bl		printException
		
		/* 读出之前保存的 LR 寄存器值 */
		mov		r0,		r4
		sub		r0,		r0,	#4
		bl		printSWIVal
		
		/* -恢复现场- */
		ldmia	sp!,		{r0-r12, pc}^									/* ^:会把SCV管理模式中的spsr值恢复到被中断模式中的cpsr里 */
		
softwareInterruptString:
		.string	"Software Interrupt Execption"
		
/**
  *********************************************************************************************************
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-10 20:00:00
  * @attention          : Interrupt Request
  *********************************************************************************************************
  */
		.align	4
irq:
		/* -执行到这里之前- 
		 * -1. IRQ模式中 LR 保存有被中断模式中下一调即将执行指令的地址-
		 * -2. IRQ模式中 SPSR 保存有被中断模式中的CPSR-
		 * -3. 被中断模式中 CPSR 的 M4~M0 被设置为10010, 进入到IRQ模式-
		 * -4. 跳转到 0x00000018 地址执行程序-
		 */
		
		/* -设置Interrupt Request异常: 栈SP- */
		ldr		sp,		=0x33D00000
		
		/* -保存现场- */
		sub		lr,		lr,	#4
		stmdb	sp!,		{r0-r12, lr}									/* 在irq异常处理函数中有可能会修改r0~r12, 所以先保存, lr-4是异常处理函数返回地址 */
		
		/* -处理Interrupt Request异常- */
		bl		S3C2440_IRQHandler
		
		/* -恢复现场- */
		ldmia	sp!,		{r0-r12, pc}^									/* ^:会把IRQ中断模式中的spsr值恢复到被中断模式中的cpsr里 */
		
/********************************************** END OF FLEE **********************************************/
