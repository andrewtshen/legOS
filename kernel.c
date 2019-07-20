#include "UART.h"
#include "priv.h"
#include "test.h"
#include "MPU.h"
#include <stdint.h>

extern const uint8_t _binary_user_bin_start[];
extern const uint8_t _binary_user_bin_end[];
extern uint8_t _USER_TEXT_START[];

void main() {
    /* Testing MPU and Privileges */
    // writeln_str("Testing MPU");
    // test_MPU();
    // writeln_str("---");
    // writeln_str("Testing Privileges");
    // test_privileges();
    // writeln_str("---");

    /* Copy user text from rodata to user prog */
    uint8_t* _user_start_p = _USER_TEXT_START;
    for (const uint8_t* i = _binary_user_bin_start; i < _binary_user_bin_end; i++) {
        *(_user_start_p) = *i;
        _user_start_p++;
    }

    // /* Check if copied correctly */
    // _user_start_p = _USER_TEXT_START;
    // for (int i = 0; i < _binary_user_bin_end - _binary_user_bin_start; i++) {
    //     write_int(*(_user_start_p + i));
    // }
    // writeln_str("\n");

    /* User prog entry point */
    writeln_str("User Program Entry Point.");
    int *p = (int*)((int)_USER_TEXT_START | 0x1);
    int(*userprog)() = (int (*)()) p;

    int ret = userprog();
    writeln_int(ret);
 
    // switch_to_user();

    writeln_str("END OF MAIN REACHED.");
}
