#ifndef UI_H
#define UI_H

#include <tonc.h>

class UI {
public:
    static void init();
    static void clear();
    static void print(int x, int y, const char* text);
};

#endif // UI_H
