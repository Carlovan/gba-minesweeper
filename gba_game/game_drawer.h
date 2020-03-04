#ifndef GAME_DRAWER_H
#define GAME_DRAWER_H

#include <minesweeper>
#include <backgrounds>
#include <sprites>

class GameDrawer {
private:
	const Minesweeper &game;
	Background &bgBack;
	Background &bgSymbols;
	Sprite     &sprCursor;
	const Minesweeper::position_type upperLeft;

	void draw_frame();
	void cell_opened_callback(const Minesweeper &game, size_t row, size_t col);

public:
	GameDrawer(Minesweeper &game_,
		Background &bgBack_, Background &bgSymbols_,
		Sprite &sprCursor_);

	void draw_all(bool showMines = false);
	void update(const Minesweeper::position_type pos, bool showMines = false);
	void update_current();
	void update_cursor();
};
#endif