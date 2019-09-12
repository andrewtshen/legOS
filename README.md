# legOS
legOS, an OS built piece by piece for the lm3s6965evb stellaris board with an ARM Cortex M3 processor. It is lightweight, easily modifable, and offers features such as MPU and  simple program downloading.

## Instructions

#### Build Options
##### Make All Files
Use `make`

##### Make and Run
Use `make qemu`

##### Make and Run with GDB
Use `make qemugdb`

##### Mount GDB
Use `make gdb`

##### Build a Binary to be Downloaded to legOS
Use `make download`. The default program name is `download.c`, but you can change this in the `Makefile` as well as the dependencies.

#### I/O
All output is redirected to `guest.out` and all input is redirected to `guest.in`, which are both FIFO special files. To read the output use `cat guest.out` in a seperate window. To input data to `guest.in` you can use any suitable methods to write to the file. One such method is `printf "3" > guest.in`, which will write the number 3 to the operating system. Running `downloader.py` is currently a work in progress.

#### Usage
legOS currently has two testing programs that load from the system ROM memory and are precompiled in. legOS also has functionality that can hold 3 slots for external programs of up to 8kb (this is due to the limited RAM memory and inaccessible extended flash memory/inability to increase RAM memory on QEMU).

Programs loaded onto legOS, must be compiled with the `user.ld` linker script in order to make sure that it is correctly built.

## A Tour of legOS
##### Important Files
`startup.c`: provides the startup process for legOS (modifying setting up bss, rom, ram, and launching the main kernel function) as well as contains fault handlers. This includes the `write` syscall (0) and the `read` syscall (1) in the syscall handler.

`kernel.c`: contains the kernel code for legOS.

`SVC_Handler.s`: implementation of the `SVC_Handler`, which branches back to the `SVCHandler_main` in `startup.c`, where the c implementation of the svc handler lies.

`kernel.ld`: linker script for kernel code.

`UART.c`: interface for the kernel to use UART directly without going through syscalls.

`MPU.c`: implementation for the MPU set up for memory protection enclosing the user data.

`launch.s`: contains code that branches from the kernel code to the user code as well as switches the mode and stack pointer.

`user.c`: a sample user file that is stored in the ROM and can be loaded at runtime to be executed by user.

`user.ld`: linker script for compiling executable binaries on the kernel.

`user_startup.s`: compiled into the user binary, which branches to the main function (depending on how it's compilied it might not be in the same place always, but with our linker script we can force).

`memorylayout.txt`: contains a visual diagram of how the memory is distributed on the machine.

`mem.ld`: contains some general memory marks that are global.

`guest.in`/`guest.out`: FIFO special files used for redirected input and output.

##### Utility
`string.c`: some simple standard c string library functions.

`math.c`: some simple standard c math library functions.

`printf.c`: xv6 styled printf for user usage.

`scanf.c`: interface to `read` syscall usage.

`test.c`: some tests to make sure everything works.`
