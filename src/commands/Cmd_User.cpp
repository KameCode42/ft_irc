#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdUser(Server& server, Client& client, std::istringstream& iss)
{
	if (client.getIsRegistered()) {
		server.sendMessageToClient(client.getFd(),
			ERR_ALREADYREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	std::string userName, hostName, serverName, realName;
	iss >> userName >> hostName >> serverName;
	std::getline(iss, realName);

	if (realName.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "USER"));
		return;
	}

	if (!realName.empty() && (realName[0] == ' ' || realName[0] == ':'))
		realName.erase(0, 1);

	if (userName.empty() || hostName.empty() || serverName.empty() || realName.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "USER"));
		return;
	}

	client.setUserName(userName);
	//client.setHostName(hostName);
	client.setRealName(realName);
	client.setHasUser(true);

	if (client.getHasPass() && client.getHasNick() && client.getHasUser()) {
		client.setIsRegistered(true);

		CLIENT_REGISTERED(client.getFd(), client.getNickName());
		server.sendMessageToClient(client.getFd(),
			RPL_WELCOME(SERVER_NAME, client.getNickName()));
		return;
	}
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdUser:
si deja enregistrer, return erreur
extraire la ligne complete
stocker la derniere ligne "realname"
supprime espace et : au debut de la ligne
aucun parametre ne peut etre vide
stock les donne et passe user a true
si le pass est ok, nick ok et user ok = connection ok
-
USER <username> <hostname> <servername> :<realname>
le nom du server est ignorer
USER axds 0 * :axds
=============================================================
*/
