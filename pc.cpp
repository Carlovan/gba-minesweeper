/*
 * Giulio Carlassare, 2020-01-13
 * Computer version of the Minesweeper game.
 * This is used to test the code before writing the actual GBA code.
 * Writing the game for two different environments is also useful to
 * help separate the actual game logic from the user interface.
 */

#include <iostream>
#include <map>

#include <minesweeper>

enum class Command {
	UP = 'w',
	RIGHT = 'd',
	DOWN = 's',
	LEFT = 'a',
	FLAG ='f',
	OPEN = 'o'
};

const size_t GRID_SIZE = 8;
const std::string START_MSG = "Good luck!\n";
const std::string WON_MSG = "You won! :D";
const std::string LOST_MSG = "You lost :(\n";
const std::map<CellState, char> STATES_SYM = {{CellState::CLOSED, 'H'}, {CellState::FLAGGED, '!'}};

void draw_grid(const Minesweeper &game) {
	std::cout<<'+';
	for(size_t i = 0; i < game.columns(); i++)
		std::cout<<'-';
	std::cout<<"+\n";
	for(size_t r = 0; r < game.rows(); r++) {
		std::cout<<'|';
		for(size_t c = 0; c < game.columns(); c++) {
			if (game.finished() && game.is_mine(r, c)) {
				std::cout<<'*';
			} else if (std::pair{r, c} == game.cursor()) {
				std::cout<<'@';
			} else if (game.at(r, c) == CellState::OPENED) {
				auto neighbours = game.neighbours(r, c);
				std::cout<<(char)(neighbours ? neighbours + '0': ' ');
			} else {
				std::cout<<STATES_SYM.at(game.at(r, c));
			}
		}
		std::cout<<"|\n";
	}
	std::cout<<'+';
	for(size_t i = 0; i < game.columns(); i++)
		std::cout<<'-';
	std::cout<<'+';
}

Command read_command() {
	char cmd;
	std::cin>>cmd;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return static_cast<Command>(cmd);
}

int main() {
	Minesweeper game(GRID_SIZE);

	std::cout<<START_MSG;

	while(!game.finished()) {
		draw_grid(game);
		switch(read_command()) {
			case Command::UP:
				game.up();
				break;
			case Command::RIGHT:
				game.right();
				break;
			case Command::DOWN:
				game.down();
				break;
			case Command::LEFT:
				game.left();
				break;
			case Command::FLAG:
				game.toggle_flag();
				break;
			case Command::OPEN:
				game.open();
				break;
		}
	}

	draw_grid(game);

	if (game.won()) {
		std::cout<<WON_MSG;
	} else {
		std::cout<<LOST_MSG;
	}
}