#include <CLI/CLI.hpp>

#include <iostream>
#include <cstdio>
#include <string>

#include "engine/engine.h"

int main(int argc, char** argv) {
	// Read command-line arguments
	CLI::App app;
	argv = app.ensure_utf8(argv);

	std::string enginepath;
	int ngames{1000};
	int mspt{1000};

	app.add_option("-e,--engine", enginepath, "Path to engine executable")->required()->check(CLI::ExistingFile);
	app.add_option("-g,--games", ngames, "Number of games to play")->check(CLI::PositiveNumber);
	app.add_option("-t,--thinkingtime", mspt, "Thinking time per turn in milliseconds")->check(CLI::PositiveNumber);

	CLI11_PARSE(app, argc, argv);
	//===========================

	// Disable buffering
	std::setbuf(stdin, nullptr);
	std::setbuf(stdout, nullptr);
	//===========================

	// Initialize engine
	Engine engine(enginepath);
	//===========================

	// Print about
	std::cout << "2048 tester\n";
	std::cout << "-----------\n";
	std::cout << "\n";
	std::cout << "Testing engine: " << enginepath << "\n";
	std::cout << "Number of games: " << ngames << "\n";
	std::cout << "Milliseconds per turn: " << mspt << "\n";
	//===========================

	// Play games
	engine.init();
	//===========================
}