#ifndef GAME_H
#define GAME_H

#include "GameState.h"

class Game {
public:
    Game();
    ~Game();

    void changeState(GameState* newState);

    void update();
    void draw();

private:
    GameState* currentState;
};

#endif // GAME_H
