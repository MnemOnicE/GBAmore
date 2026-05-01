#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "GameState.h"

class State_Menu : public GameState {
public:
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
