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
    write_str("User prog start: ");
    writeln_int((int) _user_start_p);
    for (const uint8_t* i = _binary_user_bin_start; i < _binary_user_bin_end; i++) {
        *(_user_start_p) = *i;
        _user_start_p++;
    }
    write_str("User prog end: ");
    writeln_int((int) _user_start_p);

    // MPU_setup(); /* Set up the MPU */

    switch_to_user();
}
