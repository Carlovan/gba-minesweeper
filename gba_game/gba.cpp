#include <cstring>

#include <tonc.h>
#include <backgrounds>
#include <minesweeper>
#include "game_drawer.h"

#include <background.h> // graphics

const int GRID_SIZE = 8;

void init_gba() {
	// Initialize interrupts
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// All 4 regular backgrounds
	REG_DISPCNT= DCNT_MODE0;
}

int main() {
	init_gba();

	Minesweeper game(GRID_SIZE);
	Background bgBackground = Background::create(0, 3, 3, 0, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	Background bgSymbols    = Background::create(1, 2, 3, 1, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	GameDrawer drawer(game, bgBackground, bgSymbols);

	// Init graphics
	memcpy(pal_bg_mem, backgroundPal, backgroundPalLen);
	memcpy(tile8_mem[3], backgroundTiles, backgroundTilesLen);

	drawer.draw_all();

	while(!game.finished()) {
		VBlankIntrWait();
		key_poll();

		// if (key_hit(KEY_B)) {
		// 	game.toggle_flag();
		// }
		// if (key_hit(KEY_A)) {
		// 	game.open();
		// }

		// game.moveh(key_tri_horz());
		// game.movev(key_tri_vert());
		drawer.update_current();
	}
}
