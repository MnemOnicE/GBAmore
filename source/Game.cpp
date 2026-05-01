#include "Game.h"

/**
 * @brief Constructs a Game and initializes its state pointer.
 *
 * Initializes the Game instance with no active state (sets `currentState` to `nullptr`).
 */
Game::Game() : currentState(nullptr) {
    profile.agatesCollected = 0;
    profile.canDash = false;
}

/**
 * @brief Cleans up the current game state and frees associated resources.
 *
 * If a current state exists, calls its teardown routine.
 */
Game::~Game() {
    if (currentState) {
        currentState->teardown();

    }
}

/**
 * @brief Replace the game's current state with a new state and initialize it.
 *
 * If a current state exists, it is torn down before assignment.
 * After assigning `newState`, the state's `init(this)` is called when `newState` is non-null.
 *
 * @param newState Pointer to the new GameState to become the active state. Game does not assume ownership of state memory; caller is responsible for lifetime. Passing `nullptr` clears the current state.
 */
void Game::changeState(GameState* newState) {
    if (currentState) {
        currentState->teardown();
         // Game does not delete or free states; callers must manage state lifetimes (e.g., using static instances).
    }

    currentState = newState;

    if (currentState) {
        currentState->init(this);
    }
}

/**
 * @brief Update the active game state.
 *
 * Calls `update()` on the currently assigned `GameState` if one exists.
 */
void Game::update() {
    if (currentState) {
        currentState->update();
    }
}

/**
 * @brief Renders the active game state.
 *
 * If a current state is set, calls its draw() method to perform rendering.
 */
void Game::draw() {
    if (currentState) {
        currentState->draw();
    }
}
