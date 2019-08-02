#include "user.h"
#include "utility.h"
#include "printf.h"

int main() {
    char * h = "hello";
    printf("%s %s", h, "world!");
    // svcprint('c');
    // svcprint('d');
    // asm volatile ("MOV R0, 'c'");
    // char c = 'c';
    // asm volatile ("MOV R0, %0": : "r" (c));
    // asm volatile ("svc 0");
    // asm volatile ("MOV R0, 'd'");
    // asm volatile ("svc 0");

    // char* a = "hello";
    // asm volatile ("MOV R0, 256");
    // asm volatile ("MOV R1, 15");
    // asm volatile ("svc 0");
}