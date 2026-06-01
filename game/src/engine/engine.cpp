#include <iostream>

#include "engine.h"

Engine::Engine(const std::string& path):
	engine_stdout(ctx),
	engine_stdin(ctx),
	engine(ctx, path, {}, boost::process::process_stdio{engine_stdin, engine_stdout})
{}

Engine::~Engine() {
	send("quit");
}

void Engine::send(const std::string& msg) {
	boost::system::error_code ec;

	boost::asio::write(engine_stdin, boost::asio::buffer(msg + "\n"), ec);

	if (ec) {
		std::cout << "Fail\n";
	}
}

bool Engine::getline(std::string& line) {
	while (true) {
		boost::asio::streambuf buffer;
		boost::system::error_code ec;

		boost::asio::read_until(engine_stdout, buffer, "\n", ec);

		if (ec) return false;

		std::istream is(&buffer);
		std::string retline;

		if (std::getline(is, retline)) {
			line = retline;
			return true;
		}
	}
}

void Engine::wait_for(const std::string& msg) {
	std::string line;

	while (getline(line)) {
		if (line == msg) break;
	}
}

void Engine::init() {
	send("u2i");
	wait_for("u2iok");
}