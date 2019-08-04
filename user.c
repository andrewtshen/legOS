#include "user.h"
#include "utility.h"
#include "printf.h"
#include "string.h"
#include "UART.h"
#include "math.h"
#include "test.h"

int main() {
    int a = 15;
    printf("this is a: %d\n", a);
    printf("this is a: %x\n", a);
    printf("this is %d\n", 8);

    KERNEL_test_assert();

}