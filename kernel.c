#include "UART.h"
#include "priv.h"
#include "test.h"
#include "MPU.h"
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

    writeln_str("Finished User Prog Setup");
    
    /* Load the pointer to the user program start */
    int *p = (int*)((int)_USER_TEXT_START | 0x1);

    /* Set up the MPU */
    MPU_setup();

    /* Switch to user mode */
    switch_to_user();
    // asm volatile("svc 0"); // note: svc was changed to try to set up user
    // writeln_str("User Program Entry Point.");
    
    // int(*userprog)() = (int (*)()) p;

    // int ret = userprog();
    // writeln_int(ret);
}
