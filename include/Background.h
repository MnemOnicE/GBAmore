#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <tonc.h>

class Background {
public:
    static void load(const void* tiles, unsigned int tileLen,
                     const void* map, unsigned int mapLen,
                     const void* pal, unsigned int palLen,
                     int cbb, int sbb, int bg_index, bool is8bpp);
};

#endif // BACKGROUND_H
