# GBA Minesweeper

I just like programming for the GameBoy Advance and I find Minesweeper to be a pretty entertaining game, still remaining fairly simple.

# PC version
I wrote a simple console version of the game. I did that to help separate the actual game logic from the "user interface".

## Running
Just run `make`, then run `./pc`.

## Usage
Type a command and press `Enter ↵`. Commands are

* `w`,`a`,`s`,`d` to move the cursor
* `f` to toggle flag
* `o` to open the cell

Cell symbols meanings:

* `@` is the cursor
* `H` is a closed cell
* `!` is a (closed) flagged cell
* ` ` is an opened cell
* a cell containing a number indicates the number of bombs in continguous cells
* `*` is a bomb (only show at the end)