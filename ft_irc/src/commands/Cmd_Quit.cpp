#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdQuit(Server& server, Client& client, std::istringstream& iss)
{
	std::string quitMessage;
	iss >> quitMessage;

	if (quitMessage.empty())
		quitMessage = "Client Quit";
	else if (quitMessage[0] == ':')
		quitMessage = quitMessage.substr(1);

	std::string fullQuitMsg = RPL_QUITMSG(client.getNickName(), client.getUserName(), client.getHostName(), quitMessage);
	server.broadcastMessage(fullQuitMsg, client.getFd());
	server.handleClientDisconnection(client.getFd());
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdQuit:
extraire le quit message
si vide, return par defaut "Client quit"
si non, extrait la sous chaine du message
-
QUIT:[quit message] -> quit message pas obligatoire
=============================================================
*/
