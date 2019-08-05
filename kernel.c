#include "UART.h"
#include "test.h"
#include "MPU.h"
#include "launch.h"
#include <stdint.h>

extern const uint8_t _binary_user_bin_start[];
extern const uint8_t _binary_user_bin_end[];
extern const uint8_t _binary_user2_bin_start[];
extern const uint8_t _binary_user2_bin_end[];
extern uint8_t _USER_TEXT_START[];
extern uint8_t _USER2_TEXT_START[];

void load_userprog(uint8_t* _user_start_p, const uint8_t* bin_start, const uint8_t *bin_end) {
    for (const uint8_t* i = bin_start; i < bin_end; i++) {
        *(_user_start_p) = *i;
        _user_start_p++;
    }
}

void main() {   
    /* Copy user text from rodata to user prog */
    load_userprog(_USER_TEXT_START, _binary_user_bin_start, _binary_user_bin_end);
    load_userprog(_USER2_TEXT_START, _binary_user2_bin_start, _binary_user2_bin_end);
    
    /* Load the pointer to the user program start */
    // int *p = (int*)((int)_USER_TEXT_START | 0x1);
    // int(*userprog)() = (int (*)()) p;

    // writeln_str("Launch start: ");
    // writeln_hex((int)&launch);

    // /* Set up the MPU */
    MPU_setup();

    /* Switch to user mode and change the stack pointer */

    UART_printf("Which program to enter? \n");
    UART_printf("1. name \n");
    UART_printf("2. animal \n");
    UART_printf("Your choice: ");
    unsigned int choice;
    UART_read_int(&choice);
    if (choice == 1) {
        launch();    
    } else if (choice == 2) {
        launch2();
    }
    
}
