#include <cstring>
#include <tonc.h>

#include <bg0.h>

int main() {
    // All 4 regular backgrounds. Activate BG0
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

    // Setup BG0
    REG_BG0CNT = BG_PRIO(0) | BG_CBB(3) | BG_SBB(0) | BG_8BPP | BG_REG_32x32;

    memcpy(pal_bg_mem, bg0Pal, bg0PalLen);
    memcpy(tile8_mem[3], bg0Tiles, bg0TilesLen);
    memcpy(se_mem[0], bg0Map, bg0MapLen);
}
