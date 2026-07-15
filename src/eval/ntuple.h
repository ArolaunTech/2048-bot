#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <bit>
#include <iostream>

#include "../board/board.h"
#include "../utils/maths.h"

#ifndef NTUPLE_H
#define NTUPLE_H

class NTupleNet {
private:
	std::vector<std::vector<float> > weights;
	std::vector<std::vector<int> > tuples;

	static const int NTILES = 18;
public:
	NTupleNet() {
		weights.clear();
		tuples.clear();
	}

	std::size_t count_weights() const {
		std::size_t out = 0;

		for (std::size_t i = 0; i < weights.size(); i++) {
			out += weights[i].size();
		}

		return out;
	}

	float eval(const Board& b) const {
		if (b.isLoss()) return b.sum();

		float out = 0;

		for (std::size_t i = 0; i < tuples.size(); i++) {
			int idxs[8];

			for (int j = 0; j < 8; j++) {
				idxs[j] = 0;
			}

			for (std::size_t j = 0; j < tuples[i].size(); j++) {
				for (int k = 0; k < 8; k++) {
					idxs[k] *= NTILES;
				}

				int r = tuples[i][j] / 4;
				int c = tuples[i][j] % 4;

				idxs[0] += b.getCell(r, c);
				idxs[1] += b.getCell(3-r, c);
				idxs[2] += b.getCell(r, 3-c);
				idxs[3] += b.getCell(3-r, 3-c);
				idxs[4] += b.getCell(c, r);
				idxs[5] += b.getCell(3-c, r);
				idxs[6] += b.getCell(c, 3-r);
				idxs[7] += b.getCell(3-c, 3-r);
			}

			for (int j = 0; j < 8; j++) {
				out += weights[i][idxs[j]];
			}
		}

		return out;
	}

	void train(const Board& b, float target, float learning_rate) {
		float currout = eval(b);

		float error = target - currout;

		float d = error * learning_rate;

		for (std::size_t i = 0; i < tuples.size(); i++) {
			int idxs[8];

			for (int j = 0; j < 8; j++) {
				idxs[j] = 0;
			}

			for (std::size_t j = 0; j < tuples[i].size(); j++) {
				for (int k = 0; k < 8; k++) {
					idxs[k] *= NTILES;
				}

				int r = tuples[i][j] / 4;
				int c = tuples[i][j] % 4;

				idxs[0] += b.getCell(r, c);
				idxs[1] += b.getCell(3-r, c);
				idxs[2] += b.getCell(r, 3-c);
				idxs[3] += b.getCell(3-r, 3-c);
				idxs[4] += b.getCell(c, r);
				idxs[5] += b.getCell(3-c, r);
				idxs[6] += b.getCell(c, 3-r);
				idxs[7] += b.getCell(3-c, 3-r);
			}

			for (int j = 0; j < 8; j++) {
				weights[i][idxs[j]] += d;
			}
		}
	}

	void load_from_file(const std::string& path) {
		// open file
		std::ifstream out;
		out.open(path, std::ios::binary | std::ios::in);

		if (!out.is_open()) {
			throw std::runtime_error("Could not open file.");
		}

		// reset
		tuples.clear();
		weights.clear();

		// vars
		char data[4];

		// read
		std::uint8_t ntuples;
		out.read(data, 1);

		ntuples = static_cast<std::uint8_t>(data[0]);

		for (std::uint8_t i = 0; i < ntuples; i++) {
			out.read(data, 1);

			std::uint8_t tuplesize = static_cast<std::uint8_t>(data[0]);
			std::vector<int> tuple;

			for (std::uint8_t j = 0; j < tuplesize; j++) {
				out.read(data, 1);
				tuple.push_back(static_cast<int>(static_cast<unsigned char>(data[0])));
			}

			tuples.push_back(tuple);
		}

		for (std::size_t i = 0; i < tuples.size(); i++) {
			for (std::size_t j = 0; j < tuples[i].size(); j++) {
				out.read(data, 4); // dumping this is techinically wrong but whatever
			}

			out.read(data, 4);

			std::uint32_t nweights = 
				(static_cast<std::uint32_t>(static_cast<unsigned char>(data[0])) << 0) +
				(static_cast<std::uint32_t>(static_cast<unsigned char>(data[1])) << 8) +
				(static_cast<std::uint32_t>(static_cast<unsigned char>(data[2])) << 16) +
				(static_cast<std::uint32_t>(static_cast<unsigned char>(data[3])) << 24);

			out.read(data, 1);

			std::uint8_t floatsize = static_cast<std::uint8_t>(data[0]);

			std::vector<float> weightset;
			for (std::uint32_t j = 0; j < nweights; j++) {
				out.read(data, floatsize);

				weightset.push_back(std::bit_cast<float>(data));
			}

			weights.push_back(weightset);
		}

		// close
		out.close();
	}

