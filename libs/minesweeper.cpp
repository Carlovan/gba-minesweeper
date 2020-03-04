#include <minesweeper>
#include <random_helpers>

Minesweeper::Minesweeper(const size_t gridSize):
	cells(gridSize, std::vector(gridSize, static_cast<cell_type>(CellState::CLOSED))),
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
	if (cells[row][col] >= 0) {
		return CellState::OPENED;
	}
	return static_cast<CellState>(cells[row][col]);
}

CellState Minesweeper::at(const position_type pos) const {
	return at(pos.row, pos.col);
}

bool Minesweeper::is_mine(const size_t row, const size_t col) const {
	if (row >= rows() || col >= columns())
		return false;
	return mines[row][col];
}

bool Minesweeper::is_mine(const position_type pos) const {
	return is_mine(pos.row, pos.col);
}

size_t Minesweeper::neighbours(const size_t row, const size_t col) const {
	return at(row, col) == CellState::OPENED ? cells[row][col] : 0;
}

size_t Minesweeper::neighbours(const position_type pos) const {
	return neighbours(pos.row, pos.col);
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

void Minesweeper::setCellOpenedCallback(cellopened_callback_type_ptr callback) {
	cellopenedCallback = std::move(callback);
}

void Minesweeper::removeCellOpenedCallback() {
	setCellOpenedCallback(nullptr);
}

// Move the cursor vertically; down if dir > 0, up if dir < 0, nothing if dir == 0
void Minesweeper::movev(int dir) {
	if (dir > 0)
		down();
	else if (dir < 0)
		up();
}

// Same as movev but moves horizontally; to right if dir > 0
void Minesweeper::moveh(int dir) {
	if (dir > 0)
		right();
	else if (dir < 0)
		left();
}

void Minesweeper::up() {
	if (cursorPosition.row > 0)
		cursorPosition.row--;
}

void Minesweeper::right() {
	if (cursorPosition.col < columns()-1)
		cursorPosition.col++;
}

void Minesweeper::down() {
	if (cursorPosition.row < rows()-1)
		cursorPosition.row++;
}

void Minesweeper::left() {
	if (cursorPosition.col > 0)
		cursorPosition.col--;
}

void Minesweeper::toggle_flag() {
	if (at(cursor()) != CellState::OPENED) {
		cells[cursor().row][cursor().col] = static_cast<cell_type>(at(cursor()) == CellState::CLOSED ? CellState::FLAGGED : CellState::CLOSED);
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
			size_t r = pos.row + v;
			size_t c = pos.col + h;
			if (r < rows() && c < columns()) {
				count += mines[r][c];
			}
		}
	}
	return count;
}

void Minesweeper::open_cell(const position_type pos) {
	if (mines[pos.row][pos.col]) {
		lost = true;
	} else if (at(pos) != CellState::OPENED) {
		const auto minesNear = count_neighbours(pos);
		cells[pos.row][pos.col] = minesNear;
		openedCells++;
		if (cellopenedCallback != nullptr) (*cellopenedCallback)(*this, pos);
		// If there are no mines near me open the neighbours
		if (minesNear == 0) {
			for (int v = -1; v <= 1; v++) {
				for(int h = -1; h <= 1; h++) {
					size_t r = pos.row + v;
					size_t c = pos.col + h;
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
		position_type pos;
		do {
			pos.row = random(rows()-1);
			pos.col = random(columns()-1);
		} while(mines[pos.row][pos.col] || cursor() == pos);
		mines[pos.row][pos.col] = true;
	}
}
