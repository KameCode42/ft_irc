#include "Server.hpp"

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return 1;
	}

	int port = std::atoi(argv[1]);
	if (invalidPort(port)) return 1;

	try {

		Server serv(port, argv[2]);
		serv.run();

	} catch (std::exception& e) {
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
