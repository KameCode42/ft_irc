#include <sstream>
#include "Server.hpp"

void cmdTopic(Server &server, Client &client, std::istringstream &iss) {

	std::string channelName;
	iss >> channelName;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (channelName.empty() || channelName[0] != '#') {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "TOPIC #channel :<topic>"));
		return;
	}

	channelName = channelName.substr(1);

	std::map<std::string, Channel *>::iterator it = server.getChannel(channelName);
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

	std::string newTopic;
	std::getline(iss, newTopic);
	if (!newTopic.empty() && newTopic[0] == ' ')
		newTopic = newTopic.substr(1);

	if (newTopic[0] == ':')
		newTopic = newTopic.substr(1);

	if (newTopic.empty()) {
		if (channel->getTopic().empty()) {
			server.sendMessageToClient(client.getFd(), 
				RPL_NOTOPIC(SERVER_NAME, client.getNickName(), channelName));
		} 
		else {
			server.sendMessageToClient(client.getFd(), 
				RPL_TOPICIS(SERVER_NAME, client.getNickName(), channelName, channel->getTopic()));
		}
	} 
	else {
		if (channel->isTopicRestricted() && !channel->isOperator(client)) {
			server.sendMessageToClient(client.getFd(), 
				ERR_CHANOPRIVSNEEDED(SERVER_NAME, client.getNickName(), channelName));
			return;
		}

		channel->setTopic(newTopic);

		std::string topicMsg = ":" + client.getNickName() + "!" + client.getUserName()
						   + "@" + client.getHostName() + " TOPIC #" + channelName + " :" + newTopic + EOL;

		channel->broadcastToMembers(topicMsg, -1);
	}
}
