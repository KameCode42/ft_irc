#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <sstream>

void	cmdPart(Server& server, Client& client, std::istringstream& iss)
{
	std::string nameChannel;
	iss >> nameChannel;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (nameChannel.empty() || nameChannel[0] != '#') {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "PART #channel"));
		return;
	}

	std::string channelName;
	if (nameChannel[0] == '#')
		channelName = nameChannel.substr(1);

	std::map<std::string, Channel*>::iterator it = server.getChannel(channelName);
	if (it == server.getChannels().end()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), nameChannel));
		return;
	}

	Channel* channel = it->second;

	if (!channel->hasClient(&client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTONCHANNEL(SERVER_NAME, client.getNickName(), nameChannel));
		return;
	}

	std::string quitMessage;
	std::getline(iss, quitMessage);

	if (quitMessage.empty())
		quitMessage = "Client Quit";
	else {
		if (quitMessage[0] == ' ' || quitMessage[0] == ':')
			quitMessage.erase(0, 1);
	}

	std::vector<Client*> clients = channel->getClients();

	for (size_t i = 0; i < clients.size(); i++) {
		server.sendMessageToClient(clients[i]->getFd(),
			RPL_PARTMSG(client.getNickName(), client.getUserName(), client.getHostName(),
						channelName, quitMessage));
	}

	CLIENT_QUIT_CHANNEL(client.getFd(), client.getNickName(), nameChannel);
	channel->removeClient(&client);

	if (channel->isOperator(client))
		channel->removeOperator(client);

	if (channel->isInvited(client))
		channel->removeInvitedClient(client);

	if (channel->isEmpty() == true) {
		REMOVE_CHANNEL(nameChannel);
		server.removeChannel(nameChannel);
	}
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdPart:
Extrait le nom du channel
Si le client n'est pas enregistrer = erreur
Si le nom du channel est vide ou n'a pas # (#test) = erreur
Check si dans le serveur le channel existe, erreur si non
Associe la cle au channel
Si le client n'est pas present dans le channel = erreur
Check si un message optionnel est ecrit
Recupere le reste du message iss
Si le message est vide = message par defaut
Si commence par : ou espace, supprime le premier character
Sinon parcours la liste des clients et envoie a chacun le message
Le client est supprimer du channel
Si le channel est vide, le supprimer du server
-
PART #test
PART #test :bye
=============================================================
*/
