#include "string.h"

/* Return string length */
int strlen(const char *s) {
    int n;

    for(n = 0; s[n] != 0; n++)
        ;
    return n;
}

/* Concatenate src to dest of total length size */
char* strcat(char* dest, const char* src, int size) {
    /* Check if overflows dest */
    if (strlen(dest) + strlen(src) > size-1) return 0;

    char* ptr = dest + strlen(dest);

    while (*src != '\0')
        *ptr++ = *src++;

    *ptr = '\0';

    return dest;
}

/* Concatenate char src to destination of total length size */
char* strcatc(char* dest, char src, int size) {
    /* Check if overflows dest*/
    if (strlen(dest) + 1 >= size-1) return 0;

    /* Add char onto end */
    char* p = dest + strlen(dest);
    *p++ = src;
    *p = '\0';

    return dest;
}

/* Copy string from src to dest of total length size */
char* strncpy(char* dest, const char* src, int num, int size) {
    /* Return if no memory is allocated to the dest or dest overflows */
    if (dest == 0) return 0;
    if (strlen(src) >= size-1) return 0;

    /* Copy over string */
    char* ptr = dest;
    while (*src && num--) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';

    return ptr;
}