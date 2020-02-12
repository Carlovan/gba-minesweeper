#include <cstring>
#include <tonc.h>

#include <background.h>

const int SCREEN_W = 30;
const int SCREEN_H = 20;
const int GRID_SIZE = 8;

int main() {
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    // All 4 regular backgrounds. Activate BG0
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

    // Setup BG0
    REG_BG0CNT = BG_PRIO(0) | BG_CBB(3) | BG_SBB(0) | BG_8BPP | BG_REG_32x32;

    memcpy(pal_bg_mem, backgroundPal, backgroundPalLen);
    memcpy(tile8_mem[3], backgroundTiles, backgroundTilesLen);
    se_fill(se_mem[0], 0);

    // Draw the grid
    int gx = (SCREEN_W - GRID_SIZE) / 2;
    int gy = (SCREEN_H - GRID_SIZE) / 2;

    se_frame(se_mem[0], gx-1, gy-1, gx+GRID_SIZE+1, gy+GRID_SIZE+1, 1);
    se_rect(se_mem[0], gx, gy, gx+GRID_SIZE, gy+GRID_SIZE, 2);
    se_mat[0][gy][gx] = 3;

    while(1) {
        VBlankIntrWait();
    }
}
