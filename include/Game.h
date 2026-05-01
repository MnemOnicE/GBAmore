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
    static constexpr int MAP_WIDTH = 8;
    static constexpr int MAP_HEIGHT = 8;
    bool exploredRooms[MAP_HEIGHT][MAP_WIDTH];
    int currentRoomX;
    int currentRoomY;
};

/**
 * Main game engine class that holds player state and coordinates game states.
 */
class Game {
public:
    /**
     * Construct a Game instance and initialize its internal state.
     */
    Game();

    /**
     * Destroy the Game instance and release its active state resources.
     */
    ~Game();

    /**
     * Replace the currently active game state with `newState`.
     * @param newState Pointer to the GameState that will become the active state.
     */
    void changeState(GameState* newState);

    /**
     * Advance game logic by one step for the active state.
     */
    void update();

    /**
     * Render the active state to the current output/screen.
     */
    void draw();

    PlayerProfile profile;
private:
    GameState* currentState;
};

#endif // GAME_H
