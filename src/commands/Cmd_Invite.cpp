#include "Server.hpp"
#include "Client.hpp"
#include "Messages.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <sstream>

void cmdInvite(Server &server, Client &client, std::istringstream &iss) {
	std::string targetNickname;
	iss >> targetNickname;
	std::string channelName;
	iss >> channelName;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (targetNickname.empty() || channelName.empty()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "INVITE"));
		return;
	}

	if (channelName[0] != '#') {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), channelName));
		return;
	}
	channelName = channelName.substr(1);

	Client *invitedClient = server.getClientByNickname(targetNickname);
	if (!invitedClient) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHNICK(SERVER_NAME, client.getNickName(), targetNickname));
		return;
	}

	std::map<std::string, Channel*>::iterator it = server.getChannel(channelName);
	if (it == server.getChannels().end()) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	Channel *channel = it->second;
	if (!channel->isMember(client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTONCHANNEL(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	if (!channel->isOperator(client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_CHANOPRIVSNEEDED(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	if (channel->isMember(*invitedClient)) {
		server.sendMessageToClient(client.getFd(),
			ERR_USERNOTINCHANNEL(SERVER_NAME, client.getNickName(), targetNickname, channelName));
		return;
	}

	if (channel->isInviteOnly() && !channel->isOperator(client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_CHANOPRIVSNEEDED(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	channel->addInvitedClient(*invitedClient);

	std::string inviteMsg = ":" + client.getNickName() + "!" + client.getUserName()
						  + "@" + client.getHostName() + " INVITE " + invitedClient->getNickName()
						  + " #" + channelName + EOL;
	
	server.sendMessageToClient(invitedClient->getFd(), inviteMsg);

	server.sendMessageToClient(client.getFd(), 
		RPL_INVITING(client.getNickName(), client.getUserName(), client.getHostName(), invitedClient->getNickName(), channelName));
}
