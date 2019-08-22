#include "UART.h"
#include "test.h"
#include "MPU.h"
#include "launch.h"
#include "math.h"
#include <stdint.h>

extern const uint8_t _binary_user_bin_start[];
extern const uint8_t _binary_user_bin_end[];
extern const uint8_t _binary_user2_bin_start[];
extern const uint8_t _binary_user2_bin_end[];
extern uint8_t _USER_TEXT_START[];
extern uint8_t _PROG_SLOT_1[];
extern uint8_t _PROG_SLOT_2[];
extern uint8_t _PROG_SLOT_3[];
extern int _prog_slot_size[];

void load_prog(uint8_t* _user_start_p, const uint8_t* bin_start, const uint8_t *bin_end) {
    /* Copy user text from rodata to user prog */
    uint8_t* p = _user_start_p;
    for (const uint8_t* i = bin_start; i < bin_end; i++) {
        *p++ = *i;
    }
}

void download_to_slot(const uint8_t* _PROG_SLOT_num, int _prog_slot_size) {
    uint8_t* s = (uint8_t*)_PROG_SLOT_num;
    UART_printf("Download start: %x\n", _PROG_SLOT_num);
    UART_printf("Download end: %x\n", _PROG_SLOT_num+_prog_slot_size);
    while(1) {
        char a, b;

        a = read();
        if (a == '\r') break;
        else write(a);

        b = read();
        if (b == '\r') break;
        else write(b);

        *s++ = hex_to_digit(a) * 16 + hex_to_digit(b);

        a = read();
        if (a == '\r') break;
        else write(a);
        b = read();
        if (b == '\r') break;
        else write(b);             

        *s++ = hex_to_digit(a) * 16 + hex_to_digit(b);

        char c = read(); // discard space
        if (c == '\r') break;
        else write(c);
    }
    write('\n');
}

void main() {   
    /* Set up the MPU */
    // MPU_setup();

    /* Switch to user mode and change the stack pointer */

    while (1) {
        UART_printf("Which program to enter? \n");
        UART_printf("1. name\n");
        UART_printf("2. animal\n");
        UART_printf("3. download\n");
        UART_printf("4. execute\n");
        UART_printf("Your choice: ");
        unsigned int choice;
        UART_read_int(&choice);

        switch (choice) {
            case 1:
                load_prog(_USER_TEXT_START, _binary_user_bin_start, _binary_user_bin_end);
                launch();
                break;
            case 2:
                load_prog(_USER_TEXT_START, _binary_user2_bin_start, _binary_user2_bin_end);
                launch();
                break;
            case 3:
                UART_printf("Which slot to download to?: ");
                unsigned int slot_num;
                UART_read_int(&slot_num);
                switch (slot_num) {
                    case 1: 
                        download_to_slot(_PROG_SLOT_1, (int)_prog_slot_size);
                        break;
                    case 2: 
                        download_to_slot(_PROG_SLOT_2, (int)_prog_slot_size);
                        break;
                    case 3: 
                        download_to_slot(_PROG_SLOT_3, (int)_prog_slot_size);
                        break;
                    default:
                        UART_printf("Not an option."); 
                        break;
                }
                break;
            case 4:
                load_prog(_USER_TEXT_START, _PROG_SLOT_1, _PROG_SLOT_1+(int)_prog_slot_size);
                launch();
                break;
            default:
                UART_printf("Not an option.");
                break;
        }
    }    
}
