#include "Game.h"

/**
 * @brief Construct a Game and initialize its runtime state.
 *
 * Initializes the Game with no active state and a default player profile:
 * `currentState` is set to `nullptr`, `profile.agatesCollected` is set to 0,
 * and `profile.canDash` is set to false.
 */
Game::Game() : currentState(nullptr) {
    profile.agatesCollected = 0;
    profile.canDash = false;
}

/**
 * @brief Destroys the Game and runs teardown on the active game state.
 *
 * If an active `currentState` exists, its `teardown()` method is invoked to allow the state to release resources.
 * The destructor does not delete or take ownership of `currentState`.
 */
Game::~Game() {
    if (currentState) {
        currentState->teardown();

    }
}

/**
 * @brief Replace the game's current state and initialize the new state.
 *
 * If a current state exists, its teardown() is called before replacing it; this function does not delete the previous state. After assignment, if `newState` is non-null its init(this) is invoked. Passing `nullptr` clears the current state.
 *
 * @param newState Pointer to the new GameState to become active. Ownership is not taken by Game; callers are responsible for managing the state's lifetime.
 */
void Game::changeState(GameState* newState) {
    if (currentState) {
        currentState->teardown();
         // Assuming the game owns the state memory and states are dynamically allocated. For now, we assume simple pointer transfer
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
