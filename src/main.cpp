#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>

#include "board/board.h"
#include "opt/engine.h"
#include "opt/strategies/ordered.h"
#include "test/test.h"

int main() {
	Board::buildMoveMap();

	conduct_board_test();
	conduct_board_speed_test();

	int freqs[18];
	for (int i = 0; i < 18; i++) {
		freqs[i] = 0;
	}

	const double TIME_LIMIT = 10;

	const auto start = std::chrono::high_resolution_clock::now();

	int it = 0;
	while (true) {
		Board game;
		game.spawn();
		game.spawn();

		OrderedEngine e;

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


		for (int i = 0; i <= best; i++) {
			freqs[i]++;
		}

		it++;

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		if (duration.count() > static_cast<int>(1000000 * TIME_LIMIT)) break;
	}

	std::cout << "Performed " << it << " tests\n\n";
	for (int i = 0; i < 18; i++) {
		std::cout << (1 << i) << ": " << (double)freqs[i]/it * 100 << " (" << freqs[i] << " passed)\n";
	}
}