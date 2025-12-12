#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <algorithm>
#include <string>
#include <stdexcept>

#define LINE_MAX 512
#define BUFFER_MAX 1024

class	Client
{
	public:
		Client();
		Client(int fd, std::string const& host);
		~Client();

		int			getFd()const;
		void		setFd(int fd);
		std::string	getPassWord()const;
		void		setPassWord(std::string const& passWord);
		std::string	getNickName()const;
		void		setNickName(std::string const& nickName);
		std::string	getUserName()const;
		void		setUserName(std::string const& userName);
		std::string	getHostName()const;
		void		setHostName(std::string const& hostName);
		std::string	getRealName()const;
		void		setRealName(std::string const& realName);
		std::string	getBuffer()const;
		void		setBuffer(std::string const& buffer);
		bool		getHasPass()const;
		void		setHasPass(bool status);
		bool		getHasNick()const;
		void		setHasNick(bool status);
		bool		getHasUser()const;
		void		setHasUser(bool status);
		bool		getIsRegistered()const;
		void		setIsRegistered(bool status);

		void		appendToBuffer(std::string const& data);
		bool		hasCompleteCommand()const;
		std::string	extractNextCommand();

	private:
		int			_fd;
		std::string	_passWord;
		std::string	_hostName;
		std::string	_nickName;
		std::string	_userName;
		std::string	_realName;
		std::string	_buffer;
		bool		_hasPass;
		bool		_hasNick;
		bool		_hasUser;
		bool		_isRegistered;
};

#endif

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
Header:
#include <arpa/inet.h> -> stocker ou afficher l’adresse IP
=============================================================

=============================================================
Define:
LINE_MAX 512 -> taille max commande IRC
BUFFER_MAX 1024 -> taille max du buffer
=============================================================

=============================================================
class:
_fd;			->	Descripteur de socket
_passWord;		->	mdp
_hostName;		->	Adresse IP ou host
_nickName;		->	Pseudo IRC (pseudonyme)
_userName;		->	Nom d'utilisateur
_realName;		->	Nom réel (USER)
_buffer;		->	Pour accumuler les messages fragmentés
_hasPass;		->	mdp ok
_hasNick;		->	nick ok
_hasUser;		->	user ok
_isRegistered;	->	Après NICK+USER
=============================================================
*/
