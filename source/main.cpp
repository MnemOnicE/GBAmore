#include <tonc.h>

int main()
{
    // Initialize the GBA display for Mode 0 (tiled)
    // We enable Background 0 (BG0) which is what TTE uses by default
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    // Initialize the Tonc Text Engine (TTE) for a 4bpp character background
    // This provides a hardware-accelerated way to print text in Mode 0
    tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

    // Print a "Hello World" message to verify it works
    tte_write("#{P:72,64}Hello, GBAmore!");

    // Basic game loop
    while(1)
    {
        // Wait for VBlank (vertical sync) to avoid tearing and save battery
        vid_vsync();
    }

    return 0;
}
