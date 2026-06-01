#include <iostream>
#include <cstdint>
#include <cassert>
#include <chrono>
#include <array>
#include <vector>
#include <random>

#include "test.h"
#include "../board/board.h"

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<std::uint8_t> distribution(0, 17);

void conduct_board_test() {
	// Stolen from https://github.com/macroxue/2048-ai/blob/master/board_test.cc
	std::uint8_t layout[4][4] = {
		{ 0, 0, 0, 1 },
		{ 0, 2, 0, 2 },
		{ 2, 2, 2, 2 },
		{ 3, 1, 1, 1 },
	};

	std::uint8_t moved_left[4][4] = {
    	{ 1, 0, 0, 0 },
    	{ 3, 0, 0, 0 },
    	{ 3, 3, 0, 0 },
    	{ 3, 2, 1, 0 },
  	};
  	std::uint8_t moved_right[4][4] = {
    	{ 0, 0, 0, 1 },
    	{ 0, 0, 0, 3 },
    	{ 0, 0, 3, 3 },
    	{ 0, 3, 1, 2 },
	};
	std::uint8_t moved_up[4][4] = {
    	{ 2, 3, 2, 1 },
    	{ 3, 1, 1, 3 },
    	{ 0, 0, 0, 1 },
    	{ 0, 0, 0, 0 },
	};
	std::uint8_t moved_down[4][4] = {
    	{ 0, 0, 0, 0 },
    	{ 0, 0, 0, 1 },
    	{ 2, 3, 2, 3 },
    	{ 3, 1, 1, 1 },
	};

  	Board left(layout);

  	assert(left.moveLeft());
  	assert(left == Board(moved_left));

  	Board right(layout);
  	assert(right.moveRight());
  	assert(right == Board(moved_right));

  	Board up(layout);
  	assert(up.moveUp());
  	assert(up == Board(moved_up));

  	Board down(layout);
  	assert(down.moveDown());
  	assert(down == Board(moved_down));

	std::uint8_t layout2[4][4] = {
		{ 0, 0, 0, 1 },
		{ 0, 0, 3, 2 },
		{ 2, 3, 4, 5 },
		{ 3, 4, 5, 6 },
	};
	Board down2(layout2);
	assert(!down2.moveDown());
	assert(down2 == Board(layout2));

	Board right2(layout2);
	assert(!right2.moveRight());
	assert(right2 == Board(layout2));

	Board up2(layout2);
	assert(up2.moveUp());

	Board left2(layout2);
	assert(left2.moveLeft());

	std::uint8_t layout3[4][4] = {
		{ 3, 4, 5, 6 },
		{ 2, 3, 4, 5 },
		{ 1, 2, 0, 0 },
		{ 3, 0, 0, 0 },
	};
	Board up3(layout3);
	assert(!up3.moveUp());
	assert(up3 == Board(layout3));

	Board left3(layout3);
	assert(!left3.moveLeft());
	assert(left3 == Board(layout3));

	Board right3(layout3);
	assert(right3.moveRight());

	Board down3(layout3);
	assert(down3.moveDown());

	std::cout << "Tests passed!\n";
}

void conduct_board_speed_test() {
	const int N = 1000000;

	std::vector<Board> boards;

	for (int i = 0; i < N; i++) {

		std::uint8_t cells[4][4];
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				cells[r][c] = distribution(generator);
			}
		}

		boards.push_back(Board(cells));
	}

	int sum = 0;

	auto t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++) {
		if (boards[i].moveUp()) sum++;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = t2 - t1;

	std::cout << N / duration.count() << "\n";
	std::cout << sum << "\n";
}