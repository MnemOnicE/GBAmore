#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <tonc.h>

/**
 * Load background tiles, map, and palette into VRAM and configure a GBA background layer.
 *
 * Copies the provided tile graphics, screen map, and palette data into the appropriate
 * video memory blocks and sets up the background specified by `bg_index`.
 *
 * @param tiles Pointer to tile graphics data.
 * @param tileLen Size of `tiles` in bytes.
 * @param map Pointer to screen map data.
 * @param mapLen Size of `map` in bytes.
 * @param pal Pointer to palette data.
 * @param palLen Size of `pal` in bytes.
 * @param cbb Character (tile) base block index to load tiles into (0-3).
 * @param sbb Screen base block index to load the map into (0-31).
 * @param bg_index Background layer index to configure (typically 0-3).
 * @param is8bpp If `true`, tiles are treated as 8 bits-per-pixel; if `false`, 4 bpp is used.
 */
class Background {
public:
    static void load(const void* tiles, unsigned int tileLen,
                     const void* map, unsigned int mapLen,
                     const void* pal, unsigned int palLen,
                     int cbb, int sbb, int bg_index, bool is8bpp);
};

#endif // BACKGROUND_H
