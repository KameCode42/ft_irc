#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdPass(Server& server, Client& client, std::istringstream& iss)
{
	if (client.getIsRegistered()) {
		server.sendMessageToClient(client.getFd(),
			ERR_ALREADYREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	std::string password;
	iss >> password;

	if (password.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "PASS"));
		return;
	}

	if (password != server.getPassword()) {
		server.sendMessageToClient(client.getFd(),
			ERR_PASSWDMISMATCH(SERVER_NAME, client.getNickName()));
		return;
	}

	client.setPassWord(password);
	client.setHasPass(true);
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdPass:
Si client deja enregistrer, return erreur
Extrait le mot de passe
Si vide, return erreur manque parametre
Si != du mdp server, return erreur
Enregistre le mdp et le met a true
-
PASS mdp
=============================================================
*/
