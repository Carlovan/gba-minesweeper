#include <minesweeper>
#include <random_helpers>

Minesweeper::Minesweeper(const size_t gridSize):
	cells(gridSize, std::vector(gridSize, static_cast<char>(CellState::CLOSED))),
	mines(gridSize, std::vector(gridSize, false)) {}

size_t Minesweeper::columns() const {
	return cells.size() > 0 ? cells[0].size() : 0;
}

size_t Minesweeper::rows() const {
	return cells.size();
}

CellState Minesweeper::at(const size_t row, const size_t col) const {
	if (row >= rows() || col >= columns())
		return CellState::CLOSED;
	if (cells[row][col] >= 0)
		return CellState::OPENED;
	return static_cast<CellState>(cells[row][col]);
}

CellState Minesweeper::at(const std::pair<size_t, size_t> pos) const {
	return at(pos.first, pos.second);
}

bool Minesweeper::is_mine(const size_t row, const size_t col) const {
	if (row >= rows() || col >= columns())
		return false;
	return mines[row][col];
}

size_t Minesweeper::neighbours(const size_t row, const size_t col) const {
	return at(row, col) == CellState::OPENED ? cells[row][col] : 0;
}

bool Minesweeper::finished() const {
	return lost || won();
}

bool Minesweeper::won() const {
	return rows() * columns() == minesCount + openedCells;
}

Minesweeper::position_type Minesweeper::cursor() const {
	return cursorPosition;
}

void Minesweeper::setCellOpenedCallback(cellopened_callback_type callback) {
	cellopenedCallback = callback;
}

void Minesweeper::removeCellOpenedCallback() {
	setCellOpenedCallback(nullptr);
}

void Minesweeper::up() {
	if (cursorPosition.first > 0)
		cursorPosition.first--;
}

void Minesweeper::right() {
	if (cursorPosition.second < columns()-1)
		cursorPosition.second++;
}

void Minesweeper::down() {
	if (cursorPosition.first < rows()-1)
		cursorPosition.first++;
}

void Minesweeper::left() {
	if (cursorPosition.second > 0)
		cursorPosition.second--;
}

void Minesweeper::toggle_flag() {
	if (at(cursor()) != CellState::OPENED) {
		cells[cursor().first][cursor().second] = static_cast<char>(at(cursor()) == CellState::CLOSED ? CellState::FLAGGED : CellState::CLOSED);
	}
}

void Minesweeper::open() {
	if (!initialized) {
		initialize_mines();
		initialized = true;
	}
	if (at(cursor()) == CellState::CLOSED) {
		open_cell(cursor());
	}
}

// Also counts the given position
size_t Minesweeper::count_neighbours(const position_type pos) const {
	size_t count = 0;
	for(int v = -1; v <= 1; v++) {
		for(int h = -1; h <= 1; h++) {
			size_t r = pos.first + v;
			size_t c = pos.second + h;
			if (r < rows() && c < columns()) {
				count += mines[r][c];
			}
		}
	}
	return count;
}

void Minesweeper::open_cell(const position_type pos) {
	if (mines[pos.first][pos.second]) {
		lost = true;
	} else if (at(pos) != CellState::OPENED) {
		const auto minesNear = count_neighbours(pos);
		cells[pos.first][pos.second] = minesNear;
		openedCells++;
		if (cellopenedCallback != nullptr) cellopenedCallback(*this, pos.first, pos.second);
		// If there are no mines near me open the neighbours
		if (minesNear == 0) {
			for (int v = -1; v <= 1; v++) {
				for(int h = -1; h <= 1; h++) {
					size_t r = pos.first + v;
					size_t c = pos.second + h;
					if ((h != 0 || v != 0) && r < rows() && c < columns()) {
						open_cell({r, c});
					}
				}
			}
		}
	}
}

void Minesweeper::initialize_mines() {
	minesCount = rows() * columns() / 8;

	RandomSize random;
	for(size_t i = 0; i < minesCount; i++) {
		size_t row, col;
		do {
			row = random(rows()-1);
			col = random(columns()-1);
		} while(mines[row][col]);
		mines[row][col] = true;
	}
}
