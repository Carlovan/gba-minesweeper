#include <cstring>
#include <vector>
#include <algorithm>

#include <tonc.h>
#include <backgrounds>
#include <menu>
#include <sprites>
#include <animators>
#include <random_helpers>
#include <minesweeper>
#include "game_drawer.h"

#include <gfx.h> // graphics
#include <cursor.h> // Cursor sprite
#include "tiles_def.h"

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

void play(Background &bgBackground, Background &bgSymbols, Sprite &sprCursor, const MinesweeperParams &params) {
	Minesweeper game(params);
	GameDrawer drawer(game, bgBackground, bgSymbols, sprCursor);

	SpriteBlinker cursorBlinker(sprCursor, 50, 0, 4);

	VBlankIntrWait();
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

std::vector<SCR_ENTRY> intToTiles(int n) {
	std::vector<SCR_ENTRY> tiles;
	while (n > 0) {
		int next = n / 10;
		int digit = (n - next * 10); // To avoid both division and modulo
		tiles.push_back(static_cast<SCR_ENTRY>(TilesIndexes::NUMBERS_BASE) + digit);
		n = next;
	}
	std::reverse(tiles.begin(), tiles.end());
	return tiles;
}

Menu createMenu(const std::vector<MinesweeperParams> &options) {
	Menu menu;
	menu.setTileSet({
		static_cast<SCR_ENTRY>(TilesIndexes::WOOD_BACK),
		static_cast<SCR_ENTRY>(TilesIndexes::RIGHT_ARROW),
		static_cast<SCR_ENTRY>(TilesIndexes::WOOD_HEDGE),
		static_cast<SCR_ENTRY>(TilesIndexes::WOOD_VEDGE),
		static_cast<SCR_ENTRY>(TilesIndexes::WOOD_CORNER),
	});

	for(const auto &params : options) {
		std::vector<SCR_ENTRY> gs = intToTiles(params.gridSize);
		std::vector<SCR_ENTRY> row;
		row.insert(row.end(), gs.begin(), gs.end());
		row.push_back(static_cast<SCR_ENTRY>(TilesIndexes::TIMES));
		row.insert(row.end(), gs.begin(), gs.end());
		menu.add(row);
	}

	return menu;
}

int main() {
	init_gba();

	Background bgBackground = Background::create(0, 3, 3, 0, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	Background bgSymbols    = Background::create(1, 2, 3, 1, BgBitDepht::BPP8, BgSize::REG_32x32).value();
	Sprite     sprCursor    = Sprite::create(0, 2, SprBitDepht::BPP8, SprSize::S16x16, 0).value();

	const std::vector<MinesweeperParams> options{{8}, {16}};
	Menu startMenu = createMenu(options);

	// Init graphics
	memcpy(pal_bg_mem, gfxPal, gfxPalLen);
	memcpy(tile8_mem[3], gfxTiles, gfxTilesLen);

	memcpy(pal_obj_mem, cursorPal, cursorPalLen);
	memcpy(tile8_mem[4], cursorTiles, cursorTilesLen);

	while(1) {
		se_fill(bgBackground.getSbb(), (SCR_ENTRY)TilesIndexes::GRASS);
		se_fill(bgSymbols.getSbb(), (SCR_ENTRY)TilesIndexes::NONE);
		auto params = options[startMenu.prompt(bgBackground, bgSymbols)];
		se_fill(bgSymbols.getSbb(), (SCR_ENTRY)TilesIndexes::NONE);
		play(bgBackground, bgSymbols, sprCursor, params);
	}
}
