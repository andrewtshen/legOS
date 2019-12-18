#include "printf.h"

void main() {
    printf("Hello World from User!\n");
    asm volatile ("ecall");
}