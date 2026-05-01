#ifndef STATE_LEVEL1_H
#define STATE_LEVEL1_H

#include "GameState.h"
#include "shadow.h"
#include <tonc.h>

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
 * Perform cleanup and prepare the state for shutdown or transition.
 */
struct Entity {
    int x, y;
    int dx, dy;
    int width, height;
    int type; // 0 = Player, 1 = Shadow
    bool active;
};

class State_Level1 : public GameState {
public:
    static State_Level1 instance;
    State_Level1();
    ~State_Level1();

    void init(Game* gameContext) override;
    void update() override;
    void draw() override;
    void teardown() override;

private:
    Game* game;
    OBJ_ATTR obj_buffer[128];
    Entity entities[16];
    int num_entities;
    bool inCutscene;

    bool checkCollision(Entity& a, Entity& b);

};

#endif // STATE_LEVEL1_H
