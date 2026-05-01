#ifndef GAME_H
#define GAME_H

#include "GameState.h"

/**
 * Game class that manages the active GameState and provides lifecycle operations.
 */

/**
 * Construct a Game instance.
 *
 * Initializes the game's internal state.
 */

/**
 * Destroy the Game instance.
 *
 * Performs any necessary cleanup for the game's internal state.
 */

/**
 * Replace the active game state with a new state.
 *
 * @param newState Pointer to the GameState that should become the active state.
 */

/**
 * Advance the game logic by one update step for the active state.
 */

/**
 * Render the active game state.
 */

/**
 * Pointer to the currently active GameState.
 */
struct PlayerProfile {
    int agatesCollected;
    bool canDash;
};

/**
 * Core game object that owns runtime state and drives the main loop.
 */
 
/**
 * Construct a Game and initialize its internal state.
 */
 
/**
 * Destroy the Game and release any owned resources.
 */
 
/**
 * Set the active game state.
 * @param newState Pointer to the GameState to become active; may be `nullptr` to clear the active state.
 */
 
/**
 * Advance the active game state's logic by one update step.
 */
 
/**
 * Render the active game state.
 */
 
/**
 * Runtime player state stored on the Game instance (tracks collected agates and dash capability).
 */
class Game {
public:
    Game();
    ~Game();

    void changeState(GameState* newState);

    void update();
    void draw();

    PlayerProfile profile;
private:
    GameState* currentState;
};

#endif // GAME_H
