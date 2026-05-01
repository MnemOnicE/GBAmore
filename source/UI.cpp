#include "UI.h"

void UI::init() {
    // Initialize the GBA display for Mode 0 (tiled)
    // We enable Background 0 (BG0) which is what TTE uses by default
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    // Initialize the Tonc Text Engine (TTE) for a 4bpp character background
    tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));
}

void UI::clear() {
    // Clear the text using TTE erase command (fills the screen with background color)
    tte_erase_screen();
}

void UI::print(int x, int y, const char* text) {
    // Set position and print string
    tte_set_pos(x, y);
    tte_write(text);
}
