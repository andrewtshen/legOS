# legOS
legOS, an ARM Cortex M3 OS built piece by piece.

## Instructions
To make use `make`.

To run on qemu use `qemu-system-arm -M lm3s6965evb -m 128M -nographic -serial mon:stdio -kernel main.bin`. 

To run with GDB on startup use `qemu-system-arm -M lm3s6965evb -m 128M -S -s -nographic -serial mon:stdio -kernel main.bin` and connect with `arm-none-eabi-gdb main.elf` followed by `target remote localhost:1234`.

### System Calls
System Calls are located in the `user.h` file.
A list of system calls and their usage are here: 

### Utility Functions