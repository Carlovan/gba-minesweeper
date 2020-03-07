#include "game_drawer.h"
#include <tonc.h>
#include <functional>

enum class Tiles: SCR_ENTRY {
	NONE        = 0,
	GRASS       = 1,
	FRAME,
	CLOSED_CELL,
	OPENED_CELL,
	LAST_TILE_
};

enum class SymbolTiles: SCR_ENTRY {
	NONE = 0,
	FLAG = static_cast<SCR_ENTRY>(Tiles::LAST_TILE_),
	MINE,
	NUMBERS_BASE,
};

class DrawerCallback: public Minesweeper::cellopened_callback_type {
private:
	GameDrawer &drawer;

public:
	DrawerCallback(GameDrawer &drawer_): drawer{drawer_} {}

	void operator()(const Minesweeper &game, const Minesweeper::position_type pos) override {
		VBlankIntrDelay(5);
		drawer.update(pos);
	}
};

GameDrawer::GameDrawer(Minesweeper &game_, Background &bgBack_, Background &bgSymbols_, Sprite &sprCursor_)
  : game{game_}, bgBack{bgBack_}, bgSymbols{bgSymbols_}, sprCursor{sprCursor_},
	upperLeft{(SCR_HT - game_.rows()) / 2, (SCR_WT - game_.columns()) / 2} {
		game_.setCellOpenedCallback(std::make_unique<DrawerCallback>(*this));
		se_fill(bgBack_.getSbb(), static_cast<SCR_ENTRY>(Tiles::GRASS));
	}

void GameDrawer::draw_frame() {
	se_frame(bgBack.getSbb(),
		upperLeft.col - 1,
		upperLeft.row - 1,
		upperLeft.col + game.columns() + 1,
		upperLeft.row + game.rows()    + 1,
		static_cast<SCR_ENTRY>(Tiles::FRAME));
}

void GameDrawer::draw_all(bool showMines) {
	draw_frame();

	for (size_t c = 0; c < game.columns(); c++) {
		for (size_t r = 0; r < game.rows(); r++) {
			update({r, c}, showMines);
		}
	}

	update_cursor();
}

void GameDrawer::update(const Minesweeper::position_type pos, bool showMines) {
	auto tilePos = upperLeft + pos;
	Tiles tileValue = Tiles::NONE;
	SymbolTiles symbolValue = SymbolTiles::NONE;

	if (game.at(pos) == CellState::OPENED) {
		tileValue = Tiles::OPENED_CELL;
	} else {
		tileValue = Tiles::CLOSED_CELL;
	}

	if (game.at(pos) == CellState::OPENED && game.neighbours(pos) > 0) {
		symbolValue = SymbolTiles::NUMBERS_BASE;
	}
	if (game.at(pos) == CellState::FLAGGED) {
		symbolValue = SymbolTiles::FLAG;
	}
	if (showMines && game.is_mine(pos)) {
		symbolValue = SymbolTiles::MINE;
	}

	SCR_ENTRY backgroundEntry = static_cast<SCR_ENTRY>(tileValue);
	se_plot(bgBack.getSbb(), tilePos.col, tilePos.row, backgroundEntry);

	SCR_ENTRY symbolEntry = static_cast<SCR_ENTRY>(symbolValue);
	if (symbolValue == SymbolTiles::NUMBERS_BASE) {
		symbolEntry += game.neighbours(pos);
	}
	se_plot(bgSymbols.getSbb(), tilePos.col, tilePos.row, symbolEntry);
}

void GameDrawer::update_current() {
	update(game.cursor());
}

void GameDrawer::update_cursor() {
	auto [y, x] = (game.cursor() + upperLeft) * 8;
	sprCursor.position(x, y);
}