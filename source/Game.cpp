#include "Game.h"

/**
 * @brief Construct a Game and initialize its runtime profile and active state.
 *
 * Initializes the Game with no active state and a cleared player profile:
 * - `currentState` is set to `nullptr`.
 * - `profile.agatesCollected` is set to 0.
 * - `profile.canDash` is set to false.
 * - `profile.currentRoomX` and `profile.currentRoomY` are set to 0.
 * - All entries of the 8×8 `profile.exploredRooms` grid are set to `false`, then `profile.exploredRooms[0][0]` is set to `true`.
 */
Game::Game() : currentState(nullptr) {
    profile.agatesCollected = 0;
    profile.canDash = false;
    profile.currentRoomX = 0;
    profile.currentRoomY = 0;
    for (int y = 0; y < PlayerProfile::MAP_HEIGHT; ++y) {
        for (int x = 0; x < PlayerProfile::MAP_WIDTH; ++x) {
            profile.exploredRooms[y][x] = false;
        }
    }
    profile.exploredRooms[0][0] = true;
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
