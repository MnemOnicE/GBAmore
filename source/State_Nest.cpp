#include "State_Nest.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>
#include <stdio.h>

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

void State_Nest::drawMenu() {
    UI::clear();
    char buf[64];
    sprintf(buf, "The Nest - Agates: %d", game->profile.agatesCollected);
    UI::print(16, 16, buf);

    if (viewingCollection) {
        sprintf(buf, "You hold %d of 5 pieces.", game->profile.agatesCollected);
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
