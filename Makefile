MAKEFLAGS += -r
MAKEFLAGS += -R

ARM_CC=arm-none-eabi-gcc
ARM_LD=arm-none-eabi-ld
ARM_OBJCOPY=arm-none-eabi-objcopy
ARM_QEMU=qemu-system-arm
ARM_GDB=arm-none-eabi-gdb

HEADERFILES := $(wildcard *.h)
KERN_OBJFILES := \
	kernel.o \
	MPU.o \
	startup.o \
	test.o \
	UART.o \
	user.bin.o \
	launch.o \
	printf.o \
	SVC_Handler.o \
	string.o \

USER_OBJFILES := \
	user.o \
	user_startup.o \
	sum.o \
	printf.o \
	string.o \
	UART.o \

GLOABL_LINKERFILES := \
	mem.ld

ARM_C_FLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -nostdlib -Wall -Wextra -fno-builtin
QEMU_FLAGS=-M lm3s6965evb -m 128M -nographic -serial mon:stdio

KERNEL=kernel
USER=user

.PHONY: all
all: $(KERNEL).bin $(USER).elf

$(KERNEL).bin: $(KERNEL).elf
	$(ARM_OBJCOPY) -O binary $< $@

$(USER).bin: $(USER).elf
	$(ARM_OBJCOPY) -O binary $< $@

$(USER).bin.o: $(USER).bin
	$(ARM_OBJCOPY) -I binary -B arm -O elf32-littlearm --rename-section .data=.rodata,contents,alloc,load,readonly,data $< $@ 

$(KERNEL).elf: $(KERN_OBJFILES)
	$(ARM_LD) -Tkernel.ld -o $@ $^

$(USER).elf: $(USER_OBJFILES)
	$(ARM_LD) -Tuser.ld -o $@ $^

$(USER).ld: $(GLOABL_LINKERFILES)
$(KERNEL).ld: $(GLOABL_LINKERFILES)

%.o: %.c $(HEADERFILES)
	$(ARM_CC) $(ARM_C_FLAGS) -o $@ $<

%.o: %.s $(HEADERFILES)
	$(ARM_CC) $(ARM_C_FLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *.elf *.bin

.PHONY: qemu
qemu: $(KERNEL).bin
	$(ARM_QEMU) $(QEMU_FLAGS) -kernel $<

.PHONY: qemugdb
qemugdb: $(KERNEL).bin 
	$(ARM_QEMU) $(QEMU_FLAGS) -S -s -kernel $<

.PHONY: gdb
gdb: $(KERNEL).elf
	$(ARM_GDB) $<
