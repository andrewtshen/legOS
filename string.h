#ifndef STRING_H
#define STRING_H

int strlen(const char*s);
char* strcat(char* dest, const char* src, int size);
char* strcatc(char* s, char t, int size);
char* strncpy(char* dest, const char* src, int num, int size);
int strcmp(const char* s1, const char* s2);

#endif