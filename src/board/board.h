#include <cstdint>
#include <iostream>

#ifndef BOARD_H
#define BOARD_H

const int MOVE_MAP_SIZE = 1 << 20;

class Board {
	/*
	 - Rows and columns are aligned with 2048 board
	 - Each std::uint8_t encodes a tile
	
	Values:
		 - 0: empty
		 - 1: 2^1 (2)
		 - 2: 2^2 (4)
		 ...
		 - 17: 2^17 (131072)

	There is no 5-bit integer type so I'm using an 8-bit one.
	*/
private:
	static std::uint8_t move_map[MOVE_MAP_SIZE][4];
	static bool moves[MOVE_MAP_SIZE];
protected:
	std::uint8_t cells[4][4];
public:
	Board() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				cells[i][j] = 0;
			}
		}
	}

	Board(std::uint8_t board[4][4]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				cells[i][j] = board[i][j];
			}
		}
	}

	void swap(std::uint8_t row[4], int i, int j) {
		if (row[i] == 0 && row[j] != 0) {
			row[i] = row[j];
			row[j] = 0;
		}
	}

	bool moveRowLeft(const std::uint8_t oldr[4], std::uint8_t newr[4]) {
		for (int i = 0; i < 4; i++) {
			newr[i] = oldr[i];
		}

		swap(newr, 2, 3);
		swap(newr, 1, 2);
		swap(newr, 0, 1);
		swap(newr, 2, 3);
		swap(newr, 1, 2);
		swap(newr, 2, 3);

		for (int i = 0; i < 3; i++) {
			if (newr[i] == newr[i + 1] && newr[i] > 0) {
				newr[i]++;
				newr[i + 1] = 0;
			}
		}

		swap(newr, 2, 3);
		swap(newr, 1, 2);
		swap(newr, 0, 1);
		swap(newr, 2, 3);
		swap(newr, 1, 2);
		swap(newr, 2, 3);

		return !compare(oldr, newr);
	}

	bool compare(const std::uint8_t oldr[4], const std::uint8_t newr[4]) {
		for (int i = 0; i < 4; i++) {
			if (oldr[i] != newr[i]) return false;
		}

		return true;
	}

	bool moveUp() {
		bool moved = false;

		for (int j = 0; j < 4; j++) {
			std::uint32_t row = 0;

			for (int i = 0; i < 4; i++) {
				row |= cells[i][j] << (5 * (3 - i));
			}

			moved |= moves[row];

			for (int i = 0; i < 4; i++) {
				cells[i][j] = move_map[row][i];
			}
		}

		return moved;
	}

	bool moveDown() {
		bool moved = false;

		for (int j = 0; j < 4; j++) {
			std::uint32_t row = 0;

			for (int i = 0; i < 4; i++) {
				row |= cells[i][j] << (5 * i);
			}

			moved |= moves[row];

			for (int i = 0; i < 4; i++) {
				cells[i][j] = move_map[row][3-i];
			}
		}

		return moved;
	}

	bool moveLeft() {
		bool moved = false;

		for (int i = 0; i < 4; i++) {
			std::uint32_t row = 0;

			for (int j = 0; j < 4; j++) {
				row |= cells[i][j] << (5 * (3 - j));
			}

			moved |= moves[row];

			for (int j = 0; j < 4; j++) {
				cells[i][j] = move_map[row][j];
			}
		}

		return moved;
	}

	bool moveRight() {
		bool moved = false;

		for (int i = 0; i < 4; i++) {
			std::uint32_t row = 0;

			for (int j = 0; j < 4; j++) {
				row |= cells[i][j] << (5 * j);
			}

			moved |= moves[row];

			for (int j = 0; j < 4; j++) {
				cells[i][j] = move_map[row][3-j];
			}
		}

		return moved;
	}

	bool operator==(const Board& rhs) const {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (cells[i][j] != rhs.cells[i][j]) return false;
			}
		}
		return true;
	}

	bool operator!=(const Board& rhs) const {
		return !(*this == rhs);
	}

	static void buildMoveMap();
};

#endif