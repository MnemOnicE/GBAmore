#include "State_Level1.h"
#include "Background.h"
#include "bg_level1.h"
#include "State_Nest.h"
#include "State_Map.h"
#include "Game.h"
#include "UI.h"
#include "chipmunk.h"
#include "shadow.h"
#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"

State_Level1 State_Level1::instance;

/**
 * @brief Initialize the Level 1 state with default member values.
 *
 * Sets the game context pointer to nullptr and clears the cutscene flag.
 */
State_Level1::State_Level1() : game(nullptr), inCutscene(false) {
}

State_Level1::~State_Level1() {
}

/**
 * @brief Initialize Level 1 state with its graphics, entities, and game context.
 *
 * Sets up the level's UI text, loads player and shadow sprite tiles and palettes,
 * initializes the entity array (player and patrolling shadow), enables object
 * rendering, prepares the shadow OAM buffer, and loads the BG for Level 1.
 *
 * @param gameContext Pointer to the current Game instance used by the state.
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

    // Load the Chipmunk into Tile Index 0 and PalBank 0:
    memcpy32(&tile_mem[4][0], chipmunkTiles, chipmunkTilesLen / 4);
    memcpy32(pal_obj_bank[0], chipmunkPal, chipmunkPalLen / 4);

    // Load the Shadow into Tile Index 8 (assuming 16x16 chipmunk is 8 tiles) and PalBank 1:
    memcpy32(&tile_mem[4][8], shadowTiles, shadowTilesLen / 4);
    memcpy32(pal_obj_bank[1], shadowPal, shadowPalLen / 4);

    num_entities = 2;
    // Player
    entities[0] = {112, 72, 0, 0, 16, 16, 0, true};
    // Patrolling Shadow
    entities[1] = {180, 100, 1, 0, 16, 16, 1, true};

    // Set inactive for remaining entities just to be safe
    for (int i = 2; i < 16; i++) {
        entities[i].active = false;
    }

    // Enable Objects (sprites) and 1D object mapping
    REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

    // Load background via Background wrapper (BG1, CBB 1, SBB 29, 4bpp)
    Background::load(bg_level1Tiles, bg_level1TilesLen, bg_level1Map, bg_level1MapLen, bg_level1Pal, bg_level1PalLen, 1, 29, 1, false);
}

/**
 * @brief Determines whether two active entities' axis-aligned bounding boxes overlap.
 *
 * Checks that both entities are marked active and that their rectangular bounds intersect.
 *
 * @param a First entity to test.
 * @param b Second entity to test.
 * @return true if both entities are active and their bounding rectangles overlap, false otherwise.
 */
bool State_Level1::checkCollision(Entity& a, Entity& b) {
    if (!a.active || !b.active) return false;
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

/**
 * @brief Updates Level 1 state: input, movement, camera, entities, collisions, and win transition.
 *
 * Processes player input and applies movement with screen-edge clamping, recenters and clamps the camera to the 256×256 map and scrolls the background, advances the patrolling shadow entity, and checks collisions (collision with the shadow resets the player and plays the hurt SFX). Handles immediate state transitions: Select opens the map; interacting with the goal region by pressing A plays the level-up SFX, increments the profile's agate count, enters a cutscene that disables sprites and shows the completion UI; when already in the cutscene pressing A returns to the Nest state.
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

    entities[0].dx = dx;
    entities[0].dy = dy;

    entities[0].x += entities[0].dx;
    entities[0].y += entities[0].dy;

    // Clamp coordinates to screen boundaries
    if (entities[0].x < 0) entities[0].x = 0;
    if (entities[0].x > 256 - 16) entities[0].x = 256 - 16;
    if (entities[0].y < 0) entities[0].y = 0;
    if (entities[0].y > 256 - 16) entities[0].y = 256 - 16;

    int camera_x = entities[0].x - (240 / 2) + (16 / 2); // Center X
    int camera_y = entities[0].y - (160 / 2) + (16 / 2); // Center Y

    // Clamp camera to the 256x256 map bounds
    if (camera_x < 0) camera_x = 0;
    if (camera_x > 256 - 240) camera_x = 256 - 240;
    if (camera_y < 0) camera_y = 0;
    if (camera_y > 256 - 160) camera_y = 256 - 160;

    // Scroll the background
    REG_BG1HOFS = camera_x;
    REG_BG1VOFS = camera_y;

    // Make the Shadow patrol:
    entities[1].x += entities[1].dx;
    if (entities[1].x < 150 || entities[1].x > 210) entities[1].dx *= -1;

    // Check collision:
    if (checkCollision(entities[0], entities[1])) {
        entities[0].x = 112; // Soft Reset
        entities[0].y = 72;
        mmEffect(SFX_HURT);  // Trigger Maxmod
    }

    // Check win condition
    if (entities[0].x >= 112 && entities[0].x < 128 && entities[0].y >= 16 && entities[0].y < 32) {
        if (key_hit(KEY_A)) {
            mmEffect(SFX_LEVELUP);
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

/**
 * @brief Renders active entity sprites into OAM and updates hardware OAM for the current camera.
 *
 * When not in a cutscene, computes a camera centered on the player and clamped to the map,
 * populates the shadow OAM buffer with up to `num_entities` active entities (setting palette,
 * shape/size, and screen position) and hides remaining OAM slots, then copies the buffer to
 * hardware OAM so sprites are drawn with the current camera offset.
 */
void State_Level1::draw() {
    // Nothing to do for now, handled by TTE printing directly

    if (!inCutscene) {
        int camera_x = entities[0].x - (240 / 2) + (16 / 2); // Center X
        int camera_y = entities[0].y - (160 / 2) + (16 / 2); // Center Y
        if (camera_x < 0) camera_x = 0;
        if (camera_x > 256 - 240) camera_x = 256 - 240;
        if (camera_y < 0) camera_y = 0;
        if (camera_y > 256 - 160) camera_y = 256 - 160;

        for (int i = 0; i < 128; i++) {
            if (i < num_entities && entities[i].active) {
                u16 attr2 = (entities[i].type == 0) ? (ATTR2_PALBANK(0) | 0) : (ATTR2_PALBANK(1) | 8);
                obj_set_attr(&obj_buffer[i],
                             ATTR0_SQUARE,
                             ATTR1_SIZE_16,
                             attr2);
                obj_set_pos(&obj_buffer[i], entities[i].x - camera_x, entities[i].y - camera_y);
            } else {
                obj_set_attr(&obj_buffer[i], ATTR0_HIDE, 0, 0);
            }
        }

        // Copy the shadowed OAM buffer to hardware OAM memory
        oam_copy(oam_mem, obj_buffer, 128);
    }
}

/**
 * @brief Tear down Level 1 and restore display state for the next scene.
 *
 * Resets background scroll to (0,0), clears UI text, clears the shadow OAM buffer and copies it to hardware to remove sprites, and disables object rendering and background 1 so subsequent states start with a clean display.
 */
void State_Level1::teardown() {
    // Reset background scroll to prevent bleeding into other states
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;

    UI::clear();

    // Clear out OAM and update hardware to remove the sprite
    oam_init(obj_buffer, 128);
    oam_copy(oam_mem, obj_buffer, 128);

    // Disable objects
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);

    // Disable background 1
    REG_DISPCNT &= ~DCNT_BG1;
}
