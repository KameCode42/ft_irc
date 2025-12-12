#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Client.hpp"
#include "Messages.hpp"

class Server;

void	cmdPass(Server& server, Client& client, std::istringstream& iss);
void	cmdNick(Server& server, Client& client, std::istringstream& iss);
void	cmdUser(Server& server, Client& client, std::istringstream& iss);



void	parseCommand(Server& server, Client& client, std::string& message);

#endif
