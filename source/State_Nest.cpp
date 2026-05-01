#include "State_Nest.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include <tonc.h>
#include <stdio.h>

State_Nest State_Nest::instance;

/**
 * @brief Constructs a State_Nest and initializes its internal state.
 *
 * Initializes the state's members: sets `game` to `nullptr`, `selectedOption` to `0`,
 * and `viewingCollection` to `false`.
 */
State_Nest::State_Nest() : game(nullptr), selectedOption(0), viewingCollection(false) {
}

/**
 * @brief Destructor for State_Nest.
 *
 * Performs no cleanup; provided for completeness and potential future use.
 */
State_Nest::~State_Nest() {
}

/**
 * @brief Initialize the Nest state with the provided game context and render its menu.
 *
 * Resets menu state (selection and collection view), stores the game context for later
 * state transitions and profile access, disables sprite/object display layers so the
 * screen is text-only, clears the UI, and draws the initial menu.
 *
 * @param gameContext Pointer to the current Game instance used for state changes and profile data.
 */
void State_Nest::init(Game* gameContext) {
    this->game = gameContext;
    selectedOption = 0;
    viewingCollection = false;

    // Disable objects completely, as this is a pure text menu
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

    UI::clear();
    drawMenu();
}

/**
 * @brief Render the Nest menu and collection screen to the UI.
 *
 * Clears the current UI and draws the Nest header including the player's agate count,
 * then either renders the collection view (showing how many of 5 pieces are held and a prompt to return)
 * or renders the two-option menu with a cursor marker placed according to `selectedOption`.
 *
 * Observes member state: `game->profile.agatesCollected`, `viewingCollection`, and `selectedOption`.
 */
void State_Nest::drawMenu() {
    UI::clear();
    char buf[64];
    sprintf(buf, "The Nest - Agates: %d", game->profile.agatesCollected);
    UI::print(16, 16, buf);

    if (viewingCollection) {
        sprintf(buf, "You hold %d of 5 pieces.", game->profile.agatesCollected);
        UI::print(16, 48, buf);
        UI::print(16, 64, "The rest are still out there.");
        UI::print(16, 96, "Press B to return");
    } else {
        if (selectedOption == 0) {
            UI::print(16, 48, "> 1. View Collection");
            UI::print(16, 64, "  2. Leave Nest (Enter Level 1)");
        } else {
            UI::print(16, 48, "  1. View Collection");
            UI::print(16, 64, "> 2. Leave Nest (Enter Level 1)");
        }
    }
}

/**
 * @brief Processes player input for the Nest menu and updates the menu state.
 *
 * When the collection view is active, pressing B returns to the main menu. When the collection view is inactive,
 * UP or DOWN toggles the selected menu option; A either opens the collection view or transitions the game to Level 1.
 * Side effects: updates `viewingCollection` and `selectedOption`, redraws the menu via `drawMenu()`, and may change
 * the game's state via `game->changeState(...)`.
 */
void State_Nest::update() {
    if (viewingCollection) {
        if (key_hit(KEY_B)) {
            viewingCollection = false;
            drawMenu();
        }
    } else {
        if (key_hit(KEY_UP) || key_hit(KEY_DOWN)) {
            selectedOption = 1 - selectedOption;
            drawMenu();
        }

        if (key_hit(KEY_A)) {
            if (selectedOption == 0) {
                viewingCollection = true;
                drawMenu();
            } else {
                game->changeState(&State_Level1::instance);
            }
        }
    }
}

/**
 * @brief Performs no per-frame rendering for the Nest state.
 *
 * This method is intentionally empty; the Nest state performs UI updates immediately
 * in its initialization and input-handling code rather than in a per-frame draw call.
 */
void State_Nest::draw() {
}

/**
 * @brief Clean up UI when exiting the Nest state.
 *
 * Clears all on-screen UI elements and text to leave the display in a neutral state.
 */
void State_Nest::teardown() {
    UI::clear();
}
