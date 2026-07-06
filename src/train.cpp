#include <iostream>
#include <string>
#include <limits>

#include <CLI/CLI.hpp>

#include "eval/ntuple.h"
#include "board/board.h"
#include "test/test.h"

int main(const int argc, const char *const *const argv) {
	/*========== Argument parsing ==========*/
	CLI::App app;

	std::string inputpath = "";
	std::string outputpath;

	int Ngames;

	app.add_option(
		"-i,--input", 
		inputpath, 
		"Initial network for training"
	)->check(CLI::ExistingFile);

	app.add_option(
		"-o,--output",
		outputpath,
		"Output for network, will overwrite existing file if exists"
	)->check(CLI::NonexistentPath | CLI::ExistingFile)->required();

	app.add_option(
		"-g,--games",
		Ngames,
		"Number of games to play"
	)->check(CLI::NonNegativeNumber)->required();

	CLI11_PARSE(app, argc, argv);

	/*========== Initialization (not the network) ==========*/
	std::cout << "Building board...\n";

	Board::buildMoveMap();

	conduct_board_test();

	std::cout << "Success! Board is built correctly!\n";

	/*========== Initialize network ==========*/
	NTupleEval evaluator;

	if (inputpath == "") {
		// Default network settings
		// Taken from Multistage Temporal Difference Learning for 2048-Like Games by Yeh et al.
		evaluator.add_tuple(std::vector<int>{0, 4, 8, 9, 12, 13});
		evaluator.add_tuple(std::vector<int>{1, 5, 9, 10, 13, 14});
		evaluator.add_tuple(std::vector<int>{1, 2, 5, 6, 9, 10});
		evaluator.add_tuple(std::vector<int>{2, 3, 6, 7, 10, 11});
	} else {
		evaluator.load_from_file(inputpath);
	}

	std::cout << "\nNetwork has " << evaluator.count_weights() << " weights.\n";

	/*========== Training network ==========*/
	std::cout << "\nTraining network for " << Ngames << " game(s)...\n";

	const float learning_rate = 0.01;
	const int aggregate = 10000;

	int aggregatesum = 0;

	for (int i = 0; i < Ngames; i++) {
		// play game
		Board game;
		game.spawn();
		game.spawn();

		while (!game.isLoss()) {
			// Make move
			int bestmove = 0;
			float bestscore = -std::numeric_limits<float>::infinity();

			for (int j = 0; j < 4; j++) {
				Board copy = game;

				if (!copy.makeMove(j)) continue;

				int nempty = 0;
				float totalscore = 0;

				for (int r = 0; r < 4; r++) {
					for (int c = 0; c < 4; c++) {
						if (copy.getCell(r, c) != 0) continue;

						nempty++;

						copy.setCell(2, r, c);
						totalscore += 0.1 * evaluator.eval(copy);

						copy.setCell(1, r, c);
						totalscore += 0.9 * evaluator.eval(copy);

						copy.setCell(0, r, c);
					}
				}

				float movescore = totalscore / nempty;

				if (movescore > bestscore) {
					bestscore = movescore;
					bestmove = j;
				}
			}

			evaluator.train(game, bestscore, learning_rate);

			game.makeMove(bestmove);

			// Spawn
			game.spawn();
		}

		aggregatesum += game.sum();

		if (i % aggregate == aggregate - 1) {
			std::cout << static_cast<float>(aggregatesum) / aggregate << " " << i + 1 << "\n\n";
			aggregatesum = 0;

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					std::cout << (1 << static_cast<int>(game.getCell(r, c))) << "\t";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	}
	
	/*========== Output to file ==========*/
	std::cout << "\nSaving to file...\n";

	evaluator.save_to_file(outputpath);
	
	std::cout << "Success!\n";
}