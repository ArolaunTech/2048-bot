#include "../board/board.h"

#include <limits>
#include <cstdint>

#ifndef ENGINE_H
#define ENGINE_H

struct SearchResult {
	float eval;
	std::uint8_t bestmove;
};

class Engine {
private:
public:
	Engine() {
		
	}

	float eval(const Board& b) {
		return 0;
	}

	SearchResult search(const Board& b, int depth) {
		SearchResult out;

		if (b.isLoss()) {
			out.eval = -std::numeric_limits<float>::infinity();
			return out;
		}

		if (depth == 0) {
			out.eval = eval(b);
			return out;
		}

		for (std::uint8_t movenum = 0; movenum < 4; movenum++) {
			Board copy = b;

			copy.makeMove(movenum);


		}

		return out;
	}

	std::uint8_t makeDecision(const Board& b) {
		Board copy = b;

		if (copy.moveUp()) return 0;
		if (copy.moveLeft()) return 2;
		if (copy.moveRight()) return 3;
		return 1;
	}
};

#endif