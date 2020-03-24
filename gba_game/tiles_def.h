#ifndef TILES_DEF_H
#define TILES_DEF_H

enum class TilesIndexes: SCR_ENTRY {
	NONE        = 0,
	GRASS       = 1,
	FRAME,
	CLOSED_CELL,
	OPENED_CELL,
	FLAG,
	MINE,
	NUMBERS_BASE,
	TIMES = static_cast<SCR_ENTRY>(NUMBERS_BASE) + 10,
	RIGHT_ARROW,
};

#endif