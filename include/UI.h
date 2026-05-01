#ifndef UI_H
#define UI_H

#include <tonc.h>

/**
 * Initialize the UI subsystem and allocate or configure any display resources required before use.
 */
 
/**
 * Clear the UI display and reset any UI-related state to its default.
 */

/**
 * Render a null-terminated C string at the specified screen coordinates.
 * @param x Horizontal coordinate where the text will start.
 * @param y Vertical coordinate where the text will start.
 * @param text Null-terminated C string to render.
 */
class UI {
public:
    static void init();
    static void clear();
    static void print(int x, int y, const char* text);
};

#endif // UI_H
