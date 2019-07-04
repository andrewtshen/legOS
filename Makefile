MAKEFLAGS += -r
MAKEFLAGS += -R

ARM_CC=arm-none-eabi-gcc
ARM_LD=arm-none-eabi-ld
ARM_OBJCOPY=arm-none-eabi-objcopy

HEADERFILES := $(wildcard *.h)
OBJFILES := $(patsubst %.c, %.o, $(wildcard *.c))

ARM_C_FLAGS=-O0 -c -g -mcpu=cortex-m3 -mthumb -o
ARM_LD_FLAGS=-Tstm32.ld -o
ARM_OBJCOPY_FLAGS=-O binary

MAIN=main

$(MAIN).bin: $(MAIN).elf
	$(ARM_OBJCOPY) $(ARM_OBJCOPY_FLAGS) $< $@

$(MAIN).elf: $(OBJFILES)
	$(ARM_LD) $(ARM_LD_FLAGS) $@ $^

%.o: %.c $(HEADERFILES)
	$(ARM_CC) $(ARM_C_FLAGS) $@ $<

clean:
	rm *.o *.elf *.bin
