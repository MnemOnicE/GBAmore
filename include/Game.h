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
    bool exploredRooms[8][8];
    int currentRoomX;
    int currentRoomY;
};

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
