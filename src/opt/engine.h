#include "../board/board.h"

#include <limits>
#include <cstdint>

#ifndef ENGINE_H
#define ENGINE_H

class Engine {
private:
public:
	virtual std::uint8_t makeDecision(const Board& b) = 0;
};

#endif