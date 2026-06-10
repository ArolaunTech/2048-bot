#include <iostream>
#include <string>
#include <cstdint>

#include "board/board.h"
#include "opt/engine.h"
#include "test/test.h"

int main() {
	Board::buildMoveMap();

	conduct_board_test();
	conduct_board_speed_test();

	int freqs[18];
	for (int i = 0; i < 18; i++) {
		freqs[i] = 0;
	}

	for (int it = 0; it < 100000; it++) {
		Board game;
		game.spawn();
		game.spawn();

		Engine e;

		while (!game.isLoss()) {
			std::uint8_t move = e.makeDecision(game);

			if (!game.makeMove(move)) {
				std::cout << "invalid move!\n";
				break;
			}

			game.spawn();
		}

		std::uint8_t best = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (game.getCell(i, j) > best) best = game.getCell(i, j);
			}
		}

		freqs[best]++;
	}

	for (int i = 0; i < 18; i++) {
		std::cout << freqs[i] << " ";
	}
	std::cout << "\n";
}