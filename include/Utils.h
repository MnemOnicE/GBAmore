#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts an integer to a string.
 * @param n The integer to convert.
 * @param s The buffer to store the resulting string.
 */
void itoa(int n, char s[]);

/**
 * Concatenates up to three source strings into a destination buffer.
 * @param dest The destination buffer.
 * @param src1 The first source string.
 * @param src2 The second source string.
 * @param src3 The third source string (can be nullptr).
 */
void strConcat(char* dest, const char* src1, const char* src2, const char* src3);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
