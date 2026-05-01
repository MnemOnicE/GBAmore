#include "State_Menu.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

State_Menu::State_Menu() : game(nullptr) {
}

State_Menu::~State_Menu() {
}

void State_Menu::init(Game* gameContext) {
    this->game = gameContext;

    // Clear screen
    UI::clear();

    // Draw Game Title and "Press START"
    UI::print(64, 48, "GBAMore");
    UI::print(64, 80, "Press START");
}

void State_Menu::update() {
    if (key_hit(KEY_START)) {
        game->changeState(new State_Level1());
    }
}

void State_Menu::draw() {
    // TTE draws directly when print is called, so draw is empty here
}

void State_Menu::teardown() {
    UI::clear();
}
