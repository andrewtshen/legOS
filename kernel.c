#include "UART.h"
#include "priv.h"
#include "test.h"
#include "MPU.h"
#include <stdint.h>


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
}
