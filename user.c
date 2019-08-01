#include "user.h"
#include "utility.h"

void foo() {
    asm volatile("nop");
}

int main() {
    // char* a = "hello";
    asm volatile ("MOV R0, 256");
    asm volatile ("MOV R1, 15");
    asm volatile ("svc 0");
    foo();
    // int a = 5;
    // int b = 6;
    // return sum(a, b);
}