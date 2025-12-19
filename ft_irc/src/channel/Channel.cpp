#include "Channel.hpp"

/*=== Constructeur ===*/

Channel::Channel(std::string name) : _name(name), _clients() {}

Channel::~Channel() {}

/*==================================================================================*/

/*=== getter & setter ===*/

std::string	Channel::getName()const {
	return this->_name;
}

const std::vector<Client*>&	Channel::getClients()const {
	return this->_clients;
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

	_clients.push_back(client);
}

void	Channel::removeClient(Client* client)
{
	if (client == NULL)
		return;

	for (size_t i = 0; i < _clients.size(); i++) {
		if (client == _clients[i]) {
			_clients.erase(_clients.begin() + 1);
			return;
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
