#include <tonc.h>
#include "Game.h"
#include "State_Menu.h"
#include "UI.h"

/**
 * @brief Program entry point that initializes the UI and runs the game's main loop.
 *
 * Initializes standard Tonc settings and text UI via UI::init(), constructs the game
 * context and transitions to the menu state, then enters the main loop which
 * synchronizes to vertical blank, polls input, updates game logic, and renders each frame.
 *
 * @return int Exit code (returns 0, but the main loop is infinite in normal execution).
 */
int main()
{
    // Initialize standard Tonc settings and TTE
    UI::init();

    // Initialize Game context and push State_Menu
    Game game;
    game.changeState(new State_Menu());

    // Basic game loop
    while(1)
    {
        vid_vsync();
        key_poll();

        game.update();
        game.draw();
    }

    return 0;
}
