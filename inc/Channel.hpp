#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"
#include <vector>
#include <string>
#include "Client.hpp"

class Client;

class	Channel
{
	public:
		Channel(std::string name);
		Channel(const std::string& name, Client& creator);
		~Channel();

		std::string					getName()const;
		const std::vector<Client*>&	getClients()const;

		void						addClient(Client* client);
		void						removeClient(Client* client);
		bool						hasClient(Client* client)const;
		bool						isEmpty()const;

		const std::string&			getTopic() const;
		void						setTopic(const std::string& topic);
		// === MEMBERS ===
		void						addMember(Client& client);
		void						removeMember(Client& client);
		bool						isMember(const Client& client) const;

		void						addOperator(Client& client);
		void						removeOperator(Client& client);
		bool						isOperator(const Client& client) const;

		void						addInvitedClient(Client& client);
		void						removeInvitedClient(Client& client);
		bool						isInvited(const Client& client) const;
		
		size_t						getMemberCount() const;
		std::vector<Client*>		getMembers() const;
		std::vector<Client*>		getOperators() const;

		// === MODES ===
		bool						isInviteOnly() const;
		void						setInviteOnly(bool status);
		bool						isTopicRestricted() const;
		void						setTopicRestricted(bool status);
		bool						isPasswordProtected() const;
		void						setPasswordProtected(bool status);
		const std::string&			getPassword() const;
		void						setPassword(const std::string& password);
		bool						isOperatorOnly() const;
		void						setOperatorOnly(bool status);
		size_t						getUserLimit() const;
		void						setUserLimit(size_t limit);
		std::string					getModesStr() const;
		std::string					getModeParamsStr() const;

		// === COMMUNICATION ===
		void						broadcastToMembers(const std::string& message, int excludeFd);

	private:
		std::string					_name;
		std::vector<Client*>		_clients;

		std::string					_topic;
		std::vector<Client*>		_operators;
		std::vector<Client*>		_members;
		std::vector<Client*>		_invited;
		std::string					_password;

		bool						_inviteOnly;
		bool						_topicRestricted;
		bool						_passwordProtected;
		bool						_operatorOnly;
		size_t						_userLimit;
};

#endif

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
class:
_name				->	Nom du channel (#test)
_clients			->	Client membres du channel
_topic				->	topic du channel
_operators			->	Clients operateurs du channel
_members			->	Clients membres du channel
_invited			->	Clients invites du channel
_password			->	Mot de passe du channel
_inviteOnly			->	Mode +i
_topicRestricted	->	Mode +t
_passwordProtected	->	Mode +k
_operatorOnly		->	Mode +o
_userLimit			->	Mode +l
=============================================================
*/
