#include "Server.hpp"
#include "Client.hpp"
#include "Commands.hpp"

// === CONSTRUCTOR & DESTRUCTOR ===
Server::Server(int port, const std::string& pass) :
	_port(port),
	_password(pass),
	_sockfd(-1),
	_running(true) {

	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd == -1) {
		close(this->_sockfd);
		throw std::runtime_error("Socket creation failed");
	}

	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) == -1) {
		close(this->_sockfd);
		throw std::runtime_error("fcntl failed");
	}

	int opt = 1;
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		close(this->_sockfd);
		throw std::runtime_error("setsockopt failed");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		close(this->_sockfd);
		throw std::runtime_error("Bind failed");
	}

	if (listen(this->_sockfd, 5) == -1) {
		close(this->_sockfd);
		throw std::runtime_error("Listen failed");
	}
}

Server::~Server() {
	closeAllConnections();
}


// === GETTERS & SETTERS ===
const std::string& Server::getPassword() const {
	return this->_password;
}

bool Server::isNicknameTaken(const std::string& nickname) const {
	for (std::map<int, Client>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		if (it->second.getNickName() == nickname) {
			return true;
		}
	}
	return false;
}


	// struct pollfd {
	//    int   fd;         /* descripteur de fichier */
	//    short events;     /* événements demandés */
	//    short revents;    /* événements retournés */
	// };

// === MEMBER FUNCTIONS ===
void	Server::run() {

	// Polling setup --> man 2 poll
	struct pollfd pfd;
	pfd.fd = this->_sockfd;
	pfd.events = POLLIN;
	this->_pollfds.push_back(pfd);

	while (this->_running) {
		int activity = poll(this->_pollfds.data(), this->_pollfds.size(), -1);
		if (activity < 0) {
			std::cerr << RED << "Poll failed" << RESET << std::endl;
			break;
		}

		// Si nouvelle connexion
		if (this->_pollfds[0].revents & POLLIN) {
			handleNewConnection();
		}

		// Si message d'un client
		for (size_t i = 1; i < this->_pollfds.size(); ++i) {
			if (this->_pollfds[i].revents & POLLIN) {
				handleClientMessage(this->_pollfds[i].fd);
			}
		}
	}
}

void	Server::handleNewConnection() {
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientfd = accept(this->_sockfd, (struct sockaddr*)&clientAddr, &clientLen);
	if (clientfd == -1) {
		std::cerr << RED << "Accept failed" << RESET << std::endl;
		return;
	}

	if (fcntl(clientfd, F_SETFL, O_NONBLOCK) == -1) {
		close(clientfd);
		throw std::runtime_error("fcntl failed");
	}

	struct pollfd pfd;
	pfd.fd = clientfd;
	pfd.events = POLLIN;
	this->_pollfds.push_back(pfd);

	char host[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, host, INET_ADDRSTRLEN);
	Client newClient(clientfd, std::string(host));

	this->_clients[clientfd] = newClient;

	std::cout << GREEN << "\nClient <" << clientfd << "> connected\n" << RESET << std::endl;
}

void	Server::handleClientMessage(int clientfd) {
	char buffer[1024] = {0};
	ssize_t bytes = recv(clientfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes > 0) {
		buffer[bytes] = '\0';
		Client& client = this->_clients[clientfd];

		client.appendToBuffer(buffer);
		while (client.hasCompleteCommand())
		{
			std::string cmd = client.extractNextCommand();
			parseCommand(*this, client, cmd);
		}
	} else {
		if (bytes == 0) {
			handleClientDisconnection(clientfd);
		} else {
			std::cerr << RED << "Receive failed from client <" << clientfd << ">" << RESET << std::endl;
		}
	}
}

void	Server::handleClientDisconnection(int clientfd) {
	std::cout << YELLOW << "\nClient <" << clientfd << "> disconnected\n" << RESET << std::endl;

	for (size_t i = 0; i < this->_pollfds.size(); ++i) {
		if (this->_pollfds[i].fd == clientfd) {
			this->_pollfds.erase(this->_pollfds.begin() + i);
			break;
		}
	}

	this->_clients.erase(clientfd);
	close(clientfd);
}

void	Server::sendMessageToClient(int clientfd, const std::string& message) {
	ssize_t bytesSent = send(clientfd, message.c_str(), message.length(), 0);
	if (bytesSent == -1) {
		std::cerr << RED << "Send failed to client <" << clientfd << ">" << RESET << std::endl;
	}
}

void	Server::broadcastMessage(const std::string& message, int excludeFd) {
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		if (it->first != excludeFd) {
			sendMessageToClient(it->first, message);
		}
	}
}

void	Server::signalHandler(int signum) {
	if (signum == SIGINT || signum == SIGTERM || signum == SIGQUIT) {
		std::cout << YELLOW << std::endl << "Shutting down server..." << RESET << std::endl;
		this->_running = false;
		closeAllConnections();
		exit(0);
	}
}

void	Server::closeAllConnections() {
	for (size_t i = 0; i < this->_pollfds.size(); i++) {
		if (this->_pollfds[i].fd != this->_sockfd)
			close(this->_pollfds[i].fd);
	}
	this->_clients.clear();
	this->_pollfds.clear();
	close(this->_sockfd);
}


// === NON-MEMBER FUNCTIONS ===
bool	invalidPort(int port) {
	if (port <= 0 || port > 65535) {
		std::cerr << RED << "Error: Invalid Port" << RESET << std::endl;
		return true;
	}
	return false;
}