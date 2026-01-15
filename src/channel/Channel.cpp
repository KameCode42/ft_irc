#include "Channel.hpp"

/*=== Constructeur ===*/

Channel::Channel(std::string name) : _name(name), _clients(), _topic(""),
		_inviteOnly(false), _topicRestricted(false),
		_passwordProtected(false), _operatorOnly(false), _userLimit(0) {}

Channel::Channel(const std::string& name, Client& creator)
	: _name(name), _topic(""), _inviteOnly(false), _topicRestricted(false),
	  _passwordProtected(false), _operatorOnly(false), _userLimit(0) {
	this->addMember(creator);
	this->addOperator(creator);
}

Channel::~Channel() {}

/*==================================================================================*/

/*=== Getter & Setter ===*/

std::string	Channel::getName()const {
	return this->_name;
}

const std::vector<Client*>&	Channel::getClients()const {
	return this->_clients;
}

size_t	Channel::getMemberCount() const {
	return (this->_members.size());
}

std::vector<Client*> Channel::getMembers() const {
	return this->_members;
}

std::vector<Client*> Channel::getOperators() const {
	return this->_operators;
}

/*==================================================================================*/

/*=== Fonction membre ===*/

void	Channel::addClient(Client* client)
{
	if (client == NULL)
		return;

	for (size_t i = 0; i < _clients.size(); i++) {
		if (client == _clients[i])
			return;
	}

	this->_clients.push_back(client);
	this->_members.push_back(client);
}

void	Channel::removeClient(Client* client)
{
	if (client == NULL)
		return;

	for (size_t i = 0; i < _clients.size(); i++) {
		if (client == _clients[i]) {
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}

	for (std::vector<Client*>::iterator it = this->_members.begin(); it != this->_members.end(); ++it) {
		if (*it == client) {
			this->_members.erase(it);
			break;
		}
	}
}

bool	Channel::hasClient(Client* client)const
{
	if (client == NULL)
		return false;

	for (size_t i = 0; i < _clients.size(); i++) {
		if (client == _clients[i])
			return true;
	}
	return false;
}

bool	Channel::isEmpty()const
{
	if (_clients.size() == 0)
		return true;
	return false;
}

void	Channel::addMember(Client& client) {
	this->addClient(&client);
}

void	Channel::removeMember(Client& client) {
	this->removeClient(&client);
}

bool	Channel::isMember(const Client& client) const {
	for (size_t i = 0; i < this->_members.size(); ++i) {
		if (this->_members[i]->getFd() == client.getFd()) {
			return true;
		}
	}
	return false;
}

void	Channel::addOperator(Client& client) {
	this->_operators.push_back(&client);
}

void	Channel::removeOperator(Client& client) {
	for (std::vector<Client*>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it) {
		if (*it == &client) {
			this->_operators.erase(it);
			return;
		}
	}
}

bool	Channel::isOperator(const Client& client) const {
	for (size_t i = 0; i < this->_operators.size(); ++i) {
		if (this->_operators[i]->getFd() == client.getFd()) {
			return true;
		}
	}
	return false;
}

void	Channel::addInvitedClient(Client& client) {
	this->_invited.push_back(&client);
}

void	Channel::removeInvitedClient(Client& client) {
	for (std::vector<Client*>::iterator it = this->_invited.begin(); it != this->_invited.end(); ++it) {
		if (*it == &client) {
			this->_invited.erase(it);
			return;
		}
	}
}

bool	Channel::isInvited(const Client& client) const {
	for (size_t i = 0; i < this->_invited.size(); ++i) {
		if (this->_invited[i]->getFd() == client.getFd()) {
			return true;
		}
	}
	return false;
}

void	Channel::broadcastToMembers(const std::string& message, int excludeFd) {
	for (size_t i = 0; i < this->_members.size(); ++i) {
		if (this->_members[i]->getFd() != excludeFd) {
			ssize_t bytesSent = send(this->_members[i]->getFd(), message.c_str(), message.length(), 0);
			if (bytesSent == -1) {
				std::cerr << RED << "Send failed to client <" << this->_members[i]->getFd() << ">" << RESET << std::endl;
			}
		}
	}
}

/*==================================================================================*/

/*=== MODES ===*/

const std::string&	Channel::getTopic() const {
	return this->_topic;
}

void	Channel::setTopic(const std::string& topic) {
	this->_topic = topic;
}

bool	Channel::isInviteOnly() const {
	return this->_inviteOnly;
}

void	Channel::setInviteOnly(bool status) {
	this->_inviteOnly = status;
}

bool	Channel::isTopicRestricted() const {
	return this->_topicRestricted;
}

void	Channel::setTopicRestricted(bool status) {
	this->_topicRestricted = status;
}

bool	Channel::isPasswordProtected() const {
	return this->_passwordProtected;
}

void	Channel::setPasswordProtected(bool status) {
	this->_passwordProtected = status;
}

const std::string&	Channel::getPassword() const {
	return this->_password;
}

void	Channel::setPassword(const std::string& password) {
	this->_password = password;
}

bool	Channel::isOperatorOnly() const {
	return this->_operatorOnly;
}

void	Channel::setOperatorOnly(bool status) {
	this->_operatorOnly = status;
}

size_t	Channel::getUserLimit() const {
	return this->_userLimit;
}

void	Channel::setUserLimit(size_t limit) {
	this->_userLimit = limit;
}

std::string	Channel::getModesStr() const {
	std::string modes = "+";

	if (this->isInviteOnly())
		modes += "i";
	if (this->isTopicRestricted())
		modes += "t";
	if (this->isPasswordProtected())
		modes += "k";
	if (this->isOperatorOnly())
		modes += "o";
	if (this->getUserLimit() > 0)
		modes += "l";
	return modes;
}

std::string	Channel::getModeParamsStr() const {
	std::string params = " ";

	if (this->isPasswordProtected())
		params += this->getPassword() + " ";
	if (this->getUserLimit() > 0) {
		std::ostringstream oss;
		oss << this->getUserLimit();
		params += oss.str() + " ";
	}
	return params;
}

/*==================================================================================*/
/*==================================================================================*/

/*=== note fonction ===*/
/*
=============================================================
getName:
retourne le nom du channel (#test)
=============================================================

=============================================================
getClients:
retourne la liste des clients (lecture seule)
=============================================================

=============================================================
addClient:
parcours la liste des clients
si client existe, ne rien faire
sinon ajoute un client a la liste
=============================================================

=============================================================
removeClient:
parcours la liste des clients
si client existe, supprime
sinon inexistant, return simlpe
=============================================================

=============================================================
hasClient:
Verifie si un client est present dans le channel
=============================================================

=============================================================
isEmpty:
Verifie si le channel est vide
=============================================================
*/
