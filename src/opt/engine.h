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

	virtual std::uint8_t makeDecision(const Board& b) const = 0;
};

#endif