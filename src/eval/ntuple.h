#include <vector>
#include <fstream>
#include <string>

#include "../board/board.h"

#ifndef NTUPLE_H
#define NTUPLE_H

class NTupleEval {
private:
	static std::vector<std::vector<float> > weights;
	static std::vector<std::vector<int> > tuples;
public:
	float eval(const Board& b) {
		return 0;
	}

	void load_from_file(const std::string& path) {

	}

	void save_to_file(const std::string& path) {
		
	}
};

#endif