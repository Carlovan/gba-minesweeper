#include <cstring>
#include <tonc.h>

int main() {
    // All 4 regular backgrounds. Activate BG0
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

    // Setup BG0
    REG_BG0CNT = BG_PRIO(0) | BG_CBB(3) | BG_SBB(0) | BG_8BPP | BG_REG_32x32;

    // Set palette
    COLOR pal[] = {CLR_BLACK, CLR_RED, CLR_BLUE};
    memcpy(pal_bg_mem, pal, sizeof(pal));

    // Create tiles
    const TILE8 tiles[3]=
    {
        {{0, 0, 0, 0, 0, 0, 0, 0}},
        {{0x02010201, 0x02010201, 0x02010201, 0x02010201,
          0x02010201, 0x02010201, 0x02010201, 0x02010201,
          0x02010201, 0x02010201, 0x02010201, 0x02010201,
          0x02010201, 0x02010201, 0x02010201, 0x02010201}},
        {{0x00000000, 0x00100100, 0x01100110, 0x00011000,
          0x00011000, 0x01100110, 0x00100100, 0x00000000,
          0x00011000, 0x01100110, 0x00100100, 0x00000000,
          0x00011000, 0x01100110, 0x00100100, 0x00000000}},
    };
    tile8_mem[3][0]= tiles[0];
    tile8_mem[3][1]= tiles[1];
    
    // Tilemap
    se_mem[0][0] = 1;
    se_mem[0][33] = 1;
}
