#include "Game.h"

Game::Game() : currentState(nullptr) {
}

Game::~Game() {
    if (currentState) {
        currentState->teardown();
        delete currentState;
    }
}

void Game::changeState(GameState* newState) {
    if (currentState) {
        currentState->teardown();
        delete currentState; // Assuming the game owns the state memory and states are dynamically allocated. For now, we assume simple pointer transfer
    }

    currentState = newState;

    if (currentState) {
        currentState->init(this);
    }
}

void Game::update() {
    if (currentState) {
        currentState->update();
    }
}

void Game::draw() {
    if (currentState) {
        currentState->draw();
    }
}
