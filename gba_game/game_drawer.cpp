#include "game_drawer.h"
#include "tiles_def.h"
#include <tonc.h>
#include <functional>

class DrawerCallback: public Minesweeper::cellopened_callback_type {
private:
	GameDrawer &drawer;

public:
	DrawerCallback(GameDrawer &drawer_): drawer{drawer_} {}

	void operator()(const Minesweeper &game, const Minesweeper::position_type pos) override {
		VBlankIntrDelay(2);
		drawer.update(pos);
	}
};

GameDrawer::GameDrawer(Minesweeper &game_, Background &bgBack_, Background &bgSymbols_, Sprite &sprCursor_)
  : game{game_}, bgBack{bgBack_}, bgSymbols{bgSymbols_}, sprCursor{sprCursor_},
	upperLeft{(SCR_HT - game_.rows()) / 2, (SCR_WT - game_.columns()) / 2} {
		game_.setCellOpenedCallback(std::make_unique<DrawerCallback>(*this));
		se_fill(bgBack_.getSbb(), static_cast<SCR_ENTRY>(TilesIndexes::GRASS));
	}

void GameDrawer::draw_frame() {
	se_frame(bgBack.getSbb(),
		upperLeft.col - 1,
		upperLeft.row - 1,
		upperLeft.col + game.columns() + 1,
		upperLeft.row + game.rows()    + 1,
		static_cast<SCR_ENTRY>(TilesIndexes::FRAME));
}

void GameDrawer::draw_all(bool showMines) {
	draw_frame();

	for (int c = 0; c < game.columns(); c++) {
		for (int r = 0; r < game.rows(); r++) {
			update({r, c}, showMines);
		}
	}

	update_cursor();
}

void GameDrawer::update(const Minesweeper::position_type pos, bool showMines) {
	auto tilePos = upperLeft + pos;
	TilesIndexes tileValue = TilesIndexes::NONE;
	TilesIndexes symbolValue = TilesIndexes::NONE;

	if (game.at(pos) == CellState::OPENED) {
		tileValue = TilesIndexes::OPENED_CELL;
	} else {
		tileValue = TilesIndexes::CLOSED_CELL;
	}

	if (game.at(pos) == CellState::OPENED && game.neighbours(pos) > 0) {
		symbolValue = TilesIndexes::NUMBERS_BASE;
	}
	if (game.at(pos) == CellState::FLAGGED) {
		symbolValue = TilesIndexes::FLAG;
	}
	if (showMines && game.is_mine(pos)) {
		symbolValue = TilesIndexes::MINE;
	}

	SCR_ENTRY backgroundEntry = static_cast<SCR_ENTRY>(tileValue);
	se_plot(bgBack.getSbb(), tilePos.col, tilePos.row, backgroundEntry);

	SCR_ENTRY symbolEntry = static_cast<SCR_ENTRY>(symbolValue);
	if (symbolValue == TilesIndexes::NUMBERS_BASE) {
		symbolEntry += game.neighbours(pos);
	}
	se_plot(bgSymbols.getSbb(), tilePos.col, tilePos.row, symbolEntry);
}

void GameDrawer::update_current() {
	update(game.cursor());
}

void GameDrawer::update_cursor() {
	auto [y, x] = (game.cursor() + upperLeft) * 8 + cursorOffset;
	sprCursor.position(x, y);
}