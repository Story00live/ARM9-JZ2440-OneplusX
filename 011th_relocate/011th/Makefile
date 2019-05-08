# all: start.o led.o key.o main.o
# 	arm-linux-ld -Ttext 0 $^ -o key_led.elf
# 	arm-linux-objcopy -O binary -S key_led.elf key_led.bin
# 	arm-linux-objdump -D key_led.elf > key_led.dis

# %.o: %.S
# 	arm-linux-gcc -c -o $@ $<

# %.o: %.c
# 	arm-linux-gcc -c -o $@ $<

# clean:
# 	rm *.bin *.o *.elf *.dis

# .PHONY: clean

# -------------------------------------------------- 我是分割线 ----------------------------------------------------------- #

# A  = $(T)
# B := $(T)

# T = This is T

# C  = This is C
# C ?= This is New C

# all:
# 	@echo A = $(A)
# 	@echo B = $(B)
# 	@echo C = $(C)

# -------------------------------------------------- 我是分割线 ----------------------------------------------------------- #

# A = a b c d e f g/
# B = $(foreach f, $(A), $(f).o)									# 将A变量各个元素取出并执行相应操作
# C = $(filter %/, $(A))											# 从A中取出符合格式的值
# D = $(filter-out %/, $(A))										# 从A中取出不符合格式的值
# files = $(wildcard *.c)										# 取出同级目录中符合格式的文件

# files1 = a.c b.c c.c key.c led.c main.c
# files2 = $(wildcard $(files1))									# 取出同级目录中符合files1中定义的文件
# files3 = $(patsubst %.c, %.d, $(files1))							# 取出files1各个元素将符合%.c格式的元素替换成符合%.d格式, 不符合不替换只取出

# all:
# 	@echo A = $(A)
# 	@echo B = $(B)
# 	@echo C = $(C)
# 	@echo D = $(D)
# 	@echo files = $(files)
# 	@echo files1 = $(files1)
# 	@echo files2 = $(files2)
# 	@echo files3 = $(files3)

# -------------------------------------------------- 我是分割线 ----------------------------------------------------------- #

OBJS := start.o lib1funcs.o stdio.o string.o led.o key.o uart.o norflash.o sdram.o main.o
DEP_FILES := $(patsubst %, .%.d, $(OBJS))
DEP_FILES := $(wildcard $(DEP_FILES))

CFLAGS = -ffunction-sections -std=c99 -Werror -I.

all: $(OBJS)
	arm-linux-ld --gc-sections -T main.lds $^ -o main.elf
	arm-linux-objcopy -O binary -S main.elf main.bin
	arm-linux-objdump -D main.elf > main.dis

ifneq ($(DEP_FILES),)
include $(DEP_FILES)
endif

%.o: %.S
	arm-linux-gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

%.o: %.c
	arm-linux-gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

clean:
	rm *.bin *.o *.elf *.dis $(DEP_FILES)

debug:
	@echo DEP_FILES = $(DEP_FILES)

.PHONY: clean debug

# -------------------------------------------------- 我是分割线 ----------------------------------------------------------- #

# 目标文件 : 依赖文件
#	执行命令

# 命令执行条件 : 1. 目标文件不存在; 2. 依赖文件比目标文件新.

# $@  : 目标文件
# $<  : 第一个依赖文件
# $^  : 所有依赖文件
# %.* : 通配符

# .PHONY : 定义假想目标

# =		: 延时变量
# :=		: 即使变量
# ?=		: 延时变量, 如果是第一次定义才起效, 如果在前面该变量已定义, 则忽略这句
# +=		: 附加, 它是即使变量还是延时变量取决于前面的定义

# gcc -M file.c										// 打印出file.c的所有依赖
# gcc -M -MF file.d file.c								// 把依赖写入文件file.d
# gcc -c -o file.o file.c -MD -MF file.d					// 编译生成file.o 并把依赖写入file.d

# CFLAGS = -Werror -I.
# -Werror	: 将所以警告已错误方式提示
# -I		: 编译增加头文件目录

# -------------------------------------------------- 我是分割线 ----------------------------------------------------------- #

# CFLAGS = -std=c99 -Werror -I. -fdata-sections -ffunction-sections

# arm-linux-ld -T main.lds $^ --gc-sections -o main.elf



