#include "Server.hpp"
#include "Client.hpp"
#include "Commands.hpp"

Server* g_server = NULL;

static void handleSignal(int signum);

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

	if (listen(this->_sockfd, 10) == -1) {
		close(this->_sockfd);
		throw std::runtime_error("Listen failed");
	}

	g_server = this;
		
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handleSignal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

Server::~Server() {
	for (std::map<std::string, Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		delete it->second;
	}
	this->_channels.clear();

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

std::map<int, Client>& Server::getClients() {
	return this->_clients;
}

const std::map<int, Client>& Server::getClients() const {
	return this->_clients;
}

Client* Server::getClientByNickname(const std::string& nickname) {
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (it->second.getNickName() == nickname)
			return &it->second;
	}
	return NULL;
}

// === MEMBER FUNCTIONS ===
void	Server::run() {

	struct pollfd pfd;
	pfd.fd = this->_sockfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	this->_pollfds.push_back(pfd);

	while (this->_running) {
		int activity = poll(this->_pollfds.data(), this->_pollfds.size(), -1);
		if (activity < 0) {
			if (errno == EINTR)
				break;
			std::cerr << RED << "Poll failed" << RESET << std::endl;
			break;
		}

		if (this->_pollfds[0].revents & POLLIN) {
			handleNewConnection();
		}

		for (size_t i = 1; i < this->_pollfds.size(); ++i) {
			if (this->_pollfds[i].revents & POLLIN) {
				handleClientMessage(this->_pollfds[i].fd);
			}
		}
	}
}

/*==================================================================================*/
// CLIENT MANAGEMENT
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
	pfd.revents = 0;
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
			if (this->_clients.find(clientfd) == this->_clients.end())
				break;
		}
	} else {
		if (bytes == 0) {
			std::map<int, Client>::iterator it = _clients.find(clientfd);
			if (it != _clients.end()) {
				notifySharedChannelsQuit(it->second, "Client Quit");
				handleClientDisconnection(clientfd);
			}
		} else {
			std::cerr << RED << "Receive failed from client <" << clientfd << ">" << RESET << std::endl;
		}
	}
}

void	Server::notifySharedChannelsQuit(const Client& client, const std::string& quitMessage) {

	std::string fullQuitMsg = RPL_QUITMSG(client.getNickName(), client.getUserName(), client.getHostName(), quitMessage);

	for (std::map<int, Client>::iterator itClient = this->getClients().begin(); itClient != this->getClients().end(); itClient++) {
		Client& otherClient = itClient->second;

		for (std::map<std::string, Channel*>::iterator itChan = this->getChannels().begin(); itChan != this->getChannels().end(); itChan++) {
			Channel* channel = itChan->second;

			if (otherClient.getFd() != client.getFd() && channel->isMember(otherClient) && channel->isMember(client)) {
				this->sendMessageToClient(otherClient.getFd(), fullQuitMsg);
				break;
			}
		}
	}
}

void	Server::handleClientDisconnection(int clientfd) {
	std::cout << YELLOW << "\nClient <" << clientfd << "> disconnected\n" << RESET << std::endl;

	std::map<int, Client>::iterator itClient = _clients.find(clientfd);
	if (itClient == _clients.end())
		return;

	Client& client = itClient->second;

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ) {
		Channel* channel = it->second;

		if (channel->isMember(client))
			channel->removeClient(&client);

		if (channel->isOperator(client))
			channel->removeOperator(client);

		if (channel->isInvited(client))
			channel->removeInvitedClient(client);

		if (channel->isEmpty()) {
			std::map<std::string, Channel*>::iterator toErase = it;
			++it;
			delete toErase->second;
			_channels.erase(toErase);
		}
		else
			++it;
	}

   for (size_t i = 0; i < _pollfds.size(); ++i) {
        if (_pollfds[i].fd == clientfd) {
            _pollfds.erase(_pollfds.begin() + i);
            break;
        }
    }

    close(clientfd);
    _clients.erase(itClient);
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

