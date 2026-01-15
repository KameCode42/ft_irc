#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdCap(Server& server, Client& client, std::istringstream& iss)
{
	std::string subcommand;
	iss >> subcommand;

	if (subcommand == "LS")
		;
	else if (subcommand == "END")
		;
	else {
		server.sendMessageToClient(client.getFd(),
			ERR_UNKNOWNCOMMAND(SERVER_NAME, client.getNickName(), "CAP LS"));
		return;
	}
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdCap:
permet a un client (comme WeeChat, HexChat, etc.) 
de négocier des fonctionnalités optionnelles avec le serveur
avant d'etre completement enregistrer
Exemple de capaciter:
- multi-prefix → pour recevoir plusieurs statuts d’un coup (@, +)
- sasl → pour l’authentification sécurisée
- away-notify, account-notify, etc.
pas utiliser pour notre server
=============================================================
*/
