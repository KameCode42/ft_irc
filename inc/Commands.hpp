#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Client.hpp"
#include "Messages.hpp"

class Server;

void	cmdCap(Server& server, Client& client, std::istringstream& iss);
void	cmdInvite(Server& server, Client& client, std::istringstream& iss);
void	cmdPass(Server& server, Client& client, std::istringstream& iss);
void	cmdPart(Server& server, Client& client, std::istringstream& iss);
void	cmdNick(Server& server, Client& client, std::istringstream& iss);
void	cmdUser(Server& server, Client& client, std::istringstream& iss);
void	cmdQuit(Server& server, Client& client, std::istringstream& iss);
void	cmdPing(Server& server, Client& client, std::istringstream& iss);
void	cmdJoin(Server& server, Client& client, std::istringstream& iss);
void	cmdPrvmsg(Server& server, Client& client, std::istringstream& iss);
void	cmdMode(Server& server, Client& client, std::istringstream& iss);
void	cmdKick(Server& server, Client& client, std::istringstream& iss);
void	cmdTopic(Server& server, Client& client, std::istringstream& iss);
void	parseCommand(Server& server, Client& client, std::string& message);

#endif
