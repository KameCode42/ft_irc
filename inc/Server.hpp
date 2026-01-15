#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <exception>
#include <map>
#include <vector>
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <csignal>
#include <cerrno>
#include "Client.hpp"
#include "Messages.hpp"
#include "Channel.hpp"

#define SERVER_NAME		"ft_irc"
#define SERVER_VERSION	"1.0"

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"


class Client;
class Channel;

class Server {
private:
	// ==== SERVER ===
	int								_port;
	std::string						_password;
	int								_sockfd;
	int								_running;

	// ==== CLIENTS ===
	std::map<int, Client>			_clients;
	std::vector<struct pollfd>		_pollfds;

	// ==== CHANNELS ===
	std::map<std::string, Channel*>	_channels;

public:
	Server(int port, const std::string& pass);
	~Server();

	void	run();

	// === CLIENT ===
	std::map<int, Client>&			getClients();
	const std::map<int, Client>&	getClients() const;
	Client* getClientByNickname(const std::string& nickname);

	void	handleNewConnection();
	void	handleClientMessage(int clientfd);
	void	handleClientDisconnection(int clientfd);

	void	sendMessageToClient(int clientfd, const std::string& message);
	void	broadcastMessage(const std::string& message, int excludeFd);
	void    notifySharedChannelsQuit(const Client& client, const std::string& quitMessage);

	// === COMMANDS ===
	const std::string&	getPassword() const;
	bool				isNicknameTaken(const std::string& nickname) const;

	// === CHANNELS ===
	std::map<std::string, Channel*>&			getChannels();
	std::map<std::string, Channel*>::iterator	getChannel(const std::string& name);
	void	createChannel(const std::string& name, Client& newClient);
	void	removeChannel(const std::string& name);

	// === SIGNALS ===
	void	signalHandler(int signum);

	// === CLEANUP ===
	void	closeAllConnections();

};

bool	invalidPort(int port);

#endif
