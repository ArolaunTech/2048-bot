#include <iostream>
#include <string>

int main() {
	std::string line;

	while (std::getline(std::cin, line)) {
		if (line == "u2i") {
			std::cout << "u2iok\n";
			continue;
		}
		if (line == "quit") {
			return 0;
		}

		std::cout << line << "\n";
	}
}