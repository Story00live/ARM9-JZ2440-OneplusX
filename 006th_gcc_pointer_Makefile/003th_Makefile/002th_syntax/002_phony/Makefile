all: start.o led.o key.o main.o
	arm-linux-ld -Ttext 0 $^ -o key_led.elf
	arm-linux-objcopy -O binary -S key_led.elf key_led.bin
	arm-linux-objdump -D key_led.elf > key_led.dis

%.o: %.S
	arm-linux-gcc -c -o $@ $<

%.o: %.c
	arm-linux-gcc -c -o $@ $<

clean:
	rm *.bin *.o *.elf *.dis

.PHONY: clean

# 目标文件 : 依赖文件
#	执行命令

# 命令执行条件 : 1. 目标文件不存在; 2. 依赖文件比目标文件新.

# $@  : 目标文件
# $<  : 第一个依赖文件
# $^  : 所有依赖文件
# %.* : 通配符

# .PHONY : 定义假想目标
