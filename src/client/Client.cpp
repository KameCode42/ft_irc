#include "Client.hpp"

/*=== Constructeur ===*/

Client::Client() :
	_fd(-1),
	_passWord(""),
	_hostName(""),
	_nickName(""),
	_userName(""),
	_realName(""),
	_buffer(""),
	_hasPass(false),
	_hasNick(false),
	_hasUser(false),
	_isRegistered(false) {}

Client::Client(int fd, std::string const& host) :
	_fd(fd),
	_hostName(host),
	_nickName(""),
	_userName(""),
	_realName(""),
	_buffer(""),
	_hasPass(false),
	_hasNick(false),
	_hasUser(false),
	_isRegistered(false) {}

Client::~Client() {}

/*==================================================================================*/

/*=== Getter & Setter ===*/

int	Client::getFd()const {
	return this->_fd;
}

void	Client::setFd(int fd) {
	this->_fd = fd;
}

std::string	Client::getPassWord()const {
	return this->_passWord;
}

void	Client::setPassWord(std::string const& passWord) {
	this->_passWord = passWord;
}

std::string	Client::getNickName()const {
	return this->_nickName;
}

void	Client::setNickName(std::string const& nickName) {
	this->_nickName = nickName;
}

std::string	Client::getUserName()const {
	return this->_userName;
}

void	Client::setUserName(std::string const& userName) {
	this->_userName = userName;
}

std::string	Client::getHostName()const {
	return this->_hostName;
}

void	Client::setHostName(std::string const& hostName) {
	this->_hostName = hostName;
}

std::string	Client::getRealName()const {
	return this->_realName;
}

void	Client::setRealName(std::string const& realName) {
	this->_realName = realName;
}

std::string	Client::getBuffer()const {
	return this->_buffer;
}

void	Client::setBuffer(std::string const& buffer) {
	this->_buffer = buffer;
}

bool	Client::getHasPass()const {
	return this->_hasPass;
}

void	Client::setHasPass(bool status) {
	this->_hasPass = status;
}

bool	Client::getHasNick()const {
	return this->_hasNick;
}

void	Client::setHasNick(bool status) {
	this->_hasNick = status;
}

bool	Client::getHasUser()const {
	return this->_hasUser;
}

void	Client::setHasUser(bool status) {
	this->_hasUser = status;
}

bool	Client::getIsRegistered()const {
	return this->_isRegistered;
}

void	Client::setIsRegistered(bool status) {
	this->_isRegistered = status;
}

/*==================================================================================*/

/*=== Fonction membre ===*/

void	Client::appendToBuffer(std::string const& data) {
	if (data.empty())
		return;

	if (_buffer.size() + data.size() > BUFFER_MAX)
		throw std::runtime_error("Error : command failed");

	_buffer += data;

	size_t found = _buffer.rfind("\r\n");
	size_t currentLine;

	if (found == std::string::npos)
		currentLine = _buffer.size();
	else
		currentLine = _buffer.size() - (found + 2);

	if (currentLine > LINE_MAX)
		throw std::runtime_error("Error : line too long (RFC 1459 violation)");
}

bool	Client::hasCompleteCommand()const {
	if (_buffer.empty())
		return false;

	size_t found = _buffer.rfind("\r\n");
	if (found == std::string::npos)
		return false;
	return true;
}

std::string	Client::extractNextCommand() {
	if (!hasCompleteCommand())
		return "";

	while (_buffer.find("\r\n") == 0)
		_buffer.erase(0, 2);

	size_t found = _buffer.find("\r\n");
	if (found == std::string::npos)
		return "";
	
	std::string cmd = _buffer.substr(0, found);
	_buffer.erase(0, found + 2);

	return cmd;
}

/*==================================================================================*/
/*==================================================================================*/

/*=== note fonction ===*/
/*
=============================================================
appendToBuffer(std::string const& data):
- fonction qui permet de controler si une commande contient \r\n
et d'ajoute celle ci dans un buffer temporaire
- la taille d une commande complete ne doit pas depasser 512
- data = commande recu = PASS f \r\n NICK a \r\n
- buffer va recevoir la ligne complete
- condition npos permet de controler si on recoit une commande
decouper
- currentLine contient la commande complete jusqu'au
dernier \r\n
=============================================================

=============================================================
hasCompleteCommand:
- fonction qui permet de controler si une commande complete
se presente comme ceci : PASS f \r\n NICK a \r\n dans buffer
=============================================================

=============================================================
extractNextCommand():
- fonction qui permet d'extraire une premiere commande si
hasCompleteCommand est true
- retire les \r\n qui peuvent se trouver au debut
- recherche la premiere occurence \r
- si trouver, on extrait la commande (PASS f)
- supprimer cette partie du buffer (new buffer -> NICK a \r\n)
- return la commande extraite
- passe a la suivante si y en a une
=============================================================
*/
