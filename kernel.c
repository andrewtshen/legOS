#include "UART.h"
#include "priv.h"
#include "test.h"
#include "MPU.h"
#include <stdint.h>

extern const uint8_t _binary_user_bin_start[];
extern const uint8_t _binary_user_bin_end[];

void main() {
	// writeln_str("Testing MPU");
 //    test_MPU();

 //    writeln_str("---");
 //    writeln_str("Testing Privileges");
 //    test_privileges();
 //    writeln_str("---");
 
 	switch_to_user();

    /*writeln_str("testing mpu...");*/
    /*test_MPU();*/

    writeln_str("END OF MAIN REACHED.");

    
    for (const uint8_t* i = _binary_user_bin_start; i < _binary_user_bin_end; i++) {
    	writeln_int(*i);
	}
}
