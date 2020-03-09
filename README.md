# GBA Minesweeper

I just like programming for the GameBoy Advance and I find Minesweeper to be a pretty entertaining game, still remaining fairly simple.

## Environment setup

You need the [devkitPro](https://devkitpro.org/) toolchain to compile, and other tools included (like [Grit](https://www.coranac.com/man/grit/html/grit.htm)).

### Linux

You just need to install `devkitPro` and `make`. For the first follow [this wiki page](https://devkitpro.org/wiki/devkitPro_pacman), for the latter just use your package manager.

### Windows

:man_shrugging:

## Compilation

Just run `make` in the root folder of the repository. If everything goes fine a file called `minesweeper.gba` will be produced.  
To clean all the compilation files (including the game ROM) you can run `make clean`.

Now you can just run the game with yout favourite emulator!

## Usage

* Use arrow keys to move the cursor
* **A** key to open the cell under the cursor
* **B** to add/remove the flag from the cell under the cursor

After finishing the game the position of the mines will be showed. To restart you just need to press **A**.

# PC version

I wrote a simple console version of the game. I did that to help separate the actual game logic from the "user interface".

## Compiling & Running

To compile the PC version run `make -f Makefile.pc`. Running `make` will compile the GBA version.
Then just run `./pc`.

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
