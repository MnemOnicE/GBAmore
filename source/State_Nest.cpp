#include "State_Nest.h"
#include "State_Level1.h"
#include "Game.h"
#include "UI.h"
#include "Utils.h"
#include "chipmunk.h"
#include <tonc.h>

const int BED_ZONE_X = 50;
const int BED_ZONE_Y = 40;
const int TV_ZONE_X = 140;
const int TV_ZONE_Y = 40;
const int RING_ZONE_X = 50;
const int RING_ZONE_Y = 100;
const int DOOR_ZONE_X = 140;
const int DOOR_ZONE_Y = 100;


State_Nest State_Nest::instance;

State_Nest::State_Nest() : game(nullptr), interacting(false) {
}

State_Nest::~State_Nest() {
}

static void drawLabels() {
    UI::print(16, 16, "Bed (Save)");
    UI::print(160, 16, "TV (Lore)");
    UI::print(16, 128, "Ring (Agates)");
    UI::print(160, 128, "Door (Level 1)");
}

void State_Nest::init(Game* gameContext) {
    this->game = gameContext;
    interacting = false;

    UI::clear();
    drawLabels();

    oam_init(obj_buffer, 128);

    memcpy32(tile_mem_obj[0], chipmunkTiles, chipmunkTilesLen / 4);
    memcpy32(pal_obj_mem, chipmunkPal, chipmunkPalLen / 4);

    obj_set_attr(&obj_buffer[0],
                 ATTR0_SQUARE,
                 ATTR1_SIZE_16,
                 ATTR2_PALBANK(0) | 0);

    player_x = 112;
    player_y = 72;
    obj_set_pos(&obj_buffer[0], player_x, player_y);

    REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;
}

static void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    int j, c;
    for (j = 0, i = i - 1; j < i; j++, i--) {
        c = s[j];
        s[j] = s[i];
        s[i] = c;
    }
}

static void strConcat(char* dest, int max_len, const char* src1, const char* src2, const char* src3) {
    if (max_len <= 0) return;
    int i = 0;
    while (*src1 && i < max_len - 1) dest[i++] = *src1++;
    while (*src2 && i < max_len - 1) dest[i++] = *src2++;
    if (src3) {
        while (*src3 && i < max_len - 1) dest[i++] = *src3++;
    }
    dest[i] = '\0';
}

void State_Nest::update() {
    if (interacting) {
        if (key_hit(KEY_A) || key_hit(KEY_B)) {
            interacting = false;
            UI::clear();
            drawLabels();
        }
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

    if (player_x < 0) player_x = 0;
    if (player_x > 240 - 16) player_x = 240 - 16;
    if (player_y < 0) player_y = 0;
    if (player_y > 160 - 16) player_y = 160 - 16;

    obj_set_pos(&obj_buffer[0], player_x, player_y);

    if (key_hit(KEY_A)) {
        if (player_x > DOOR_ZONE_X && player_y > DOOR_ZONE_Y) {
            // Door
            game->changeState(&State_Level1::instance);
        } else if (player_x < BED_ZONE_X && player_y < BED_ZONE_Y) {
            // Bed
            game->save();
            UI::clear();
            UI::print(64, 72, "Game Saved.");
            interacting = true;
        } else if (player_x > TV_ZONE_X && player_y < TV_ZONE_Y) {
            // TV
            UI::clear();
            UI::print(64, 72, "The static hums...");
            interacting = true;
        } else if (player_x < RING_ZONE_X && player_y > RING_ZONE_Y) {
            // Ring
            UI::clear();
            char buf[64];
            char numBuf[16];
            itoa(game->profile.agatesCollected, numBuf);
            strConcat(buf, sizeof(buf), "Agates: ", numBuf, nullptr);
            UI::print(64, 72, buf);
            interacting = true;
        }
    }
}

void State_Nest::draw() {
    if (!interacting) {
        oam_copy(oam_mem, obj_buffer, 1);
    }
}

void State_Nest::teardown() {
    UI::clear();
    oam_init(obj_buffer, 128);
    oam_copy(oam_mem, obj_buffer, 1);
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_OBJ_1D);
}