/*==================================================================================*/
// SIGNAL HANDLER
static void handleSignal(int signum) {
	if (g_server) {
		g_server->signalHandler(signum);
	}
}

void	Server::signalHandler(int signum) {
	(void)signum;
	this->_running = false;
	std::cout << YELLOW << "\nSignal interrupt: " << signum << "\nServer is shutting down..." << RESET << std::endl;
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

/*==================================================================================*/
// CHANNEL MANAGEMENT
std::map<std::string, Channel*>& Server::getChannels() {
	return this->_channels;
}

std::map<std::string, Channel*>::iterator Server::getChannel(const std::string& name) {
	return this->_channels.find(name);
}

void	Server::createChannel(const std::string& name, Client& newClient) {

	if (this->_channels.find(name) != this->_channels.end())
		return;

	Channel* newChannel = new Channel(name);
	//newChannel->addMember(newClient);
	newChannel->addOperator(newClient);
	this->_channels[name] = newChannel;
}

void	Server::removeChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);

	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

// === NON-MEMBER FUNCTIONS ===
bool	invalidPort(int port) {
	if (port <= 0 || port > 65535) {
		std::cerr << RED << "Error: Invalid Port" << RESET << std::endl;
		return true;
	}
	return false;
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
run: boucle principale du serveur
Utilise la fonction poll pour surveiller les événements sur
plusieurs descripteurs de fichiers (sockets).
- Initialise un descripteur de fichier pour la socket du serveur
- Boucle infinie tant que le serveur est en cours d'exécution
- Utilise poll pour attendre les événements sur les descripteurs de fichiers
- Si une nouvelle connexion est détectée, appelle handleNewConnection
- Si un message d'un client est détecté, appelle handleClientMessage
- Gère les erreurs de poll

handleNewConnection: gère les nouvelles connexions des clients
- Accepte une nouvelle connexion entrante
- Configure la socket du client en mode non bloquant
- Ajoute la socket du client à la liste des descripteurs surveillés par poll
- Crée un nouvel objet Client pour le client connecté
- Affiche un message de connexion réussie

handleClientMessage: gère les messages reçus des clients
- Reçoit les données du client
- Si des données sont reçues, les ajoute au buffer du client
- Traite les commandes complètes extraites du buffer du client
- Si le client se déconnecte, appelle handleClientDisconnection
- Gère les erreurs de réception

handleClientDisconnection: gère la déconnexion des clients
- Affiche un message de déconnexion
- Supprime la socket du client de la liste des descripteurs surveillés par poll
- Supprime le client de la liste des clients connectés
- Ferme la socket du client	

sendMessageToClient: envoie un message à un client spécifique
- Utilise la fonction send pour envoyer le message à la socket du client

broadcastMessage: envoie un message à tous les clients sauf un exclu
- Parcourt la liste des clients connectés
- Envoie le message à chaque client sauf celui dont le descripteur de fichier
correspond à excludeFd

signalHandler: gère les signaux système pour arrêter le serveur proprement
- Si un signal d'arrêt est reçu (SIGINT, SIGTERM, SIGQUIT),
affiche un message d'arrêt et met fin à la boucle principale d'exécution
- Ferme toutes les connexions avant de quitter

closeAllConnections: ferme toutes les connexions clients et la socket du serveur
- Parcourt la liste des descripteurs surveillés par poll
- Ferme chaque socket client
- Vide la liste des clients connectés et des descripteurs surveillés
- Ferme la socket du serveur

addChannel: crée un nouveau channel et ajoute le client créateur en tant que membre et opérateur
- Vérifie si le channel existe déjà
- Crée un nouvel objet Channel
- Ajoute le client créateur en tant que membre et opérateur du channel
- Ajoute le nouveau channel à la liste des channels du serveur



=============================================================
*/
