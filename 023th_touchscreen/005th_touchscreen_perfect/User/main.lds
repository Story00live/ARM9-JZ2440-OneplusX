/**
  *********************************************************************************************************
  * @File               : main.lds
  * @Author             : kang.yunjie
  * @version            : V1.0
  * @Date               : 2019-03-06 21:00:00
  * @Brief              : lds
  *********************************************************************************************************
  * @attention
  *
  *********************************************************************************************************
  */

OUTPUT_ARCH(arm)														/* 设置输出文件的体系架构 */
ENTRY(_start)															/* 设置入口地址全局符号 */
SECTIONS {															/* 输出文件内容布局 */
	
	. = 0x30000000;													/* 指定初始地址=>运行地址 */
	
	__code_start = .;													/* 程序运行起始地址 */
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.start.text				: 				{						/* .start.text段 */
		Core/start.o					(.text)
	}
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.init.text				: 				{						/* .init.text段 */
		System/Init/init.o				(.init.text)
		System/Interrupt/interrupt.o		(.init.text)
		Hardware/SDRAM/sdram.o			(.init.text)
		Hardware/NANDFLASH/nandflash.o	(.init.text)
		Hardware/NORFLASH/norflash.o		(.init.text)
		User/main.o					(.init.text)
	}
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.text					: 				{						/* .text段 */
		*(.text)
	}
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.rodata					: 				{						/* .rodata段 */
		*(.rodata)
	}
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.data					: 				{						/* .data段 */
		*(.data)
	}
	
	__code_end = .;													/* 程序运行结束地址 */
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	
	__bss_start = .;													/* 重定位后bss段起始地址 */
	
	.bss						: 				{						/* .bss段 */
		*(.bss)
	}
	
	__bss_end = .;														/* 重定位后bss段结束地址 */
	
	. = ALIGN(4);														/* 当前地址4字节对齐 */
	.comment					: 				{						/* .comment段 */
		*(.COMMON)
	}
}

/********************************************** END OF FLEE **********************************************/
