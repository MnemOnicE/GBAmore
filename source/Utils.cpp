#include "Utils.h"
#include <stddef.h>

void itoa(int n, char s[]) {
    int i, sign;
    unsigned int num;
    if ((sign = n) < 0) {
        num = -(unsigned int)n;
    } else {
        num = (unsigned int)n;
    }
    i = 0;
    do {
        s[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    int j, c;
    for (j = 0, i = i - 1; j < i; j++, i--) {
        c = s[j];
        s[j] = s[i];
        s[i] = c;
    }
}

void strConcat(char* dest, const char* src1, const char* src2, const char* src3) {
    int i = 0;
    if (src1) while (*src1) dest[i++] = *src1++;
    if (src2) while (*src2) dest[i++] = *src2++;
    if (src3) {
        while (*src3) dest[i++] = *src3++;
    }
    dest[i] = '\0';
}
