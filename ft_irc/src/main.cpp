#include "../inc/Server.hpp"

static Server* g_server = NULL;

static void handleSignal(int signum) {
	if (g_server) {
		g_server->signalHandler(signum);
	}
}

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return 1;
	}

	int port = std::atoi(argv[1]);
	if (invalidPort(port)) return 1;

	try {

		Server serv(port, argv[2]);
		g_server = &serv;
		std::signal(SIGINT, handleSignal);
		std::signal(SIGTERM, handleSignal);
		//std::signal(SIGQUIT, handleSignal);
		serv.run();

	} catch (std::exception& e) {
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}

