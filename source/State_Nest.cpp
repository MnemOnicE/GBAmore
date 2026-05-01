#include "State_Nest.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

State_Nest State_Nest::instance;

State_Nest::State_Nest() : game(nullptr), selectedOption(0), viewingCollection(false) {
}

State_Nest::~State_Nest() {
}

void State_Nest::init(Game* gameContext) {
    this->game = gameContext;
    selectedOption = 0;
    viewingCollection = false;

    // Disable objects completely, as this is a pure text menu
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

    UI::clear();
    drawMenu();
}

static void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    // Reverse string
    int j, c;
    for (j = 0, i = i - 1; j < i; j++, i--) {
        c = s[j];
        s[j] = s[i];
        s[i] = c;
    }
}

static void strConcat(char* dest, const char* src1, const char* src2, const char* src3) {
    int i = 0;
    while (*src1) dest[i++] = *src1++;
    while (*src2) dest[i++] = *src2++;
    if (src3) {
        while (*src3) dest[i++] = *src3++;
    }
    dest[i] = '\0';
}

void State_Nest::drawMenu() {
    UI::clear();
    char buf[64];
    char numBuf[16];

    itoa(game->profile.agatesCollected, numBuf);
    strConcat(buf, "The Nest - Agates: ", numBuf, nullptr);
    UI::print(16, 16, buf);

    if (viewingCollection) {
        strConcat(buf, "You hold ", numBuf, " of 5 pieces.");
        UI::print(16, 48, buf);
        UI::print(16, 64, "The rest are still out there.");
        UI::print(16, 96, "Press B to return");
    } else {
        if (selectedOption == 0) {
            UI::print(16, 48, "> 1. View Collection");
            UI::print(16, 64, "  2. Leave Nest (Enter Level 1)");
        } else {
            UI::print(16, 48, "  1. View Collection");
            UI::print(16, 64, "> 2. Leave Nest (Enter Level 1)");
        }
    }
}

void State_Nest::update() {
    if (viewingCollection) {
        if (key_hit(KEY_B)) {
            viewingCollection = false;
            drawMenu();
        }
    } else {
        if (key_hit(KEY_UP) || key_hit(KEY_DOWN)) {
            selectedOption = 1 - selectedOption;
            drawMenu();
        }

        if (key_hit(KEY_A)) {
            if (selectedOption == 0) {
                viewingCollection = true;
                drawMenu();
            } else {
                game->changeState(&State_Level1::instance);
            }
        }
    }
}

void State_Nest::draw() {
}

void State_Nest::teardown() {
    UI::clear();
}
