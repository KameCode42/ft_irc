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

	if (client.getIsRegistered() == false)
	{
		server.sendMessageToClient(client.getFd(),
			ERR_NOTREGISTERED(SERVER_NAME, client.getNickName()));
		return;
	}

	if (nameChannel.empty() || nameChannel[0] != '#')
	{
		server.sendMessageToClient(client.getFd(),
			ERR_NOTENOUGHPARAM(SERVER_NAME, client.getNickName(), "JOIN"));
		return;
	}

	for (size_t i = 1; i < nameChannel.length(); i++)
	{
		if (nameChannel[i] == ' ' || nameChannel.length() < 2)
		{
			server.sendMessageToClient(client.getFd(),
				ERR_CHANNELNOTFOUND(SERVER_NAME, client.getNickName(), nameChannel));
		return;
		}
	}

	Channel* channel = NULL;

	if (server._channels.find(nameChannel) != server._channels.end())
	{
		channel = &server._channels[nameChannel];
	}
	else
	{
		server._channels.insert(std::make_pair(nameChannel, Channel(nameChannel)));
		channel = &server._channels[nameChannel];
	}

	if (!channel->hasClient(&client))
		channel->addClient(&client);


	channel.getClients();

	
}
