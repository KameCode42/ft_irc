#include <sstream>
#include "Server.hpp"

// ====================================================================================
std::string modeI(Channel *channel, char action) {
	if (action == '+') {
		if (channel->isInviteOnly())
			return "IGNORE";
		channel->setInviteOnly(true);
		return "";
	}
	else {
		if (!channel->isInviteOnly())
			return "IGNORE";
		channel->setInviteOnly(false);
		return "";
	}
}

std::string modeT(Channel *channel, char action) {
	if (action == '+') {
		if (channel->isTopicRestricted())
			return "IGNORE";
		channel->setTopicRestricted(true);
		return "";
	}
	else {
		if (!channel->isTopicRestricted())
			return "IGNORE";
		channel->setTopicRestricted(false);
		return "";
	}
}
std::string modeL(Channel *channel, char action, std::istringstream &iss) {
	if (action == '+') {
		int limit;
		if (!(iss >> limit)) {
			return "ERROR";
		}
		if (limit <= 0) {
			return "ERROR";
		}
		
		channel->setUserLimit(limit);
		std::ostringstream oss;
		oss << limit;
		std::string args = oss.str();
		return args;
	}
	else {
		channel->setUserLimit(0);
		return "";
	}
}

std::string modeK(Channel *channel, char action, std::istringstream &iss) {
	if (action == '+') {
		std::string key;
		if (!(iss >> key))
			return "ERROR";	
		if (key == channel->getPassword())
			return "IGNORE";
		channel->setPasswordProtected(true);
		channel->setPassword(key);
		return key;
	}
	else {
		if (channel->getPassword().empty() && !channel->isPasswordProtected())
			return "IGNORE";
		channel->setPasswordProtected(false);
		channel->setPassword("");
		return "";
	}
}

std::string modeO(Server &server, Client &client, Channel *channel, char action, std::istringstream &iss) {
	std::string targetNickname;
	if (!(iss >> targetNickname)) {
		return "ERROR";
	}
	Client *targetClient = NULL;
	for (size_t j = 0; j < channel->getMembers().size(); ++j) {
		if (channel->getMembers()[j]->getNickName() == targetNickname) {
			targetClient = channel->getMembers()[j];
			break;
		}
	}
	if (!targetClient) {
		server.sendMessageToClient(client.getFd(),
			ERR_USERNOTINCHANNEL(SERVER_NAME, client.getNickName(), targetNickname, channel->getName()));
		return "ERROR";
	}

	for (size_t k = 0; k < channel->getOperators().size(); ++k) {
		if (channel->getOperators()[k]->getNickName() == targetNickname) {
			if (action == '+') {
				return "IGNORE";
			}
			else {
				channel->removeOperator(*targetClient);
				return targetNickname;
			}
		}
	}
	if (action == '-') {
		return "IGNORE";
	}
	channel->addOperator(*targetClient);
	return targetNickname;
}

// ====================================================================================
// MODE COMMAND
void cmdMode(Server &server, Client &client, std::istringstream &iss) {
	std::string channelName;
	if (!(iss >> channelName)) {
		server.sendMessageToClient(client.getFd(),
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), 
			"MODE #channel [+/-modes] [params] \n +/-i = invite only\n +/-t = topic restricted\n +/-k [key] = password protected\n +/-l [limit] = user limit\n +/-o [nickname] = give/take operator status"));
		return;
	}

	// IRSSI or Weechat start sending MODE <nickname> 
	if (channelName == client.getNickName()) 
		return;

	if (client.getIsRegistered() == false) {
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (channelName[0] != '#') {
		server.sendMessageToClient(client.getFd(),
			ERR_NOSUCHCHANNEL(SERVER_NAME, client.getNickName(), channelName));
		return;
	}
	else
		channelName = channelName.substr(1);

	if (channelName.empty()) {
		server.sendMessageToClient(client.getFd(), 
			ERR_NEEDMOREPARAMS(SERVER_NAME, client.getNickName(), "MODE"));
		return;
	}

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

	if (!channel->isOperator(client)) {
		server.sendMessageToClient(client.getFd(),
			ERR_CHANOPRIVSNEEDED(SERVER_NAME, client.getNickName(), channelName));
		return;
	}

	std::string token;
	std::string modes;
	std::vector<std::string> params;

	if (!(iss >> token)) {
		server.sendMessageToClient(client.getFd(),
			RPL_CHANNELMODEIS(
				SERVER_NAME,
				client.getNickName(),
				channelName,
				channel->getModesStr(),
				channel->getModeParamsStr()
			)
		);
		return;
	}

	char action = 0;
	do {
		if (token[0] == '+' || token[0] == '-') {
			action = token[0];
			modes += action;
		}

		for (size_t i = 1; i < token.size(); ++i) {
			char m = token[i];
			std::string res;


			switch (m) {
				case 'i': res = modeI(channel, action); break;
				case 't': res = modeT(channel, action); break;
				case 'k': res = modeK(channel, action, iss); break;
				case 'l': res = modeL(channel, action, iss); break;
				case 'o': res = modeO(server, client, channel, action, iss); break;
				default:
					server.sendMessageToClient(client.getFd(),
						ERR_UNKNOWNMODE(SERVER_NAME, client.getNickName(), std::string(1, m))
					);
					continue;
			}

			if (res == "ERROR" || res == "IGNORE") continue;

			modes += m;
			if (!res.empty())
				params.push_back(res);

		}

	} while (iss >> token);

	if (!modes.empty()) {

		if (!params.empty()) {
			std::string str;
			for (size_t i = 0; i < params.size(); i++)
				str += params[i] + ' ';

			channel->broadcastToMembers(
			RPL_CHANGEMODE(client.getNickName(), client.getUserName(), client.getHostName(), channelName, modes, str),
			-1);
			return;
		}
		channel->broadcastToMembers(
			RPL_CHANGEMODE(client.getNickName(), client.getUserName(), client.getHostName(), channelName, modes, ""),
			-1);
	}
}
