#include "State_Map.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

State_Map State_Map::instance;

/**
 * @brief Constructs a State_Map and initializes its game context pointer.
 *
 * Initializes the internal `game` pointer to `nullptr`.
 */
State_Map::State_Map() : game(nullptr) {}

/**
 * @brief Destroys the State_Map instance.
 *
 * The destructor is intentionally empty and performs no cleanup.
 */
State_Map::~State_Map() {}

/**
 * @brief Initialize the map state with the provided game context and reset the screen.
 *
 * Stores the given Game pointer for later use by the state and clears the UI to prepare the map view.
 *
 * @param gameContext Pointer to the active Game instance that the state will reference.
 */
void State_Map::init(Game* gameContext) {
    this->game = gameContext;
    UI::clear();
}

/**
 * @brief Handle input on the map screen and transition to the first level when requested.
 *
 * Checks for a press of KEY_SELECT or KEY_B and, when detected, requests the game
 * to change the active state to State_Level1::instance.
 */
void State_Map::update() {
    if (key_hit(KEY_SELECT) || key_hit(KEY_B)) {
        game->changeState(&State_Level1::instance);
    }
}

/**
 * @brief Draws the current world map to the screen.
 *
 * Renders an 8×8 room grid: prints "[X]" at the player's current room coordinates,
 * prints "[ ]" for rooms marked explored in the game profile, and leaves unexplored rooms blank.
 */
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

/**
 * @brief Clear the map state's UI from the screen.
 *
 * Remove any map-specific UI elements so the display is blank when leaving the map state.
 */
void State_Map::teardown() {
    UI::clear();
}
