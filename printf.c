#include "printf.h"

void cprintf(char *fmt, ...) {
    char *s;
    int c, i, state;
    int *ap;

    state = 0;
    ap = (int*)(void*)&fmt + 1;
    for(i = 0; fmt[i]; i++){
        c = fmt[i] & 0xff;
        if(state == 0){
            if(c == '%'){
                state = '%';
            } else {
                svcprint(c);
                // asm volatile ("MOV R0, %0": : "r" (c));
                // asm volatile ("svc 0");
                // foo();
            }
        } else if(state == '%'){
            if(c == 'd'){
                // printint(fd, *ap, 10, 1);
                // write_int(*ap);
                ap++;
            } else if(c == 'x' || c == 'p'){
                // printint(fd, *ap, 16, 0);
                // write_hex(*ap);
                ap++;
            } else if(c == 's'){
                s = (char*)*ap;
                ap++;
                if(s == 0)
                    s = "(null)";
                while(*s != 0){
                    // svcprint(*s);
                    s++;
                }
            } else if(c == 'c'){
                // svcprint(*ap);
                ap++;
            } else if(c == '%'){
                // svcprint(c);
            } else {
                // Unknown % sequence.  Print it to draw attention.
                // svcprint('%');
                // svcprint(c);
            }
            state = 0;
        }
    }
}

void svcprint(char c) {
    asm volatile ("MOV R0, %0": : "r" (c));
    asm volatile ("PUSH {LR}");
    asm volatile ("svc 0");
    asm volatile ("POP {LR}");
}

char int_to_hex(unsigned int s) {
    /* Conversion to Hex with switch case */
    switch (s) {
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return s+'0';
    }
}

// void write_str(const char* s) {
//     while (*s != '\0') {
//         write(*s);
//         s++;
//     }
// }

void write_int(unsigned int s) {
    if (s < 10) {
        svcprint(s + '0');
    } else {
        write_int(s / 10);
        write_int(s % 10);
    }
}

void write_hex(unsigned int s) {
    if (s < 16) {
        svcprint(int_to_hex(s));
    } else {
        write_hex(s / 16);
        write_hex(s % 16);
    }
}

// void writeln_str(const char* s) {
//     while (*s != '\0') {
//         write(*s);
//         s++;
//     }
//     write('\n');
// }

// void writeln_int(unsigned int s) {
//     if (s < 10) {
//         write(s + '0');
//     } else {
//         write_int(s / 10);
//         write_int(s % 10);
//     }
//     write('\n');
// }

// void writeln_hex(unsigned int s) {
//     if (s < 16) {
//         write(int_to_hex(s));
//     } else {
//         write_hex(s / 16);
//         write_hex(s % 16);
//     }
//     write('\n');
// }

// void read_str(char* buf, int count) {  
//     for (int i = 0; i < count; i++) {    
//         buf[i] = read();
//         write(buf[i]);      // Echo the output
        
//         /* Continue until we see carriage return */
//         if (buf[i] == '\r') {
//             buf[i] = '\0';  // Terminate string with Null character.
//             write('\n');    // Nice printing.
//             return;
//         }
//     }
//     buf[count-1] = '\0';    // Terminate String with Null character.
//     write('\n');            // Nice printing
// }

// void read_int(unsigned int* a) {
//     *a = 0; 
//     unsigned int tmp;
//     while (1) {
//         tmp = read();
//         write(tmp);         // Echo the output

//         /* Continue until we see carriage return */
//         if (tmp != '\r') {
//             tmp -= '0';
//             *a = (10 * (*a)) + tmp;
//         } else {
//             break;
//         }
//     }
//     write('\n'); // Nice printing
// }