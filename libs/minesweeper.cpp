#include <queue>

#include <minesweeper>
#include <random_helpers>

Minesweeper::Minesweeper(const int gridSize):
	cells(gridSize, std::vector(gridSize, static_cast<cell_type>(CellState::CLOSED))),
	mines(gridSize, std::vector(gridSize, false)) {}

int Minesweeper::columns() const {
	return cells.size() > 0 ? cells[0].size() : 0;
}

int Minesweeper::rows() const {
	return cells.size();
}

CellState Minesweeper::at(const int row, const int col) const {
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

bool Minesweeper::is_mine(const int row, const int col) const {
	if (row >= rows() || col >= columns())
		return false;
	return mines[row][col];
}

bool Minesweeper::is_mine(const position_type pos) const {
	return is_mine(pos.row, pos.col);
}

int Minesweeper::neighbours(const int row, const int col) const {
	return at(row, col) == CellState::OPENED ? cells[row][col] : 0;
}

int Minesweeper::neighbours(const position_type pos) const {
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
int Minesweeper::count_neighbours(const position_type pos) const {
	int count = 0;
	for(int v = -1; v <= 1; v++) {
		for(int h = -1; h <= 1; h++) {
			int r = pos.row + v;
			int c = pos.col + h;
			if (r >= 0 && c >= 0 && r < rows() && c < columns()) {
				count += mines[r][c];
			}
		}
	}
	return count;
}

void Minesweeper::open_cell(const position_type start) {
	if (mines[start.row][start.col]) {
		lost = true;
	} else {
		std::queue<position_type> toOpen; // Cells to be opened
		toOpen.push(start);
		while(!toOpen.empty()) {
			auto curr = toOpen.front();
			toOpen.pop();
			if (at(curr) == CellState::OPENED)
				continue;
			const auto minesNear = count_neighbours(curr);
			cells[curr.row][curr.col] = minesNear;
			openedCells++;
			if (cellopenedCallback != nullptr) (*cellopenedCallback)(*this, curr);
			// If there are no mines near me open the neighbours
			if (minesNear == 0) {
				for (int v = -1; v <= 1; v++) {
					for(int h = -1; h <= 1; h++) {
						int r = curr.row + v;
						int c = curr.col + h;
						if ((h != 0 || v != 0) && r >= 0 && c >= 0 && r < rows() && c < columns() && at(r, c) != CellState::OPENED) {
							toOpen.push({r, c});
						}
					}
				}
			}
		}
	}
}

void Minesweeper::initialize_mines() {
	minesCount = rows() * columns() / 8;

	RandomSize random;
	for(int i = 0; i < minesCount; i++) {
		position_type pos;
		do {
			pos.row = random(rows()-1);
			pos.col = random(columns()-1);
		} while(mines[pos.row][pos.col] || cursor() == pos);
		mines[pos.row][pos.col] = true;
	}
}
