#include "Game.h"
#include <tonc.h>
#include <string.h>

/**
 * @brief Constructs a Game and initializes its state pointer.
 *
 * Initializes the Game instance with no active state (sets `currentState` to `nullptr`).
 */
Game::Game() : currentState(nullptr) {
    profile.agatesCollected = 0;
    profile.canDash = false;
    profile.currentRoomX = 0;
    profile.currentRoomY = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            profile.exploredRooms[y][x] = false;
        }
    }
    profile.exploredRooms[0][0] = true;
    this->load();
}

/**
 * @brief Cleans up the current game state and frees associated resources.
 *
 * If a current state exists, calls its teardown routine.
 */
Game::~Game() {
    if (currentState) {
        currentState->teardown();

    }
}

/**
 * @brief Replace the game's current state with a new state and initialize it.
 *
 * If a current state exists, it is torn down before assignment.
 * After assigning `newState`, the state's `init(this)` is called when `newState` is non-null.
 *
 * @param newState Pointer to the new GameState to become the active state. Game does not assume ownership of state memory; caller is responsible for lifetime. Passing `nullptr` clears the current state.
 */
void Game::changeState(GameState* newState) {
    if (currentState) {
        currentState->teardown();
         // Game does not delete or free states; callers must manage state lifetimes (e.g., using static instances).
    }

    currentState = newState;

    if (currentState) {
        currentState->init(this);
    }
}

/**
 * @brief Update the active game state.
 *
 * Calls `update()` on the currently assigned `GameState` if one exists.
 */
void Game::update() {
    if (currentState) {
        currentState->update();
    }
}

/**
 * @brief Renders the active game state.
 *
 * If a current state is set, calls its draw() method to perform rendering.
 */
void Game::draw() {
    if (currentState) {
        currentState->draw();
    }
}

void Game::load() {
    SaveBlock tempSave;
    // Read from SRAM physical address
    u8* src = (u8*)sram_mem;
    u8* dst = (u8*)&tempSave;
    for (u32 i = 0; i < sizeof(SaveBlock); ++i) {
        dst[i] = src[i];
    }

    // 0x4742414D is "GBAM" in hex
    if (tempSave.magicSignature == 0x4742414D) {
        this->profile = tempSave.profile;
    }
    // If the signature doesn't match, we keep the default values set in the constructor.
}

void Game::save() {
    SaveBlock newSave;
    newSave.magicSignature = 0x4742414D;
    newSave.profile = this->profile;

    // Write to SRAM physical address
    u8* dst = (u8*)sram_mem;
    u8* src = (u8*)&newSave;
    for (u32 i = 0; i < sizeof(SaveBlock); ++i) {
        dst[i] = src[i];
    }
}
