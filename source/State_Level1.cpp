#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>

/**
 * @brief Construct a Level 1 game state with no associated Game context.
 *
 * Initializes the internal `game` pointer to `nullptr`.
 */
State_Level1::State_Level1() : game(nullptr) {
}

/**
 * @brief Destroy the Level 1 state instance.
 *
 * No explicit cleanup is performed by this destructor; any owned resources
 * are managed externally or have no teardown requirements.
 */
State_Level1::~State_Level1() {
}

/**
 * @brief Initialize Level 1 state with the provided game context.
 *
 * Stores the provided game context pointer, clears the UI, and prints the
 * level title ("Level 1: Memory Prototype") at screen coordinates (16, 64).
 *
 * @param gameContext Pointer to the current Game context to be stored by the state.
 */
void State_Level1::init(Game* gameContext) {
    this->game = gameContext;

    // Clear screen
    UI::clear();

    // Print level text
    UI::print(16, 64, "Level 1: Memory Prototype");
}

/**
 * @brief Executes per-frame update logic for the Level 1 game state.
 *
 * Currently no update behavior is implemented.
 */
void State_Level1::update() {
    // Nothing for now
}

/**
 * @brief No-op render step for the Level 1 state.
 *
 * The level's visual output is produced elsewhere (direct TTE printing), so this
 * draw hook intentionally performs no rendering.
 */
void State_Level1::draw() {
    // Nothing to do for now, handled by TTE printing directly
}

/**
 * @brief Tear down the level and clear on-screen UI.
 *
 * Ensures any UI elements printed for this state are removed from the display.
 */
void State_Level1::teardown() {
    UI::clear();
}
