#include "Background.h"

/**
 * @brief Loads background graphics and configuration into hardware memory and enables the specified background.
 *
 * Copies tile data into the character block `cbb`, map data into the screen block `sbb`, and palette data
 * into background palette memory. Builds and writes the background control word for the selected background
 * index with the chosen pixel format, and sets the corresponding display control bit to enable that background.
 *
 * @param tiles Pointer to source tile (graphics) data.
 * @param tileLen Length in bytes of the tile data.
 * @param map Pointer to source map (screen entry) data.
 * @param mapLen Length in bytes of the map data.
 * @param pal Pointer to source palette data.
 * @param palLen Length in bytes of the palette data.
 * @param cbb Character base block index where tiles will be copied.
 * @param sbb Screen base block index where map data will be copied.
 * @param bg_index Background index (0–3) whose control register will be configured and enabled.
 * @param is8bpp If true, configures the background for 8bpp tiles; otherwise configures for 4bpp.
 */
void Background::load(const void* tiles, unsigned int tileLen,
                      const void* map, unsigned int mapLen,
                      const void* pal, unsigned int palLen,
                      int cbb, int sbb, int bg_index, bool is8bpp) {

    // Copy tiles to Character Block Base
    memcpy32(tile_mem[cbb], tiles, tileLen / 4);

    // Copy map to Screen Block Base
    memcpy32(se_mem[sbb], map, mapLen / 4);

    // Copy palette to background palette memory
    memcpy32(pal_bg_mem, pal, palLen / 4);

    // Configure specific background register
    u16 bg_cnt = BG_CBB(cbb) | BG_SBB(sbb);
    if (is8bpp) {
        bg_cnt |= BG_8BPP;
    } else {
        bg_cnt |= BG_4BPP;
    }

    switch (bg_index) {
        case 0:
            REG_BG0CNT = bg_cnt;
            REG_DISPCNT |= DCNT_BG0;
            break;
        case 1:
            REG_BG1CNT = bg_cnt;
            REG_DISPCNT |= DCNT_BG1;
            break;
        case 2:
            REG_BG2CNT = bg_cnt;
            REG_DISPCNT |= DCNT_BG2;
            break;
        case 3:
            REG_BG3CNT = bg_cnt;
            REG_DISPCNT |= DCNT_BG3;
            break;
    }
}
