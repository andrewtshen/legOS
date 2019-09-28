PREFIX ?= riscv64-unknown-elf-
DEBUG ?= 1

QEMU := qemu-system-riscv32
QEMUFLAGS := -M sifive_e -serial stdio

CC := $(PREFIX)gcc
GDB := $(PREFIX)gdb
OBJDUMP := $(PREFIX)objdump
OBJDUMPFLAGS := --disassemble-all --source --section-headers --demangle


CFLAGS := -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib -O0
ifeq ($(DEBUG),1)
	CFLAGS += -g
endif

OBJS := \
	startup.o \
	main.o \
	UART.o \
	math.o \
	string.o \
	scanf.o \
	printf.o \


.PHONY: all
all: kernel.elf kernel.lst

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS) kernel.ld
	$(CC) $(CFLAGS) -T kernel.ld $(OBJS) -o $@

%.lst: %.elf
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@

.PHONY: clean
clean:
	rm -f *.o *.lst *.elf

.PHONY: qemu
qemu: kernel.elf
	$(QEMU) $(QEMUFLAGS) -kernel kernel.elf

.PHONY: qemu-gdb
qemu-gdb: kernel.elf
	$(QEMU) $(QEMUFLAGS) -S -s -kernel kernel.elf

.PHONY: gdb
gdb: kernel.elf
	$(GDB) kernel.elf
