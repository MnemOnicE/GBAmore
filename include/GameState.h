#ifndef GAMESTATE_H
#define GAMESTATE_H

class Game;

class GameState {
public:
    virtual ~GameState() {}
    virtual void init(Game* gameContext) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void teardown() = 0;
};

#endif // GAMESTATE_H
