#ifndef STATE_LEVEL1_H
#define STATE_LEVEL1_H

#include "GameState.h"

/**
 * Represents the first gameplay state (Level 1) and manages its lifecycle.
 */
 
/**
 * Construct a new State_Level1 instance.
 */

/**
 * Destroy the State_Level1 instance and release owned resources.
 */

/**
 * Initialize the level using the provided game context.
 * @param gameContext Pointer to the Game instance used by this state.
 */

/**
 * Advance the level's simulation by one tick/frame.
 */

/**
 * Render the level to the current rendering context.
 */

/**
 * Perform cleanup of the Level 1 state and prepare for shutdown or transition.
 *
 * Resets internal runtime state and releases or invalidates resources owned by this state.
 */
class State_Level1 : public GameState {
public:
    State_Level1();
    ~State_Level1();

    void init(Game* gameContext) override;
    void update() override;
    void draw() override;
    void teardown() override;

private:
    Game* game;
    OBJ_ATTR obj_buffer[128];
    int player_x;
    int player_y;
};

#endif // STATE_LEVEL1_H
