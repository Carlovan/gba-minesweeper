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

GameDrawer::GameDrawer(Minesweeper &game_, Background &bgBack_, Background &bgSymbols_)
  : game{game_}, bgBack{bgBack_}, bgSymbols{bgSymbols_},
	upperLeft{(SCR_HT - game_.rows()) / 2, (SCR_WT - game_.columns()) / 2} {
		game_.setCellOpenedCallback(std::make_unique<DrawerCallback>(*this));
	}

void GameDrawer::draw_frame() {
	se_frame(bgBack.getSbb(),
		upperLeft.col - 1,
		upperLeft.row - 1,
		upperLeft.col + game.columns() + 1,
		upperLeft.row + game.rows()    + 1,
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


	// TODO following is for debug
	if (game.is_mine(pos)) {
		tileValue = Tiles::FRAME;
	}
	if (game.cursor() == pos) {
		tileValue = Tiles::NONE;
	}
	se_plot(bgBack.getSbb(), tilePos.col, tilePos.row, static_cast<SCR_ENTRY>(tileValue));
}

void GameDrawer::update_current() {
	update(game.cursor());
}
