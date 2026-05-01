#ifndef STATE_MAP_H
#define STATE_MAP_H

#include "GameState.h"

class Game;

/**
 * Represents the game's map state and manages its lifecycle.
 *
 * Use the shared State_Map::instance to access this state. The state holds a
 * pointer to the Game context provided during initialization.
 */
class State_Map : public GameState {
public:
    static State_Map instance;

    /**
     * Initialize the map state with the provided game context.
     * @param gameContext Pointer to the Game instance used by this state.
     */
    void init(Game* gameContext) override;

    /**
     * Advance the map state's logic for one frame or tick.
     */
    void update() override;

    /**
     * Render the map state's visuals.
     */
    void draw() override;

    /**
     * Release resources associated with the map state and clear its game context.
     */
    void teardown() override;

private:
    State_Map();
    ~State_Map();

    // Prevent copy and assignment
    State_Map(const State_Map&) = delete;
    State_Map& operator=(const State_Map&) = delete;

    Game* game;
};

#endif // STATE_MAP_H
