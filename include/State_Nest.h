#ifndef STATE_NEST_H
#define STATE_NEST_H

#include "GameState.h"
#include <tonc.h>

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
    OBJ_ATTR obj_buffer[128];
    int player_x;
    int player_y;
    bool interacting;
};

#endif // STATE_NEST_H
