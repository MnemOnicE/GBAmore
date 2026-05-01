#ifndef STATE_NEST_H
#define STATE_NEST_H

#include "GameState.h"

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
