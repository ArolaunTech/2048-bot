#include <cstdint>
#include <unordered_map>
#include <functional>
#include <cstdint>

#include "../engine.h"
#include "../../board/board.h"
#include "../../eval/ntuple.h"

#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

struct SearchResult {
	float score;
	std::uint8_t bestmove;
};

struct TTVal {
	float eval;
	std::uint8_t bestmove;
	int depth;
};

struct BoardHasher {
	std::size_t operator()(const Board& b) const {
		std::uint64_t v1 = 0;
		std::uint64_t v2 = 0;

		for (int r = 0; r < 2; r++) {
			for (int c = 0; c < 4; c++) {
				v1 <<= 8;
				v2 <<= 8;

				v1 += b.getCell(r, c);
				v2 += b.getCell(r + 2, c);
			}
		}

		std::hash<std::uint64_t> hasher;
		std::size_t h1 = hasher(v1);
		std::size_t h2 = hasher(v2);

		// boost algorithm
		std::size_t combined = h1 + 0x9e3779b9;
		combined ^= h2 + 0x9e3779b9 + (combined << 6) + (combined >> 2);

		/*for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				std::cout << static_cast<int>(b.getCell(r, c)) << " ";
			}
			std::cout << "\n";
		}
		std::cout << v1 << " " << v2 << " " << combined << "\n";*/

		return combined;
	}
};

class ExpectimaxEngine : public Engine {
private:
	std::unordered_map<Board, TTVal, BoardHasher> transpositiontable;
	NTupleEval evaluator;
public:
	float eval(const Board& b) const {
		/*if (b.isLoss()) return 0;

		float out = 10;

		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				if (b.getCell(r, c) == 0) out++;
			}
		}

		int max = 0;
		int maxloc = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				if (b.getCell(r, c) >= max) {
					max = b.getCell(r, c);
					maxloc = 4 * r + c;
				}
			}
		}

		const float maxlocadds[16] = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1};

		out += maxlocadds[maxloc];

		return out;*/

		/*for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				std::cout << (1 << static_cast<int>(b.getCell(r, c))) << "\t";
			}
			std::cout << "\n";
		}
		std::cout << evaluator.eval(b) << "\n";
		std::cout << evaluator.count_weights() << "\n";*/

		return evaluator.eval(b);
	}

	SearchResult search(const Board& b, int depth) {
		SearchResult out;

		if (depth == 0) {
			out.score = eval(b);
			return out;
		}

		if (transpositiontable.contains(b)) {
			TTVal result = transpositiontable[b];

			if (result.depth >= depth) {
				out.score = result.eval;
				out.bestmove = result.bestmove;
				return out;
			}
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

					// spawn 4
					copy.setCell(2, r, c);

					totalscore += 0.1 * search(copy, depth - 1).score;

					// spawn 2
					copy.setCell(1, r, c);

					totalscore += 0.9 * search(copy, depth - 1).score;

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

		Board copy = b;
		TTVal result;

		result.depth = depth;
		result.eval = maxscore;
		result.bestmove = bestmove;

		transpositiontable[copy] = result;

		return out;
	}

	std::uint8_t makeDecision(const Board& b) override {
		//size_t total_collisions = 0;
    
	    //for (size_t i = 0; i < transpositiontable.bucket_count(); i++) {
	    //    size_t b_size = transpositiontable.bucket_size(i);
	    //    if (b_size > 1) {
	    //        total_collisions += b_size - 1;
	    //    }
	    //}

		//std::cout << transpositiontable.size() << " " << total_collisions << "\n";

		//BoardHasher bhasher;
		//for (auto& it : transpositiontable) {
		//	std::cout << bhasher(it.first) << "bhash\n";
		//}

		const int depth = 3;

		int bsum = b.sum();
		std::erase_if(transpositiontable, [bsum, depth](const auto& item) {
			auto const& [key, value] = item;
			return ((key.sum() - bsum) / 2 + value.depth) < depth;
		});

		SearchResult out = search(b, depth);

		return out.bestmove;
	}
};

#endif