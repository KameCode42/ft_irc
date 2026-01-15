#include <sstream>
#include "Server.hpp"

void cmdKick(Server &server, Client& client, std::istringstream& iss) {

	std::string channelName;
	iss >> channelName;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (channelName.empty() || channelName[0] != '#') {
		server.sendMessageToClient(client.getFd(), 
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "KICK <channel> <user> [<comment>]"));
		return;
	}

	channelName = channelName.substr(1);

	std::map<std::string, Channel*>::iterator it = server.getChannel(channelName);
	if (it == server.getChannels().end()) {
		server.sendMessageToClient(client.getFd(), 
			ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	Channel* channel = it->second;

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

	std::string targetNickname;
	iss >> targetNickname;

	if (targetNickname.empty()) {
		server.sendMessageToClient(client.getFd(), 
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "KICK"));
		return;
	}

	Client* targetClient = NULL;
	for (size_t i = 0; i < channel->getMembers().size(); ++i) {
		if (channel->getMembers()[i]->getNickName() == targetNickname) {
			targetClient = channel->getMembers()[i];
			break;
		}
	}

	if (!targetClient) {
		server.sendMessageToClient(client.getFd(), 
			ERR_USERNOTINCHANNEL(SERVER_NAME, client.getNickName(), targetNickname, channelName));
		return;
	}

	std::string reason;
	std::getline(iss, reason);

	if (!reason.empty() && reason[0] == ' ')
		reason.erase(0, 1);

	if (!reason.empty() && reason[0] == ':')
		reason.erase(0, 1);

	std::string kickMsg = ":" + client.getNickName() + "!" + client.getUserName()
						  + "@" + client.getHostName() + " KICK #" + channelName + " " + targetNickname;

	if (!reason.empty())
		kickMsg = kickMsg + " " + reason;

	kickMsg += EOL;

	channel->broadcastToMembers(kickMsg, -1);

	channel->removeMember(*targetClient);

	if (channel->isOperator(*targetClient))
		channel->removeOperator(*targetClient);

	if (channel->isInvited(*targetClient))
		channel->removeInvitedClient(*targetClient);

	if (channel->isEmpty())
		server.removeChannel(channelName);
}
