#include "Commands.hpp"
#include "Server.hpp"
#include "Client.hpp"

void	parseCommand(Server& server, Client& client, std::string& message)
{
	std::istringstream iss(message);
	std::string command;
	iss >> command;

	if (command.empty())
		return;

	for (size_t i = 0; i < command.size(); ++i)
		command[i] = std::toupper(command[i]);

	if (command == "CAP")
		cmdCap(server, client, iss);
	else if (command == "PASS")
		cmdPass(server, client, iss);
	else if (command == "NICK")
		cmdNick(server, client, iss);
	else if (command == "USER")
		cmdUser(server, client, iss);
	else if (command == "MODE")
		cmdMode(server, client, iss);
	else if (command == "KICK")
		cmdKick(server, client, iss);
	else if (command == "TOPIC")
		cmdTopic(server, client, iss);
	else if (command == "QUIT")
		cmdQuit(server, client, iss);
	else if (command == "PING")
		cmdPing(server, client, iss);
	else if (command == "INVITE")
		cmdInvite(server, client, iss);
	else if (command == "JOIN")
		cmdJoin(server, client, iss);
	else if (command == "PART")
		cmdPart(server, client, iss);
	else if (command == "PRIVMSG")
		cmdPrvmsg(server, client, iss);
	else if (command == "PONG")
		;
	else {
		server.sendMessageToClient(client.getFd(),
			ERR_UNKNOWNCOMMAND(SERVER_NAME, client.getNickName(), command));
	}
}

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
parseCommand:
Extrait le premier mot (ex : PASS, NICK, USER...)
Passe la commande en majuscule
Check quelle commande est passer
Envoie la bonne commande au server
=============================================================
*/
