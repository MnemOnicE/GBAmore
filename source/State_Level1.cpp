#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include "chipmunk.h"
#include <tonc.h>

/**
 * @brief Construct a Level 1 game state with no associated Game context.
 *
 * Initializes the internal `game` pointer to `nullptr`.
 */
State_Level1::State_Level1() : game(nullptr) {
}

/**
 * @brief Destroy the Level 1 state instance.
 *
 * No explicit cleanup is performed by this destructor; any owned resources
 * are managed externally or have no teardown requirements.
 */
State_Level1::~State_Level1() {
}

/**
 * @brief Initialize Level 1 state with the provided game context.
 *
 * Stores the provided game context pointer, clears the UI, and prints the
 * level title ("Level 1: Memory Prototype") at screen coordinates (16, 64).
 *
 * @param gameContext Pointer to the current Game context to be stored by the state.
 */
void State_Level1::init(Game* gameContext) {
    this->game = gameContext;

    // Clear screen
    UI::clear();

    // Print level text
    UI::print(16, 64, "Level 1: Memory Prototype");

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

}

/**
 * @brief Executes per-frame update logic for the Level 1 game state.
 *
 * Currently no update behavior is implemented.
 */
void State_Level1::update() {
    // Nothing for now

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

}

/**
 * @brief No-op render step for the Level 1 state.
 *
 * The level's visual output is produced elsewhere (direct TTE printing), so this
 * draw hook intentionally performs no rendering.
 */
void State_Level1::draw() {
    // Nothing to do for now, handled by TTE printing directly

    // Copy the shadowed OAM buffer to hardware OAM memory
    oam_copy(oam_mem, obj_buffer, 1);

}

/**
 * @brief Tear down the level and clear on-screen UI.
 *
 * Ensures any UI elements printed for this state are removed from the display.
 */
void State_Level1::teardown() {

    UI::clear();

    // Clear out OAM and update hardware to remove the sprite
    oam_init(obj_buffer, 128);
    oam_copy(oam_mem, obj_buffer, 1);

    // Disable objects
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

}
