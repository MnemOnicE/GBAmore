#include "State_Nest.h"
#include "State_Menu.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>
State_Menu State_Menu::instance;

/**
 * @brief Construct a new State_Menu instance.
 *
 * Initializes the menu state and sets the internal Game pointer to `nullptr`.
 */
State_Menu::State_Menu() : game(nullptr) {
}

/**
 * @brief Destroys the State_Menu instance.
 *
 * Performs no special cleanup; ownership of external resources (for example the stored
 * Game pointer) is not modified.
 */
State_Menu::~State_Menu() {
}

/**
 * @brief Initialize the menu state and display the title screen.
 *
 * Stores the provided game context, clears the UI, and renders the menu title
 * ("GBAMore") and the "Press START" prompt.
 *
 * @param gameContext Pointer to the Game instance this state will use.
 */
void State_Menu::init(Game* gameContext) {
    this->game = gameContext;

    // Disable objects completely, as this is a pure text menu
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

    // Clear screen
    UI::clear();

    // Draw Game Title and "Press START"
    UI::print(64, 48, "GBAMore");
    UI::print(64, 80, "Press START");
}

/**
 * @brief Polls input and transitions the game's state to the Nest when START is pressed.
 *
 * When the START button is detected, replaces the current game state with the State_Nest instance.
 * When the START button is detected, replaces the current game state with the State_Nest::instance.
 */
void State_Menu::update() {
    if (key_hit(KEY_START)) {
        game->changeState(&State_Nest::instance);
    }
}

/**
 * @brief No-op draw step for the menu state.
 *
 * The menu UI is rendered immediately when printed, so no per-frame drawing is performed here.
 */
void State_Menu::draw() {
    // TTE draws directly when print is called, so draw is empty here
}

/**
 * @brief Clears on-screen UI elements when the menu state is torn down.
 *
 * Ensures the display is reset by removing the menu text and any UI artifacts.
 */
void State_Menu::teardown() {
    UI::clear();
}
