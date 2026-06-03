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
	static std::uint32_t move_map_left[MOVE_MAP_SIZE];
	static std::uint32_t move_map_right[MOVE_MAP_SIZE];
protected:
	std::uint32_t rows[4];
	std::uint32_t cols[4];
public:
	Board() {
		for (int i = 0; i < 4; i++) {
			rows[i] = 0;
			cols[i] = 0;
		}
	}

	Board(std::uint8_t board[4][4]) {
		for (int i = 0; i < 4; i++) {
			rows[i] = 0;
			for (int j = 0; j < 4; j++) {
				rows[i] |= board[i][j] << (5 * (3 - j));
			}
		}

		for (int j = 0; j < 4; j++) {
			cols[j] = 0;
			for (int i = 0; i < 4; i++) {
				cols[j] |= board[i][j] << (5 * (3 - i));
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

	void setColsFromRows() {
		cols[0] = 
			(rows[0] & 0xf8000) +
			((rows[1] & 0xf8000) >> 5) +
			((rows[2] & 0xf8000) >> 10) +
			((rows[3] & 0xf8000) >> 15);
		cols[1] =
			((rows[0] & 0x7c00) << 5) +
			(rows[1] & 0x7c00) +
			((rows[2] & 0x7c00) >> 5) +
			((rows[3] & 0x7c00) >> 10);
		cols[2] = 
			((rows[0] & 0x3e0) << 10) +
			((rows[1] & 0x3e0) << 5) +
			(rows[2] & 0x3e0) +
			((rows[3] & 0x3e0) >> 5);
		cols[3] = 
			((rows[0] & 0x1f) << 15) +
			((rows[1] & 0x1f) << 10) +
			((rows[2] & 0x1f) << 5) +
			(rows[3] & 0x1f);
	}

	void setRowsFromCols() {
		rows[0] = 
			(cols[0] & 0xf8000) +
			((cols[1] & 0xf8000) >> 5) +
			((cols[2] & 0xf8000) >> 10) +
			((cols[3] & 0xf8000) >> 15);
		rows[1] =
			((cols[0] & 0x7c00) << 5) +
			(cols[1] & 0x7c00) +
			((cols[2] & 0x7c00) >> 5) +
			((cols[3] & 0x7c00) >> 10);
		rows[2] = 
			((cols[0] & 0x3e0) << 10) +
			((cols[1] & 0x3e0) << 5) +
			(cols[2] & 0x3e0) +
			((cols[3] & 0x3e0) >> 5);
		rows[3] = 
			((cols[0] & 0x1f) << 15) +
			((cols[1] & 0x1f) << 10) +
			((cols[2] & 0x1f) << 5) +
			(cols[3] & 0x1f);
	}

	bool moveUp() {
		bool moved = false;

		for (int j = 0; j < 4; j++) {
			moved |= (move_map_left[cols[j]] != cols[j]);

			cols[j] = move_map_left[cols[j]];
		}

		setRowsFromCols();

		return moved;
	}

	bool moveDown() {
		bool moved = false;

		for (int j = 0; j < 4; j++) {
			moved |= (move_map_right[cols[j]] != cols[j]);

			cols[j] = move_map_right[cols[j]];
		}

		setRowsFromCols();

		return moved;
	}

	bool moveLeft() {
		bool moved = false;

		for (int i = 0; i < 4; i++) {
			moved |= (move_map_left[rows[i]] != rows[i]);

			rows[i] = move_map_left[rows[i]];
		}

		setColsFromRows();

		return moved;
	}

	bool moveRight() {
		bool moved = false;

		for (int i = 0; i < 4; i++) {
			moved |= (move_map_right[rows[i]] != rows[i]);

			rows[i] = move_map_right[rows[i]];
		}

		setColsFromRows();

		return moved;
	}

	bool operator==(const Board& rhs) const {
		for (int i = 0; i < 4; i++) {
			if (rows[i] != rhs.rows[i]) return false;
		}
		return true;
	}

	bool operator!=(const Board& rhs) const {
		return !(*this == rhs);
	}

	static void buildMoveMap();
};

#endif