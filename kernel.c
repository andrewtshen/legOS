#include "UART.h"
#include "test.h"
#include "MPU.h"
#include "launch.h"
#include <stdint.h>

extern const uint8_t _binary_user_bin_start[];
extern const uint8_t _binary_user_bin_end[];
extern uint8_t _USER_TEXT_START[];

void main() {   
    /* Copy user text from rodata to user prog */
    uint8_t* _user_start_p = _USER_TEXT_START;
    for (const uint8_t* i = _binary_user_bin_start; i < _binary_user_bin_end; i++) {
        *(_user_start_p) = *i;
        _user_start_p++;
    }
    
    /* Load the pointer to the user program start */
    // int *p = (int*)((int)_USER_TEXT_START | 0x1);
    // int(*userprog)() = (int (*)()) p;

    // writeln_str("Launch start: ");
    // writeln_hex((int)&launch);

    // /* Set up the MPU */
    // MPU_setup();

    /* Switch to user mode and change the stack pointer */
    launch();
    
    // asm volatile("svc 0");
    // writeln_str("END OF KERNEL REACHED");
}
