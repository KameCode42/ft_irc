#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include <string>

class Client;

class	Channel
{
	public:
		Channel(std::string name);
		~Channel();

		std::string					getName()const;
		const std::vector<Client*>&	getClients()const;

		void						addClient(Client* client);
		void						removeClient(Client* client);
		bool						hasClient(Client* client)const;
		bool						isEmpty()const;

	private:
		std::string					_name;
		std::vector<Client*>		_clients;
};

#endif

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
class:
_name			->	Nom du channel (#test)
_clients		->	Client membres du channel
=============================================================
*/
