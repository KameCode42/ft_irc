#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include <sstream>

void	cmdNick(Server& server, Client& client, std::istringstream& iss)
{
	std::string nickName;
	iss >> nickName;

	if (nickName.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NONICKNAMEGIVEN(SERVER_NAME, client.getNickName()));
		return;
	}

	if (nickName.length() > 9) {
		server.sendMessageToClient(client.getFd(),
			ERR_ERRONEUSNICKNAME(SERVER_NAME, client.getNickName()));
		return;
	}

	size_t found = nickName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_");
	if (found != std::string::npos) {
		server.sendMessageToClient(client.getFd(),
			ERR_ERRONEUSNICKNAME(SERVER_NAME, client.getNickName()));
		return;
	}

	if (server.isNicknameTaken(nickName) == true) {
		server.sendMessageToClient(client.getFd(),
			ERR_NICKNAMEINUSE(SERVER_NAME, client.getNickName(), nickName));
		return;
	}

	client.setNickName(nickName);
	client.setHasNick(true);
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdNick:
Extrait le Nick
si vide, return erreur
si taille > 9, return erreur (protocole IRC)
si un caractere ne correspond pas, return erreur
si le nick est deja pris, return erreur
enregistre le nickname
-
NICK axds
=============================================================
*/
