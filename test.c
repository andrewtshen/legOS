#include "test.h"
#include "UART.h"
#include "MPU.h"
#include <stdint.h>
#include <assert.h>

#define ASSERT(expr) \
    if (expr) \
        printf("%s: PASSED\n", __func__); \
    else \
        printf("%s: FAILED\n", __func__);

void KERNEL_test_assert() {
    // ASSERT(0 == 1);
    // ASSERT(0 == 0);

}