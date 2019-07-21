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

    MPU_setup(); /* Set up the MPU */
    writeln_str("Finished MPU setup.");

    // /* User prog entry point */
    // writeln_str("User Program Entry Point.");
    int *p = (int*)((int)_USER_TEXT_START | 0x1); // Toggle low order bit because Thumb
    int(*userprog)() = (int (*)()) p;

    // int *a = (int*)((int)0x0 | 0x1); // Toggle low order bit because Thumb
    // int(*as)() = (int (*)()) a;    

    switch_to_user(); /* Switch to User Mode */
    int ret = userprog();
    // writeln_int(ret);
 
    // writeln_str("END OF MAIN REACHED.");
}
