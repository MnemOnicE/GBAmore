#include "State_Level1.h"
#include "Background.h"
#include "bg_level1.h"
#include "State_Nest.h"
#include "State_Map.h"
#include "Game.h"
#include "UI.h"
#include "chipmunk.h"
#include <tonc.h>
State_Level1 State_Level1::instance;

State_Level1::State_Level1() : game(nullptr), inCutscene(false) {
}

State_Level1::~State_Level1() {
}

/**
 * @brief Initialize Level 1 state and configure graphics, UI, and player sprite.
 *
 * Sets the active game context, clears cutscene state, prepares on-screen text,
 * initializes and uploads sprite graphics and palette to object VRAM, configures
 * the player sprite's OAM attributes and position, enables sprite rendering and
 * 1D object mapping, and loads the level background into VRAM.
 *
 * @param gameContext Pointer to the active Game instance used by this state.
 */
void State_Level1::init(Game* gameContext) {
    this->game = gameContext;
    inCutscene = false;

    // Clear screen
    UI::clear();

    // Print level text
    UI::print(16, 64, "Level 1: Memory Prototype");

    // Print Agate
    UI::print(112, 16, "*");

    // Initialize shadowed OAM buffer
    oam_init(obj_buffer, 128);

    // Copy chipmunk tiles and palette to object VRAM
    memcpy32(tile_mem_obj[0], chipmunkTiles, chipmunkTilesLen / 4);
    memcpy32(pal_obj_mem, chipmunkPal, chipmunkPalLen / 4);

    // Set up object attributes for our player sprite
    // ATTR0_SQUARE: 16x16 is square
    // ATTR0_COLOR_16: 4bpp (16 colors)
    // ATTR1_SIZE_16: 16x16 sprite size
    // ATTR2_PALBANK(0): Use palette bank 0
    // ATTR2_ID(0): Start at tile index 0
    obj_set_attr(&obj_buffer[0],
                 ATTR0_SQUARE,
                 ATTR1_SIZE_16,
                 ATTR2_PALBANK(0) | 0);

    // Initial position
    player_x = 112; // Middle of screen (240/2 - 16/2)
    player_y = 72;  // Middle of screen (160/2 - 16/2)
    obj_set_pos(&obj_buffer[0], player_x, player_y);

    // Enable Objects (sprites) and 1D object mapping
    REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

    // Load background via Background wrapper (BG1, CBB 1, SBB 29, 8bpp)
    Background::load(bg_level1Tiles, bg_level1TilesLen, bg_level1Map, bg_level1MapLen, bg_level1Pal, bg_level1PalLen, 1, 29, 1, true);

}

/**
 * @brief Process input, update the player sprite, and handle state transitions for Level 1.
 *
 * Processes controller input to move the player sprite and clamps its position to the screen.
 * While not in a cutscene, directional keys move the player (2 px per update) and SELECT immediately transitions to the map state.
 * When the player is inside the Agate trigger region (x ∈ [112,128), y ∈ [16,32)) pressing A starts the Level‑end cutscene:
 * it sets `inCutscene`, increments `game->profile.agatesCollected`, clears and prints the Agate UI messages, and disables OBJ rendering.
 * While in a cutscene, pressing A transitions to the Nest state; all other input and movement are ignored.
 */
void State_Level1::update() {
    if (inCutscene) {
        if (key_hit(KEY_A)) {
            game->changeState(&State_Nest::instance);
        }
        return;
    }

    if (key_hit(KEY_SELECT)) {
        game->changeState(&State_Map::instance);
        return;
    }

    int dx = 0;
    int dy = 0;

    if (key_is_down(KEY_UP))    dy -= 2;
    if (key_is_down(KEY_DOWN))  dy += 2;
    if (key_is_down(KEY_LEFT))  dx -= 2;
    if (key_is_down(KEY_RIGHT)) dx += 2;

    player_x += dx;
    player_y += dy;

    // Clamp coordinates to screen boundaries
    if (player_x < 0) player_x = 0;
    if (player_x > 240 - 16) player_x = 240 - 16;
    if (player_y < 0) player_y = 0;
    if (player_y > 160 - 16) player_y = 160 - 16;

    // Update position in OAM buffer
    obj_set_pos(&obj_buffer[0], player_x, player_y);

    // Check win condition
    if (player_x >= 112 && player_x < 128 && player_y >= 16 && player_y < 32) {
        if (key_hit(KEY_A)) {
            inCutscene = true;
            game->profile.agatesCollected++;
            UI::clear();
            UI::print(16, 64, "You found an Agate!");
            UI::print(16, 80, "Press A to return to Nest");
            // Disable sprite
            REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);
        }
    }
}

void State_Level1::draw() {
    // Nothing to do for now, handled by TTE printing directly

    if (!inCutscene) {
        // Copy the shadowed OAM buffer to hardware OAM memory
        oam_copy(oam_mem, obj_buffer, 1);
    }
}

/**
 * @brief Tear down Level 1 state and restore display hardware to a neutral state.
 *
 * Clears the on-screen UI, removes sprites from object attribute memory, and
 * disables object rendering and background 1 by updating display control flags.
 */
void State_Level1::teardown() {

    UI::clear();

    // Clear out OAM and update hardware to remove the sprite
    oam_init(obj_buffer, 128);
    oam_copy(oam_mem, obj_buffer, 1);

    // Disable objects
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

    // Disable background 1
    REG_DISPCNT &= ~DCNT_BG1;

}
