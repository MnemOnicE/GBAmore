#include <iostream>
#include <cstring>
#include <cassert>
#include <climits>
#include "Utils.h"

void test_itoa() {
    char buf[32];

    std::cout << "Testing itoa..." << std::endl;

    itoa(0, buf);
    assert(std::strcmp(buf, "0") == 0);
    std::cout << "  itoa(0) passed" << std::endl;

    itoa(123, buf);
    assert(std::strcmp(buf, "123") == 0);
    std::cout << "  itoa(123) passed" << std::endl;

    itoa(-123, buf);
    assert(std::strcmp(buf, "-123") == 0);
    std::cout << "  itoa(-123) passed" << std::endl;

    itoa(2147483647, buf);
    assert(std::strcmp(buf, "2147483647") == 0);
    std::cout << "  itoa(INT_MAX) passed" << std::endl;

    std::cout << "  Testing itoa(INT_MIN)..." << std::endl;
    itoa(-2147483648, buf);
    assert(std::strcmp(buf, "-2147483648") == 0);
    std::cout << "  itoa(INT_MIN) passed" << std::endl;
}

void test_strConcat() {
    char buf[64];

    std::cout << "Testing strConcat..." << std::endl;

    strConcat(buf, sizeof(buf), "Hello", " World", nullptr);
    assert(std::strcmp(buf, "Hello World") == 0);
    std::cout << "  strConcat(2 strings) passed" << std::endl;

    strConcat(buf, sizeof(buf), "Hello", " ", "World");
    assert(std::strcmp(buf, "Hello World") == 0);
    std::cout << "  strConcat(3 strings) passed" << std::endl;

    char smallBuf[5];
    strConcat(smallBuf, sizeof(smallBuf), "Hello", " World", nullptr);
    assert(std::strcmp(smallBuf, "Hell") == 0);
    std::cout << "  strConcat bounds checking passed" << std::endl;

    strConcat(buf, sizeof(buf), "", "", "");
    assert(std::strcmp(buf, "") == 0);
    std::cout << "  strConcat(empty strings) passed" << std::endl;
}

void test_null_checks() {
    std::cout << "Testing null pointer checks..." << std::endl;

    // These should not crash
    itoa(0, nullptr);
    std::cout << "  itoa(null) passed" << std::endl;

    strConcat(nullptr, 10, "a", "b", "c");
    std::cout << "  strConcat(dest=null) passed" << std::endl;

    char buf[10];
    strConcat(buf, sizeof(buf), nullptr, "b", "c");
    assert(buf[0] == '\0');
    std::cout << "  strConcat(src1=null) passed" << std::endl;

    strConcat(buf, sizeof(buf), "a", nullptr, "c");
    assert(buf[0] == '\0');
    std::cout << "  strConcat(src2=null) passed" << std::endl;
}

int main() {
    test_itoa();
    test_strConcat();
    test_null_checks();
    std::cout << "All tests completed!" << std::endl;
    return 0;
}
