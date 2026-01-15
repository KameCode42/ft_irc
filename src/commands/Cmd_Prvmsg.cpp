#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <sstream>

void	cmdPrvmsg(Server& server, Client& client, std::istringstream& iss)
{
	std::string target;
	iss >> target;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (target.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "PRIVMSG"));
		return;
	}

	std::string message;
	std::getline(iss, message);

	if (message.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTEXTTOSEND(SERVER_NAME, client.getNickName()));
		return;
	}

	while (!message.empty() && message[0] == ' ')
		message.erase(0, 1);

	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);

	if (message.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTEXTTOSEND(SERVER_NAME, client.getNickName()));
		return;
	}

	// =====================================================================
	// BRANCHE A : target est un CHANNEL (#channel)
	// =====================================================================

	if (!target.empty() && target[0] == '#')
	{
		Channel* channel = NULL;
		std::string targetNoHash = target.substr(1);
		std::map<std::string, Channel*>::iterator it = server.getChannel(targetNoHash);

		if (it == server.getChannels().end()) {
			server.sendMessageToClient(client.getFd(),
				ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), target));
			return;
		}
		channel = it->second;

		if (!channel->hasClient(&client)) {
			server.sendMessageToClient(client.getFd(),
				ERR_NOTONCHANNEL(SERVER_NAME, client.getNickName(), target));
			return;
		}
		std::vector<Client*> clients = channel->getClients();

		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i]->getFd() == client.getFd())
				continue;
			server.sendMessageToClient(clients[i]->getFd(),
				RPL_PRVMSG_CHANNEL(client.getNickName(), client.getUserName(),
									client.getHostName(), targetNoHash, message));
		}
		return;
	}

	// =====================================================================
	// BRANCHE B : target est un USER (nickname)
	// =====================================================================

	Client* dest = server.getClientByNickname(target);

	if (dest == NULL) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHNICK(SERVER_NAME, client.getNickName(), target));
		return;
	}

	if (dest->getFd() == client.getFd())
		return;

	server.sendMessageToClient(dest->getFd(),
		RPL_PRVMSG_USER(client.getNickName(), client.getUserName(),
						client.getHostName(), target, message));
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdPrvMsg:
Extraire target (soit channel, soit user)
Verifier si le client est enregistrer
Verifier que target ne soit pas vide
Extraire le message
Si le message est vide -> erreur
Effacer tous les espaces au debut
Effacer les ':' avant le message
Verifier que le message ne soit pas vide
-
Branche A - channel:
Si target commence par '#' = channel
Controle que le channel existe sinon erreur
Recupere la cle du channel
Controle si le client est dans le channel
Parcours la liste des clients
Si le client est le meme, continue
Sinon envoie le message a tous les membres du channel
-
Branche B - user:
Recupere la cible targer nickname
Controle si celui ci existe si non -> erreur
Si le dest est le meme que le client, passe
Envoie le message au destinataire
-
PRIVMSG Bob :salut -> PRIVMSG <nickname> :<message>
PRIVMSG #test :hello tout le monde -> PRIVMSG <#channel> :<message>
=============================================================
*/
