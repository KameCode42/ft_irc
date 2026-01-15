#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdPing(Server& server, Client& client, std::istringstream& iss)
{
	std::string tokenPing;
	iss >> tokenPing;

	if (tokenPing.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOORIGIN(SERVER_NAME, client.getNickName()));
		return;
	}

	if (!tokenPing.empty() && tokenPing[0] == ':')
		tokenPing.erase(0, 1);

	if (tokenPing.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOORIGIN(SERVER_NAME, client.getNickName()));
		return;
	}

	server.sendMessageToClient(client.getFd(), RPL_PONG(SERVER_NAME, tokenPing));
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdPing: PING et PONG
Ces deux commandes servent a maintenir la connexion “vivante”
entre le client et le serveur
Sur IRC, un client et un serveur peuvent rester connectés longtemps sans activité.
Pour éviter de garder des connexions “mortes” (par exemple
si un client a crashé ou s’est déconnecté sans envoyer QUIT), on utilise le ping/pong.
-
Commande ping:
PING :<token> (ou "server name")
client envoie : PING :123456
serveur doit répondre immédiatement :
:PONG :123456
Cela prouve que la connexion est toujours active.
Client reçoit -> :ft_irc PONG :123
-
Commande pong:
PONG :<token>
La commande PONG n'est generalement pas taper par un user
Le client doit connaitre la commande mais ne fais rien si taper
=============================================================
*/
