#include <iostream>
#include <string>

#include "board/board.h"
#include "opt/engine.h"
#include "test/test.h"

int main() {
	Board::buildMoveMap();

	conduct_board_test();
	conduct_board_speed_test();

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

	std::cout << game.to_string() << "\n";
}