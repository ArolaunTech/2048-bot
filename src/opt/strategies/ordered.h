#include <cstdint>

#include "../engine.h"
#include "../../board/board.h"

#ifndef ORDERED_H
#define ORDERED_H

class OrderedEngine : public Engine {
private:
public:
	std::uint8_t makeDecision(const Board& b) override {
		Board copy = b;

		if (copy.moveUp()) return 0;
		if (copy.moveLeft()) return 2;
		if (copy.moveRight()) return 3;
		return 1;
	}
};

#endif