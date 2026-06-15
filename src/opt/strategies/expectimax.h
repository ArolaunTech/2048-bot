#include <cstdint>

#include "../engine.h"
#include "../../board/board.h"

#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

struct SearchResult {
	float score;
	std::uint8_t bestmove;
};

class ExpectimaxEngine : public Engine {
private:
public:
	float eval(const Board& b) const {
		if (b.isLoss()) return 0;

		float out = 10;

		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				if (b.getCell(r, c) == 0) out++;
			}
		}

		return out;
	}

	SearchResult search(const Board& b, int depth) {
		SearchResult out;

		if (depth == 0) {
			out.score = eval(b);
			return out;
		}

		float maxscore = 0;
		std::uint8_t bestmove = 255;
		for (std::uint8_t i = 0; i < 4; i++) {
			// max
			Board copy = b;
			bool valid = copy.makeMove(i);

			if (!valid) continue;

			int totalempty = 0;
			float totalscore = 0;

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					if (copy.getCell(r, c) != 0) continue;

					totalempty++;

					// spawn 2
					copy.setCell(1, r, c);

					totalscore += 0.9 * search(copy, depth - 1).score;

					// spawn 4
					copy.setCell(2, r, c);

					totalscore += 0.1 * search(copy, depth - 1).score;

					// unspawn
					copy.setCell(0, r, c);
				}
			}

			float avgscore = totalscore / totalempty;

			if (bestmove == 255 || avgscore > maxscore) {
				maxscore = avgscore;
				bestmove = i;
			}
		}

		out.score = maxscore;
		out.bestmove = bestmove;

		return out;
	}

	std::uint8_t makeDecision(const Board& b) override {
		SearchResult out = search(b, 2);

		return out.bestmove;
	}
};

#endif