all: start.o led.o key.o main.o
	arm-linux-ld -Ttext 0 start.o led.o key.o main.o -o key_led.elf
	arm-linux-objcopy -O binary -S key_led.elf key_led.bin
	arm-linux-objdump -D key_led.elf > key_led.dis

start.o: start.S
	arm-linux-gcc -c -o start.o start.S
	
led.o: led.c
	arm-linux-gcc -c -o led.o led.c
	
key.o: key.c
	arm-linux-gcc -c -o key.o key.c
	
main.o: main.c
	arm-linux-gcc -c -o main.o main.c

clean:
	rm *.bin *.o *.elf *.dis
