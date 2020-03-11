#include <cstring>

#include <tonc.h>
#include <backgrounds>
#include <sprites>
#include <animators>
#include <random_helpers>
#include <minesweeper>
#include "game_drawer.h"

#include <gfx.h> // graphics
#include <cursor.h> // Cursor sprite

const int GRID_SIZE = 16;

void init_gba() {
	// Initialize interrupts
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// All 4 regular backgrounds
	REG_DISPCNT = DCNT_MODE0;

	initialize_random_system();
	initialize_sprites();

	key_repeat_limits(20, 7);
	key_repeat_mask(KEY_RIGHT | KEY_LEFT | KEY_DOWN | KEY_UP);
}

int key_rpt_tri_horz() {
	return KEY_TRIBOOL(key_repeat, KI_RIGHT, KI_LEFT);
}

int key_rpt_tri_vert() {
	return KEY_TRIBOOL(key_repeat, KI_DOWN, KI_UP);
}

void play(Background &bgBackground, Background &bgSymbols, Sprite &sprCursor) {
	VBlankIntrWait();
	Minesweeper game(GRID_SIZE);
	GameDrawer drawer(game, bgBackground, bgSymbols, sprCursor);

	SpriteBlinker cursorBlinker(sprCursor, 50, 0, 4);

	drawer.draw_all();
	sprCursor.hidden(false);

	while(!game.finished()) {
		VBlankIntrWait();
		key_poll();

		if (key_hit(KEY_B)) {
			game.toggle_flag();
		}
		if (key_hit(KEY_A)) {
			game.open();
		}

		game.moveh(key_rpt_tri_horz());
		game.movev(key_rpt_tri_vert());
		drawer.update_current();
		drawer.update_cursor();
		cursorBlinker.animate();
	}

	drawer.draw_all(true);

	key_wait_till_hit(KEY_A);
}

int main() {
	init_gba();

	Background bgBackground = Background::create(0, 3, 3, 0, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	Background bgSymbols    = Background::create(1, 2, 3, 1, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	Sprite     sprCursor    = Sprite::create(0, 2, SprBitDepht::BPP8, SprSize::S16x16, 0).value();

	// Init graphics
	memcpy(pal_bg_mem, gfxPal, gfxPalLen);
	memcpy(tile8_mem[3], gfxTiles, gfxTilesLen);

	memcpy(pal_obj_mem, cursorPal, cursorPalLen);
	memcpy(tile8_mem[4], cursorTiles, cursorTilesLen);

	while(1) {
		play(bgBackground, bgSymbols, sprCursor);
	}
}
