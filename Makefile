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
	user2.bin.o \
	download.bin.o \
	launch.o \
	printf.o \
	SVC_Handler.o \
	string.o \
	math.o \
	test.o \
	scanf.o \
	launch_download.o \

USER_OBJFILES := \
	user.o \
	user_startup.o \
	sum.o \
	printf.o \
	string.o \
	UART.o \
	math.o \
	test.o \
	scanf.o \

USER2_OBJFILES := \
	user2.o \
	user_startup.o \
	sum.o \
	printf.o \
	string.o \
	UART.o \
	math.o \
	test.o \
	scanf.o \

DOWNLOAD_OBJFILES := \
	printf.o \
	string.o \
	math.o \
	download.o \
	user_startup.o \

GLOABL_LINKERFILES := \
	mem.ld

ARM_C_FLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -nostdlib -Wall -Wextra -fno-builtin
QEMU_FLAGS=-M lm3s6965evb -m 128M -nographic -serial mon:stdio -no-reboot

KERNEL=kernel

USER=user
USER2=user2

DOWNLOAD=download

.PHONY: all
all: $(KERNEL).bin $(USER).elf $(USER2).elf $(DOWNLOAD).elf

# Compile All Binaries

$(KERNEL).bin: $(KERNEL).elf
	$(ARM_OBJCOPY) -O binary $< $@

$(USER).bin: $(USER).elf
	$(ARM_OBJCOPY) -O binary $< $@

$(USER2).bin: $(USER2).elf
	$(ARM_OBJCOPY) -O binary $< $@

$(DOWNLOAD).bin: $(DOWNLOAD).elf
	$(ARM_OBJCOPY) -O binary $< $@

# Convert to bin.o files

$(USER).bin.o: $(USER).bin
	$(ARM_OBJCOPY) -I binary -B arm -O elf32-littlearm --rename-section .data=.rodata,contents,alloc,load,readonly,data $< $@ 

$(USER2).bin.o: $(USER2).bin
	$(ARM_OBJCOPY) -I binary -B arm -O elf32-littlearm --rename-section .data=.rodata,contents,alloc,load,readonly,data $< $@ 

$(DOWNLOAD).bin.o: $(DOWNLOAD).bin
	$(ARM_OBJCOPY) -I binary -B arm -O elf32-littlearm --rename-section .data=.rodata,contents,alloc,load,readonly,data $< $@ 

# Build Elf Files CHECK: I added .ld files as dependencies

$(KERNEL).elf: $(KERN_OBJFILES) $(KERNEL).ld
	$(ARM_LD) -Tkernel.ld -o $@ $^

$(USER).elf: $(USER_OBJFILES) $(USER).ld
	$(ARM_LD) -Tuser.ld -o $@ $^

$(USER2).elf: $(USER2_OBJFILES) $(USER).ld
	$(ARM_LD) -Tuser.ld -o $@ $^

$(DOWNLOAD).elf: $(DOWNLOAD_OBJFILES) $(DOWNLOAD).ld
	$(ARM_LD) -Tdownload.ld -o $@ $^

# Linker Files

$(KERNEL).ld: $(GLOABL_LINKERFILES)
$(USER).ld: $(GLOABL_LINKERFILES)
$(DOWNLOAD).ld: $(GLOABL_LINKERFILES)

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
