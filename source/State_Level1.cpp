#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

State_Level1::State_Level1() : game(nullptr) {
}

State_Level1::~State_Level1() {
}

void State_Level1::init(Game* gameContext) {
    this->game = gameContext;

    // Clear screen
    UI::clear();

    // Print level text
    UI::print(16, 64, "Level 1: Memory Prototype");
}

void State_Level1::update() {
    // Nothing for now
}

void State_Level1::draw() {
    // Nothing to do for now, handled by TTE printing directly
}

void State_Level1::teardown() {
    UI::clear();
}
