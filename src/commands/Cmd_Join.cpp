#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <sstream>

void	cmdJoin(Server& server, Client& client, std::istringstream& iss)
{
	std::string	nameChannel;
	iss >> nameChannel;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (nameChannel.empty() || nameChannel[0] != '#') {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "JOIN #channel"));
		return;
	}

	for (size_t i = 1; i < nameChannel.length(); i++)
	{
		if (nameChannel[i] == ' ' || nameChannel.length() < 2) {
			server.sendMessageToClient(client.getFd(),
				ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), nameChannel));
			return;
		}
	}

	if (nameChannel[0] == '#')
		nameChannel = nameChannel.substr(1);

	if (nameChannel.length() > 20) {
		server.sendMessageToClient(client.getFd(),
			ERR_CHANNELNAMENOTALLOWED(SERVER_NAME, client.getNickName(), "Channelname too long +20 chars max"));
		return;
	}

	size_t found = nameChannel.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_");
	if (found != std::string::npos) {
		server.sendMessageToClient(client.getFd(), 
			ERR_CHANNELNAMENOTALLOWED(SERVER_NAME, client.getNickName(), "Channelname contains invalid characters"));
		return;
	}

	Channel* channel = NULL;
	std::map<std::string, Channel*>::iterator it = server.getChannel(nameChannel);

	if (it != server.getChannels().end())
		channel = it->second;
	else {
		std::string tooManyParams;
		if (iss >> tooManyParams) {
			server.sendMessageToClient(client.getFd(),
				ERR_TOOMANYPARAMS(SERVER_NAME, client.getNickName(), "JOIN #channel"));
			return;
		}

		server.createChannel(nameChannel, client);
		channel = server.getChannel(nameChannel)->second;
	}

	if (channel->hasClient(&client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_ALREADYINCHANNEL(SERVER_NAME, client.getNickName(), nameChannel));
		return;
	}

	// if channel is password restricted (+t)
	if (channel->isPasswordProtected()) {
		std::string password;
		iss >> password;

		if (password.empty()) {
			server.sendMessageToClient(client.getFd(), 
				ERR_BADCHANNELKEY(SERVER_NAME, client.getNickName(), nameChannel));
			return;
		}

		if (password != channel->getPassword()) {
			server.sendMessageToClient(client.getFd(), 
				ERR_BADCHANNELKEY(SERVER_NAME, client.getNickName(), nameChannel));
			return;
		}
	}

	// if channel is InvitedOnly (+i)
	if (channel->isInviteOnly() && !channel->isInvited(client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_INVITEONLYCHAN(SERVER_NAME, client.getNickName()));
		return;
	}

	if (channel->isInvited(client))
		channel->removeInvitedClient(client);

	// if user limit reached (+l)
	if (channel->getUserLimit() > 0 && channel->getMemberCount() >= channel->getUserLimit()) {
		server.sendMessageToClient(client.getFd(), 
			ERR_CHANNELISFULL(SERVER_NAME, client.getNickName(), nameChannel));
		return;
	}

	channel->addClient(&client);

	CLIENT_JOIN(client.getFd(), client.getNickName(), nameChannel);
	server.sendMessageToClient(client.getFd(),
		RPL_JOINMSG(client.getNickName(), client.getUserName(), client.getHostName(), nameChannel));

	std::vector<Client*> clients = channel->getClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getFd() == client.getFd())
			continue;
		server.sendMessageToClient(clients[i]->getFd(),
			RPL_JOINMSG(client.getNickName(), client.getUserName(), client.getHostName(), nameChannel));
	}

	std::string clientsList;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getNickName().empty())
			continue;
		if (!clientsList.empty())
			clientsList += " ";
		clientsList += clients[i]->getNickName();
	}

	server.sendMessageToClient(client.getFd(),
		RPL_NAMREPLY(SERVER_NAME, client.getNickName(), nameChannel, clientsList));

	server.sendMessageToClient(client.getFd(),
		RPL_ENDOFNAMES(SERVER_NAME, client.getNickName(), nameChannel));
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
cmdJoin:
Extrait le nom du channel (#test)
Check si le client est deja enregistrer
Si le nom du channel est vide ou ne commence pas par # = erreur
Aucun espace dans le nom du channel, doit contenir au moins (#t)
Si le channel existe, recupere la cle du channel existant
Sinon, creation du channel, recupere ensuite la cle
Si le client n'est pas le channel, on l'ajoute
Parcours tous les clients, envoie du message join seulement aux autres
Recupere la liste des clients
Parcours la liste des clients, si le client est le meme, passe
Sinon ajoute le client a la liste
Afficher les membres incluent dans le channel
Afficher la fin de la liste des membres
-
JOIN #test
=============================================================
*/
