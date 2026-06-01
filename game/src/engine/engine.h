#include <boost/process.hpp>
#include <boost/asio.hpp>

#include <string>

#ifndef ENGINE_H
#define ENGINE_H

class Engine {
private:
	boost::asio::io_context ctx;
	boost::asio::readable_pipe engine_stdout;
	boost::asio::writable_pipe engine_stdin;

	boost::process::process engine;
public:
	Engine(const std::string& path);
	~Engine();

	void send(const std::string& msg);
	void wait_for(const std::string& msg);
	bool getline(std::string& line);

	void init();
};

#endif