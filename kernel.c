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
extern uint8_t _DOWNLOAD_START[];
extern uint8_t _DOWNLOAD_END[];

void load_userprog(uint8_t* _user_start_p, const uint8_t* bin_start, const uint8_t *bin_end) {
    /* Copy user text from rodata to user prog */

    for (const uint8_t* i = bin_start; i < bin_end; i++) {
        *(_user_start_p) = *i;
        _user_start_p++;
    }
}

void main() {   
    /* Set up the MPU */
    // MPU_setup();

    /* Switch to user mode and change the stack pointer */

    while (1) {
        UART_printf("Which program to enter? \n");
        UART_printf("1. name \n");
        UART_printf("2. animal \n");
        UART_printf("3. download program \n");
        UART_printf("4. execute downloaded program \n");
        UART_printf("Your choice: ");
        unsigned int choice;
        UART_read_int(&choice);

        if (choice == 1) {
            load_userprog(_USER_TEXT_START, _binary_user_bin_start, _binary_user_bin_end);
            launch();    
        } else if (choice == 2) {
            load_userprog(_USER_TEXT_START, _binary_user2_bin_start, _binary_user2_bin_end);
            launch();
        } else if (choice == 3) {
            UART_printf("Download program: ");
            uint8_t* s = (uint8_t*)_DOWNLOAD_START;
            UART_printf("Download start: %x\n", _DOWNLOAD_START);
            UART_printf("Download end: %x\n", _DOWNLOAD_END);
            while(1) {
                char a;
                char b;

                a = read();
                if (a == '\r')
                    break;
                else
                    write(a);
                b = read();
                if (b == '\r')
                    break;
                else
                    write(b);

                *s = hex_to_digit(a) * 16 + hex_to_digit(b);
                s++;

                a = read();
                if (a == '\r')
                    break;
                else 
                    write(a);
                b = read();
                if (b == '\r')
                    break;
                else
                    write(b);             

                *s = hex_to_digit(a) * 16 + hex_to_digit(b);
                s++;

                char c = read(); // discard space
                if (c == '\r')
                    break;
                else
                    write(c);
            }
            write('\n');
        } else if (choice == 4) {
            // uint8_t* s = (uint8_t)_DOWNLOAD_START;
            // for (int i = 0; i < 3000; ++i)
            // {
            //     UART_printf("%x\n", *s);
            //     s++;
            // }
            launch_download();
        }
    }    
}
