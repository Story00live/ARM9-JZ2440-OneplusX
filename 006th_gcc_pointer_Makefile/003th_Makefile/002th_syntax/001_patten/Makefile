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

# 目标文件 : 依赖文件
#	执行命令

# $@  : 目标文件
# $<  : 第一个依赖文件
# $^  : 所有依赖文件
# %.* : 通配符
