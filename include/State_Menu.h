#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "GameState.h"

/**
 * Represents the main menu game state responsible for menu lifecycle (initialization,
 * per-frame updates, rendering, and cleanup).
 */
 
/**
 * Construct a new menu state instance.
 */
 
/**
 * Destroy the menu state and free associated resources.
 */
 
/**
 * Initialize the menu state with the provided game context.
 * @param gameContext Pointer to the Game instance the state will use for context and services.
 */
 
/**
 * Advance the menu logic for a single frame (handle input, update selections, etc.).
 */
 
/**
 * Render the menu visuals to the current render target.
 */
 
/**
 * Main menu game state that implements the menu lifecycle: initialization,
 * per-frame update, rendering, and cleanup.
 *
 * Represents the game's main menu and holds a pointer to the Game context
 * used by its lifecycle methods. A single shared instance is exposed via
 * the public static member `State_Menu::instance`.
 */
class State_Menu : public GameState {
public:
    static State_Menu instance;
    State_Menu();
    ~State_Menu();

    void init(Game* gameContext) override;
    void update() override;
    void draw() override;
    void teardown() override;

private:
    Game* game;
};

#endif // STATE_MENU_H
