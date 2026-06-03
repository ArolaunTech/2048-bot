#include "board.h"

std::uint32_t Board::move_map_left[MOVE_MAP_SIZE];
std::uint32_t Board::move_map_right[MOVE_MAP_SIZE];

void Board::buildMoveMap() {
	Board b;
	for (int i = 0; i < MOVE_MAP_SIZE; i++) {
		std::uint8_t oldrow[4], newrow[4];

		oldrow[0] = (i >> 15) & 0x1f;
		oldrow[1] = (i >> 10) & 0x1f;
		oldrow[2] = (i >> 5) & 0x1f;
		oldrow[3] = i & 0x1f;

		b.moveRowLeft(oldrow, newrow);

		Board::move_map_left[i] = 
			(newrow[0] << 15) + 
			(newrow[1] << 10) +
			(newrow[2] << 5) +
			newrow[3];

		Board::move_map_right[
			(oldrow[3] << 15) +
			(oldrow[2] << 10) +
			(oldrow[1] << 5) +
			oldrow[0]
		] = 
			(newrow[3] << 15) +
			(newrow[2] << 10) +
			(newrow[1] << 5) +
			newrow[0];
	}
}