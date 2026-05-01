#include "State_Map.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

State_Map State_Map::instance;

State_Map::State_Map() : game(nullptr) {}

State_Map::~State_Map() {}

void State_Map::init(Game* gameContext) {
    this->game = gameContext;
    UI::clear();
}

void State_Map::update() {
    if (key_hit(KEY_SELECT) || key_hit(KEY_B)) {
        game->changeState(&State_Level1::instance);
    }
}

void State_Map::draw() {
    // Redraw the map every frame
    for(int y = 0; y < 8; ++y) {
        for(int x = 0; x < 8; ++x) {
            int print_x = 56 + (x * 16);
            int print_y = 32 + (y * 12);

            if (x == game->profile.currentRoomX && y == game->profile.currentRoomY) {
                UI::print(print_x, print_y, "[X]");
            } else if (game->profile.exploredRooms[y][x]) {
                UI::print(print_x, print_y, "[ ]");
            }
            // Unexplored rooms are left blank
        }
    }
}

void State_Map::teardown() {
    UI::clear();
}
