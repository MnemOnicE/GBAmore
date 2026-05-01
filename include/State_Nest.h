#ifndef STATE_NEST_H
#define STATE_NEST_H

#include "GameState.h"

/**
 * Game state representing the "Nest" screen and its UI flow.
 *
 * Manages entering and exiting the state, per-frame updates, rendering, and a nested menu/collection view.
 */

/**
 * Construct a State_Nest instance.
 *
 * Initializes internal state to default values.
 */

/**
 * Destroy the State_Nest instance.
 *
 * Releases any resources owned by the state.
 */

/**
 * Initialize the state with the active game context.
 *
 * @param gameContext Pointer to the current Game instance used for accessing shared systems and resources.
 */

/**
 * Update state logic for a single frame.
 *
 * Handles input, selection changes, and transitions while the state is active.
 */

/**
 * Render the state's visuals for the current frame.
 *
 * Draws the menu or collection view depending on the current mode.
 */

/**
 * Teardown the state before transitioning away.
 *
 * Performs cleanup and resets state so the instance can be reinitialized later.
 */

/**
 * Draw the nest menu UI.
 *
 * Renders menu entries and highlights the currently selected option.
 */
class State_Nest : public GameState {
public:
    static State_Nest instance;

    State_Nest();
    ~State_Nest();

    void init(Game* gameContext) override;
    void update() override;
    void draw() override;
    void teardown() override;

private:
    Game* game;
    int selectedOption;
    bool viewingCollection;

    void drawMenu();
};

#endif // STATE_NEST_H
