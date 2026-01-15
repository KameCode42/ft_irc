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

	server.notifySharedChannelsQuit(client, quitMessage);
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
broadcast -> parcourt tous les clients connectes et envoie message a chacun
handleclient -> nettoie la connexion du client qui vient de quitter
				ferme le socket TCP et le supprime de la liste client
-
QUIT:[quit message] -> quit message pas obligatoire
sortie -> :Nick!User@Host QUIT :Message
sortie -> :Zanox!alice@127.0.0.1 QUIT :Bye bye!
=============================================================
*/
