#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include "Client.hpp"
#include <string>

#define EOL "\r\n"

//RPL (Replies standard)
#define RPL_CONNECTED_FIRST(server, nickname)							(std::string(":") + server + " 001 " + nickname + " :Welcome to the IRC server!" + EOL)
#define RPL_CHANNELMODES(server, nickname, channelname, modes)			(std::string(":") + server + " 324 " + nickname + " #" + channelname + " " + modes + EOL)
#define RPL_CREATIONTIME(server, nickname, channelname, creationtime)	(std::string(":") + server + " 329 " + nickname + " #" + channelname + " " + creationtime + EOL)
#define RPL_TOPICIS(server, nickname, channelname, topic)				(std::string(":") + server + " 332 " + nickname + " #" + channelname + " :" + topic + EOL)
#define RPL_NAMREPLY(server, nickname, channelname, clientslist)		(std::string(":") + server + " 353 " + nickname + " = #" + channelname + " :" + clientslist + EOL)
#define RPL_ENDOFNAMES(server, nickname, channelname)					(std::string(":") + server + " 366 " + nickname + " #" + channelname + " :End of /NAMES list" + EOL)
#define RPL_CHANGEMODE(server, nickname, channelname, mode, arguments)	(std::string(":") + server + " MODE #" + channelname + " " + mode + " " + arguments + EOL)
#define RPL_NICKCHANGE(oldnickname, nickname)							(std::string(":") + oldnickname + " NICK " + nickname + EOL)
#define RPL_JOINMSG(nickname, username, hostname, channelname)			(std::string(":") + nickname + "!" + username + "@" + hostname + " JOIN #" + channelname + EOL)
#define RPL_QUITMSG(nickname, user, host, reason)						(std::string(":") + nickname + "!" + user + "@" + host + " QUIT :" + reason + EOL)
#define RPL_PONG(server, token)											(std::string(":") + server + " PONG :" + token + EOL)

/*==================================================================================*/

//ERR (Erreurs standard)
#define ERR_NOSUCHNICK(server, nickname, name)							(std::string(":") + server + " 401 " + nickname + " " + name + " :No such nick/channel" + EOL)
#define ERR_CHANNELNOTFOUND(server, nickname, channelname)				(std::string(":") + server + " 403 " + nickname + " " + channelname + " :No such channel" + EOL)
#define ERR_NOORIGIN(server, nickname)									(std::string(":") + server + " 409 " + nickname + " :No origin specified" + EOL)
#define ERR_CMDNOTFOUND(server, nickname, command)						(std::string(":") + server + " 421 " + nickname + " " + command + " :Unknown command" + EOL)
#define ERR_NONICKNAMEGIVEN(server, nickname)							(std::string(":") + server + " 431 " + nickname + " :No nickname given" + EOL)
#define ERR_ERRONEUSNICK(server, nickname)								(std::string(":") + server + " 432 " + nickname + " :Erroneus nickname" + EOL)
#define ERR_NICKINUSE(server, nickname, nickused)						(std::string(":") + server + " 433 " + nickname + " " + nickused + " :Nickname is already in use" + EOL)
#define ERR_NOTREGISTERED(server, nickname)								(std::string(":") + server + " 451 " + nickname + " :You have not registered!" + EOL)
#define ERR_NOTENOUGHPARAM(server, nickname, command)					(std::string(":") + server + " 461 " + nickname + " " + command + " :Not enough parameters." + EOL)
#define ERR_ALREADYREGISTERED(server, nickname)							(std::string(":") + server + " 462 " + nickname + " :You may not reregister!" + EOL)
#define ERR_PASSWDMISMATCH(server, nickname)							(std::string(":") + server + " 464 " + nickname + " :Password incorrect!" + EOL)
#define ERR_KEYSET(server, nickname, channelname)						(std::string(":") + server + " 467 " + nickname + " #" + channelname + " :Channel key already set." + EOL)
#define ERR_UNKNOWNMODE(server, nickname, channelname, mode)			(std::string(":") + server + " 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + EOL)
#define ERR_NOTOPERATOR(server, nickname, channelname)					(std::string(":") + server + " 482 " + nickname + " #" + channelname + " :You're not a channel operator" + EOL)
#define ERR_NEEDMODEPARAM(server, nickname, channelname, mode)			(std::string(":") + server + " 696 " + nickname + " #" + channelname + " :You must specify a parameter for mode " + mode + EOL)
#define ERR_INVALIDMODEPARAM(server, nickname, channelname, mode)		(std::string(":") + server + " 696 " + nickname + " #" + channelname + " :Invalid mode parameter " + mode + EOL)

#endif

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
RPL (Replies standard): reponse normale du serveur a une commande valide
001 -> Message de bienvenue
324 -> Modes du channel
329 -> Date de création du channel
332 -> Sujet du channel
353 -> Liste des membres
366 -> Fin de la liste des noms
301 -> Changement de mode
305 -> Changement de nickname
OIN / QUIT (messages d’événement, pas de code numérique)
=============================================================

=============================================================
ERR (Erreurs standard): reponse d'erreur
401 -> Aucun nick ou channel trouvé
403 -> Channel introuvable
409 -> aucun parametre (ping/pong)
421 -> Commande inconnue
431 -> Aucun nickname fourni
432 -> Nickname invalide
433 -> Nickname déjà utilisé
451 -> Non enregistré
461 -> Paramètres insuffisants
462 -> Déjà enregistré
464 -> Mauvais mot de passe
467 -> Clé de channel déjà définie
472 -> Mode inconnu
482 -> Pas opérateur
696 -> Erreur de paramètre de mode
=============================================================

=============================================================
Exemple commande:
[:prefix] COMMAND params :trailing
Dans IRSSI --> /rawlog open raw.txt
<< CAP LS
<< PASS alex
<< NICK axds
<< USER axds 0 * :axds
>> :ft_irc 001 axds :Welcome to the IRC server!
<< MODE axds +i
>> : 421 * MODE :Unknown command
<< PING ft_irc
>> : 421 * PING :Unknown command
<< CAP LS
<< PASS alex
<< NICK axds
<< USER axds axds 127.0.0.1 ft_irc:axds
>> :ft_irc 001 axds :Welcome to the IRC server!
<< MODE axds +i
>> : 421 * MODE :Unknown command
<< PING ft_irc
>> : 421 * PING :Unknown command
=============================================================

=============================================================
Connection weechat:
/server add monirc 127.0.0.1/6667 -password=alice -tls=no
/connect monirc
/server del monirc = supprimer server
=============================================================
*/
