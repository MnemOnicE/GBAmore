#include <tonc.h>
#include "Game.h"
#include "State_Menu.h"
#include "UI.h"

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
