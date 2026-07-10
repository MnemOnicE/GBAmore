#ifndef STATE_MAP_H
#define STATE_MAP_H

#include "GameState.h"

class Game;

class State_Map : public GameState {
public:
    static State_Map instance;

    void init(Game* gameContext) override;
    void update() override;
    void draw() override;
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
