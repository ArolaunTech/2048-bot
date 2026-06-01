#include <iostream>
#include <string>

#include "board/board.h"
#include "opt/engine.h"
#include "test/test.h"

int main() {
	Board::buildMoveMap();

	conduct_board_test();
	conduct_board_speed_test();
}