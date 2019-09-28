#include "printf.h"
#include "string.h"
#include "UART.h"
#include "math.h"
#include "test.h"
#include "scanf.h"

void main() {

    printf("What is your favorite animal?: ");
    char name[20];
    read_str(name, 20);

    printf("Your favorite animal is %s\n", name);

    // USER_tests();
}
