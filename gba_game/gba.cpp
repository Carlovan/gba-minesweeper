#include <cstring>

#include <tonc.h>
#include <backgrounds>

#include <background.h> // graphics

const int SCREEN_W = 30;
const int SCREEN_H = 20;
const int GRID_SIZE = 8;

// This is to test the behaviour of the Background class
void show() {
	std::optional maybeBg0 = Background::create(0, 0, 3, 0, BgBitDepht::BPP8, BgSize::REG_32x32);
	auto bgSbb = maybeBg0->getSbb();
	se_fill(bgSbb, 0);

	// Draw the grid
	int gx = (SCREEN_W - GRID_SIZE) / 2;
	int gy = (SCREEN_H - GRID_SIZE) / 2;

	se_frame(bgSbb, gx-1, gy-1, gx+GRID_SIZE+1, gy+GRID_SIZE+1, 1);
	se_rect(bgSbb, gx, gy, gx+GRID_SIZE, gy+GRID_SIZE, 2);
	se_mat[0][gy][gx] = 3;

	key_wait_till_hit(KEY_A);
}

int main() {
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// All 4 regular backgrounds
	REG_DISPCNT= DCNT_MODE0;

	// Copy palette and tiles (used in show() this is just a test)
	memcpy(pal_bg_mem, backgroundPal, backgroundPalLen);
	memcpy(tile8_mem[3], backgroundTiles, backgroundTilesLen);

	while(1) {
		key_wait_till_hit(KEY_A);
		show();
	}
}