	void save_to_file(const std::string& path) const {
		std::ofstream out;
		out.open(path, std::ios::binary | std::ios::out);

		if (!out.is_open()) {
			throw std::runtime_error("Could not open file.");
		}

		// Tuples
		out << static_cast<char>(tuples.size());

		for (std::size_t i = 0; i < tuples.size(); i++) {
			out << static_cast<char>(tuples[i].size());

			for (std::size_t j = 0; j < tuples[i].size(); j++) {
				out << static_cast<char>(tuples[i][j]);
			}
		}

		// Weights
		for (std::size_t i = 0; i < tuples.size(); i++) {
			for (std::size_t j = 0; j < tuples[i].size(); j++) {
				out << static_cast<char>((NTILES >> 0) & 0xff);
				out << static_cast<char>((NTILES >> 8) & 0xff);
				out << static_cast<char>((NTILES >> 16) & 0xff);
				out << static_cast<char>((NTILES >> 24) & 0xff);
			}

			out << static_cast<char>((weights[i].size() >> 0) & 0xff);
			out << static_cast<char>((weights[i].size() >> 8) & 0xff);
			out << static_cast<char>((weights[i].size() >> 16) & 0xff);
			out << static_cast<char>((weights[i].size() >> 24) & 0xff);

			out << static_cast<char>(sizeof(float));

			for (std::size_t j = 0; j < weights[i].size(); j++) {
				// Put weights[i][j] into out, 4 bytes
				float weight = weights[i][j];
				out.write(reinterpret_cast<const char*>(&weight), sizeof(weight));
			}
		}

		out.close();
	}

	void add_tuple(const std::vector<int>& newtuple) {
		std::size_t tuplesize = newtuple.size();
		int nweights = pown(NTILES, tuplesize);

		std::vector<int> newtuplecopy = newtuple;

		tuples.push_back(newtuplecopy);
		weights.push_back(std::vector<float>(nweights, 1e4));
	}
};

class NTupleEval {
private:
	std::vector<NTupleNet> evaluators;
	std::vector<int> thresholds;
public:
	NTupleEval() {
		evaluators.clear();
		thresholds.clear();

		add_evaluator();
	}

	std::size_t count_weights() const {
		std::size_t out = 0;

		for (std::size_t i = 0; i < evaluators.size(); i++) {
			out += evaluators[i].count_weights();
		}

		return out;
	}

	float eval(const Board& b) const {
		int sum = b.sum();

		if (b.isLoss()) return sum;

		for (std::size_t i = 0; i < thresholds.size(); i++) {
			if (thresholds[i] > sum) {
				return evaluators[i].eval(b);
			}
		}
		return evaluators[evaluators.size() - 1].eval(b);
	}

	void train(const Board& b, float target, float learning_rate) {
		int sum = b.sum();

		for (std::size_t i = 0; i < thresholds.size(); i++) {
			if (thresholds[i] > sum) {
				evaluators[i].train(b, target, learning_rate);
				return;
			}
		}
		evaluators[evaluators.size() - 1].train(b, target, learning_rate);
	}

	void load_from_file(const std::string& path) {
		std::ifstream in;
		in.open(path, std::ios::in);

		if (!in.is_open()) {
			throw std::runtime_error("Could not open file.");
		}

		evaluators.clear();
		thresholds.clear();

		std::string line;
		int i = 0;

		while (std::getline(in, line)) {
			if (i % 2 == 0) {
				add_evaluator();

				evaluators[i / 2].load_from_file(line);
			} else {
				thresholds.push_back(std::stoi(line));
			}

			i++;
		}
	}

	void save_to_file(const std::string& path) const {
		std::ifstream in;
		in.open(path, std::ios::in);

		if (!in.is_open()) {
			throw std::runtime_error("Could not open file.");
		}

		std::string line;
		int i = 0;

		while (std::getline(in, line)) {
			if (i % 2 == 0) {
				evaluators[i / 2].save_to_file(line);
			}

			i++;
		}
	}

	void add_evaluator() {
		NTupleNet n;
		evaluators.push_back(n);
	}

	void add_threshold(int nthreshold) {
		thresholds.push_back(nthreshold);
	}

	void add_tuple(int idx, const std::vector<int>& newtuple) {
		evaluators[idx].add_tuple(newtuple);
	}
};

#endif