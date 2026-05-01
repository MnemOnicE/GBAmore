#include "UI.h"

/**
 * @brief Configures the GBA display for tiled text rendering and initializes the Tonc Text Engine.
 *
 * Sets the display control to Mode 0 with BG0 enabled and initializes TTE instance 0
 * for a 4bpp character background using character block 0 and screen block 31.
 */
void UI::init() {
    // Initialize the GBA display for Mode 0 (tiled)
    // We enable Background 0 (BG0) which is what TTE uses by default
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    // Initialize the Tonc Text Engine (TTE) for a 4bpp character background
    tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));
}

/**
 * @brief Clears the text layer and restores the background.
 *
 * Erases all characters on the current text screen and fills the layer with the configured background color.
 */
void UI::clear() {
    // Clear the text using TTE erase command (fills the screen with background color)
    tte_erase_screen();
}

/**
 * @brief Prints a null-terminated string to the text layer at the specified tile coordinates.
 *
 * Positions the text cursor at the given (x, y) tile coordinates and writes the provided C-string
 * to the screen's text layer.
 *
 * @param x Horizontal tile column where text starts.
 * @param y Vertical tile row where text starts.
 * @param text Null-terminated C-string to display.
 */
void UI::print(int x, int y, const char* text) {
    // Set position and print string
    tte_set_pos(x, y);
    tte_write(text);
}
