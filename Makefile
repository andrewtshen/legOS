ARM_CC=arm-none-eabi-gcc
ARM_LD=arm-none-eabi-ld
ARM_OBJCOPY=arm-non-eabi-objcopy
ARM_CFLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -o
ARM_LDFLAGS=-Tstm32.ld -o

sum.elf: startup.o sum.o UART.o
	$(ARM_LD) $(ARM_LDFLAGS) sum.elf startup.o sum.o UART.o

sum.o: sum.c
	$(ARM_CC) $(ARM_CFLAGS) sum.o sum.c

startup.o: startup.c
	$(ARM_CC) $(ARM_CFLAGS) startup.o startup.c

UART.o: UART.c
	$(ARM_CC) $(ARM_CFLAGS) UART.o UART.c

clean:
	rm *.o
	rm *.elf
	rm *.bin
