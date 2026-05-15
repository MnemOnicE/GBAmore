#include "Utils.h"
#include <stddef.h>

void itoa(int n, char s[], size_t size) {
    if (size == 0) return;
    char temp[32];
    int i, sign;
    unsigned int num;
    if ((sign = n) < 0) {
        num = -(unsigned int)n;
    } else {
        num = (unsigned int)n;
    }
    i = 0;
    do {
        temp[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);
    if (sign < 0)
        temp[i++] = '-';

    size_t destIdx = 0;
    for (int j = i - 1; j >= 0 && destIdx < size - 1; j--) {
        s[destIdx++] = temp[j];
    }
    s[destIdx] = '\0';
}

void strConcat(char* dest, size_t destCap, const char* src1, const char* src2, const char* src3) {
    if (destCap == 0 || !dest || !src1 || !src2) return;
    size_t i = 0;
    while (*src1 && i < destCap - 1) dest[i++] = *src1++;
    while (*src2 && i < destCap - 1) dest[i++] = *src2++;
    if (src3) {
        while (*src3 && i < destCap - 1) dest[i++] = *src3++;
    }
    dest[i] = '\0';
}
