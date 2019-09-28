MAKEFLAGS += -r
MAKEFLAGS += -R

PREFIX ?= arm-none-eabi-

ARM_CC := $(PREFIX)gcc
ARM_LD := $(PREFIX)ld
ARM_OBJCOPY := $(PREFIX)objcopy
ARM_QEMU = qemu-system-arm
ARM_GDB := $(PREFIX)gdb

HEADERFILES := $(wildcard *.h)

COMMON_OBJFILES := \
	printf.o \
	math.o \
	UART.o \
	scanf.o \
	string.o \
	test.o \

KERN_OBJFILES := \
	kernel.o \
	MPU.o \
	startup.o \
	user.bin.o \
	user2.bin.o \
	download.bin.o \
	launch.o \
	SVC_Handler.o \
	test.o \
	$(COMMON_OBJFILES) \

USER_OBJFILES := \
	user.o \
	user_startup.o \
	$(COMMON_OBJFILES) \

USER2_OBJFILES := \
	user2.o \
	user_startup.o \
	$(COMMON_OBJFILES) \

DOWNLOAD_OBJFILES := \
	download.o \
	user_startup.o \
	$(COMMON_OBJFILES) \

GLOABL_LINKERFILES := \
	mem.ld

ARM_C_FLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -nostdlib -Wall -Wextra -fno-builtin -ffreestanding
QEMU_FLAGS=-M lm3s6965evb -m size=128M -nographic -serial pipe:guest -no-reboot

KERNEL=kernel

USER=user
USER2=user2

DOWNLOAD=download

.PHONY: all
all: $(KERNEL).bin $(USER).bin $(USER2).bin $(DOWNLOAD).bin $(USER).elf $(USER2).elf $(DOWNLOAD).elf

# Compile All Binaries

%.bin: %.elf
	$(ARM_OBJCOPY) -O binary $< $@

# Convert to bin.o files

%.bin.o: %.bin
	$(ARM_OBJCOPY) -I binary -B arm -O elf32-littlearm --rename-section .data=.rodata,contents,alloc,load,readonly,data $< $@ 

# Build Elf Files

$(KERNEL).elf: $(KERN_OBJFILES) $(KERNEL).ld
	$(ARM_LD) -Tkernel.ld -o $@ $^

$(USER).elf: $(USER_OBJFILES) $(USER).ld
	$(ARM_LD) -Tuser.ld -o $@ $^

$(USER2).elf: $(USER2_OBJFILES) $(USER).ld
	$(ARM_LD) -Tuser.ld -o $@ $^

$(DOWNLOAD).elf: $(DOWNLOAD_OBJFILES) $(USER).ld
	$(ARM_LD) -Tuser.ld -o $@ $^

# Linker Files

$(KERNEL).ld: $(GLOABL_LINKERFILES)
$(USER).ld: $(GLOABL_LINKERFILES)

%.o: %.c $(HEADERFILES)
	$(ARM_CC) $(ARM_C_FLAGS) -o $@ $<

%.o: %.s $(HEADERFILES)
	$(ARM_CC) $(ARM_C_FLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *.elf *.bin

.PHONY: download
download: $(DOWNLOAD).bin

.PHONY: qemu
qemu: $(KERNEL).bin
	$(ARM_QEMU) $(QEMU_FLAGS) -kernel $<

.PHONY: qemugdb
qemugdb: $(KERNEL).bin 
	$(ARM_QEMU) $(QEMU_FLAGS) -S -s -kernel $<

.PHONY: gdb
gdb: $(KERNEL).elf
	$(ARM_GDB) $<
