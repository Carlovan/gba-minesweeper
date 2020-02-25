#include "game_drawer.h"
#include <tonc.h>
#include <functional>

enum class Tiles: SCR_ENTRY {
	NONE        = 0,
	FRAME       = 1,
	CLOSED_CELL = 2,
	OPENED_CELL = 3,
	FLAGGED_CELL = 4,
};

void GameDrawer::cell_opened_callback(const Minesweeper &game, size_t row, size_t col) {

}


GameDrawer::GameDrawer(Minesweeper &game_, Background &bgBack_, Background &bgSymbols_)
  : game{game_}, bgBack{bgBack_}, bgSymbols{bgSymbols_},
	upperLeft{(SCR_WT - game_.columns()) / 2, (SCR_HT - game_.rows()) / 2} {
		game_.setCellOpenedCallback(std::bind(&GameDrawer::cell_opened_callback, *this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

void GameDrawer::draw_frame() {
	se_frame(bgBack.getSbb(),
		upperLeft.x - 1,
		upperLeft.y - 1,
		upperLeft.x + game.columns() + 1,
		upperLeft.y + game.rows()    + 1,
		static_cast<SCR_ENTRY>(Tiles::FRAME));
}

void GameDrawer::draw_all() {
	draw_frame();

	for (size_t c = 0; c < game.columns(); c++) {
		for (size_t r = 0; r < game.rows(); r++) {
			update({r, c});
		}
	}
}

void GameDrawer::update(const Minesweeper::position_type pos) {
	auto tilePos = upperLeft + pos;
	Tiles tileValue = Tiles::NONE;
	if (game.at(pos) == CellState::OPENED) {
		tileValue = Tiles::OPENED_CELL;
	} else if (game.at(pos) == CellState::CLOSED) {
		tileValue = Tiles::CLOSED_CELL;
	} else if (game.at(pos) == CellState::OPENED) {
		tileValue = Tiles::FLAGGED_CELL;
	}
	se_plot(bgBack.getSbb(), tilePos.x, tilePos.y, static_cast<SCR_ENTRY>(tileValue));
}

void GameDrawer::update_current() {
	update(game.cursor());
}
