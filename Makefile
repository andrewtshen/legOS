ARM_CC=arm-none-eabi-gcc
ARM_LD=arm-none-eabi-ld
ARM_OBJCOPY=arm-none-eabi-objcopy

ARM_C_FLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -o
ARM_LD_FLAGS=-Tstm32.ld -o
ARM_OBJCOPY_FLAGS=-O binary

sum.bin: sum.elf
	$(ARM_OBJCOPY) $(ARM_OBJCOPY_FLAGS) sum.elf sum.bin

sum.elf: startup.o sum.o UART.o
	$(ARM_LD) $(ARM_LD_FLAGS) sum.elf startup.o sum.o UART.o

sum.o: sum.c
	$(ARM_CC) $(ARM_C_FLAGS) sum.o sum.c

startup.o: startup.c
	$(ARM_CC) $(ARM_C_FLAGS) startup.o startup.c

UART.o: UART.c
	$(ARM_CC) $(ARM_C_FLAGS) UART.o UART.c

clean:
	rm *.o
	rm *.elf
	rm *.bin
