#include "board.h"

std::uint8_t Board::move_map[1 << 20][4];
bool Board::moves[MOVE_MAP_SIZE];

void Board::buildMoveMap() {
	Board b;
	for (int i = 0; i < MOVE_MAP_SIZE; i++) {
		std::uint8_t oldrow[4];

		oldrow[0] = (i >> 15) & 0x1f;
		oldrow[1] = (i >> 10) & 0x1f;
		oldrow[2] = (i >> 5) & 0x1f;
		oldrow[3] = i & 0x1f;

		Board::moves[i] = b.moveRowLeft(oldrow, Board::move_map[i]);
	}
}