#ifndef STATE_LEVEL1_H
#define STATE_LEVEL1_H

#include "GameState.h"

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
};

#endif // STATE_LEVEL1_H
