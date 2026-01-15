#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include "Client.hpp"
#include <string>

#define EOL "\r\n"

//RPL (Replies standard)
#define RPL_WELCOME(server, nick)										(std::string(":") + server + " 001 " + nick + " :Welcome to the IRC server!" + EOL)
#define RPL_UMODEIS(server, nick, modes)								(std::string(":") + server + " 221 " + nick + " " + modes + EOL)
#define RPL_CHANNELMODEIS(server, nick, channel, modes, params)			(std::string(":") + server + " 324 " + nick + " #" + channel + " " + modes + " " + params + EOL)
#define RPL_CREATIONTIME(server, nick, channel, creationtime)			(std::string(":") + server + " 329 " + nick + " #" + channel + " " + creationtime + EOL)
#define RPL_NOTOPIC(server, nick, channel)								(std::string(":") + server + " 331 " + nick + " #" + channel + " :No topic is set" + EOL)
#define RPL_TOPICIS(server, nick, channel, topic)						(std::string(":") + server + " 332 " + nick + " #" + channel + " :" + topic + EOL)
#define RPL_NAMREPLY(server, nick, channel, clientslist)				(std::string(":") + server + " 353 " + nick + " = #" + channel + " :" + clientslist + EOL)
#define RPL_ENDOFNAMES(server, nick, channel)							(std::string(":") + server + " 366 " + nick + " #" + channel + " :End of /NAMES list" + EOL)

#define RPL_PONG(server, token)											(std::string(":") + server + " PONG :" + token + EOL)
#define RPL_INVITING(nick, user, host, target, channel)					(std::string(":") + nick + "!" + user + "@" + host + " INVITE " + target + " #" + channel + EOL)
#define RPL_PRVMSG_CHANNEL(nick, user, host, channel, msg)				(std::string(":") + nick + "!" + user + "@" + host + " PRIVMSG #" + channel + " :" + msg + EOL)
#define RPL_PRVMSG_USER(nick, user, host, target, message)				(std::string(":") + nick + "!" + user + "@" + host + " PRIVMSG " + target + " :" + message + EOL)
#define RPL_CHANGEMODE(nick, user, host, channel, mode, args)			(std::string(":") + nick + "!" + user + "@" + host + " MODE #" + channel + " " + mode + " " + args + EOL)
#define RPL_NICKCHANGE(oldnick, user, host, nick)						(std::string(":") + oldnick + "!" + user + "@" + host + " NICK " + nick + EOL)
#define RPL_JOINMSG(nick, user, host, channel)							(std::string(":") + nick + "!" + user + "@" + host + " JOIN #" + channel + EOL)
#define RPL_PARTMSG(nick, user, host, channel, reason)					(std::string(":") + nick + "!" + user + "@" + host + " PART #" + channel + " " + reason + EOL)
#define RPL_QUITMSG(nick, user, host, reason)							(std::string(":") + nick + "!" + user + "@" + host + " QUIT :" + reason + EOL)
/*==================================================================================*/

//ERR (Erreurs standard)
#define ERR_NOSUCHNICK(server, nick, name) 								(std::string(":") + server + " 401 " + nick + " " + name + " :No such nick/channel" + EOL)
#define ERR_NOSUCHCHANNEL(server, nick, channel) 						(std::string(":") + server + " 403 " + nick + " " + channel + " :No such channel" + EOL)
#define ERR_CANNOTSENDTOCHAN(server, nick, channel) 					(std::string(":") + server + " 404 " + nick + " " + channel + " :Cannot send to channel" + EOL)
#define ERR_NOORIGIN(server, nick) 										(std::string(":") + server + " 409 " + nick + " :No origin specified" + EOL)
#define ERR_NOTEXTTOSEND(server, nick) 									(std::string(":") + server + " 412 " + nick + " :No text to send" + EOL)
#define ERR_UNKNOWNCOMMAND(server, nick, command) 						(std::string(":") + server + " 421 " + nick + " " + command + " :Unknown command" + EOL)
#define ERR_NONICKNAMEGIVEN(server, nick) 								(std::string(":") + server + " 431 " + nick + " :No nick given" + EOL)
#define ERR_ERRONEUSNICKNAME(server, nick)								(std::string(":") + server + " 432 " + nick + " :Erroneous nick" + EOL)
#define ERR_NICKNAMEINUSE(server, nick, nickused)						(std::string(":") + server + " 433 " + nick + " " + nickused + " :nick is already in use" + EOL)
#define ERR_USERNOTINCHANNEL(server, nick, user, channel) 				(std::string(":") + server + " 441 " + nick + " " + user + " " + channel + " :They aren't on that channel" + EOL)
#define ERR_NOTONCHANNEL(server, nick, channel) 						(std::string(":") + server + " 442 " + nick + " " + channel + " :You're not on that channel" + EOL)
#define ERR_ALREADYINCHANNEL(server, nick, channel)						(std::string(":") + server + " 443 " + nick + " #" + channel + " :is already on channel" + EOL)
#define ERR_USERONCHANNEL(server, nick, user, channel) 					(std::string(":") + server + " 443 " + nick + " " + user + " " + channel + " :is already on channel" + EOL)
#define ERR_NOTREGISTERED(server, nick) 								(std::string(":") + server + " 451 " + nick + " :You have not registered" + EOL)
#define ERR_NEEDMOREPARAMS(server, nick, command) 						(std::string(":") + server + " 461 " + nick + " " + command + " :Not enough parameters" + EOL)
#define ERR_TOOMANYPARAMS(server, nick, command) 						(std::string(":") + server + " 467 " + nick + " " + command + " :Too many parameters" + EOL)
#define ERR_ALREADYREGISTERED(server, nick) 							(std::string(":") + server + " 462 " + nick + " :You may not reregister" + EOL)
#define ERR_PASSWDMISMATCH(server, nick) 								(std::string(":") + server + " 464 " + nick + " :Password incorrect" + EOL)
#define ERR_KEYSET(server, nick, channel) 								(std::string(":") + server + " 467 " + nick + " " + channel + " :Channel key already set" + EOL)
#define ERR_CHANNELISFULL(server, nick, channel) 						(std::string(":") + server + " 471 " + nick + " " + channel + " :Cannot join channel (+l)" + EOL)
#define ERR_UNKNOWNMODE(server, nick, mode) 							(std::string(":") + server + " 472 " + nick + " " + mode + " :is unknown mode char to me" + EOL)
#define ERR_INVITEONLYCHAN(server, nick) 								(std::string(":") + server + " 473 " + nick + " :Cannot join channel (+i)" + EOL)
#define ERR_BADCHANNELKEY(server, nick, channel)						(std::string(":") + server + " 475 " + nick + " " + channel + " :Cannot join channel (+k)" + EOL)
#define ERR_CHANOPRIVSNEEDED(server, nick, channel) 					(std::string(":") + server + " 482 " + nick + " " + channel + " :You're not channel operator" + EOL)
#define ERR_UMODEUNKNOWNFLAG(server, nick) 								(std::string(":") + server + " 501 " + nick + " :Unknown MODE flag" + EOL)
#define ERR_USERSDONTMATCH(server, nick) 								(std::string(":") + server + " 502 " + nick + " :Cannot change mode for other users" + EOL)
#define ERR_CHANNELNAMENOTALLOWED(server, nick, channel)				(std::string(":") + server + " 479 " + nick + " " + channel + " :Forbidden" + EOL)

/*==================================================================================*/

//MESSAGES SERVER
#define CLIENT_REGISTERED(clientfd, nick)								std::cout << BLUE << "Client <" << clientfd << "> : " << nick << " is registered" << RESET << std::endl;
#define CLIENT_JOIN(clientfd, nick, channel)							std::cout << BLUE << "Client <" << clientfd << "> : " << nick << " has joined the channel #" << channel << RESET << std::endl;
#define CLIENT_QUIT_CHANNEL(clientfd, nick, channel)					std::cout << BLUE << "Client <" << clientfd << "> : " << nick << " has left the channel " << channel << RESET << std::endl;
#define REMOVE_CHANNEL(channel)											std::cout << BLUE << "[CHANNEL] " << channel << " is now empty → removing channel" << RESET << std::endl;

#endif

/*==================================================================================*/
/*==================================================================================*/

/*=== Note ===*/
/*
=============================================================
RPL (Replies standard): reponse normale du serveur a une commande valide
001 -> Message de bienvenue
221 -> modes utilisateur actuellement actifs pour ton pseudo
324 -> Modes du channel
329 -> Date de création du channel
331 -> Aucun topic defini pour le canal
332 -> Sujet du channel
353 -> Liste des membres
366 -> Fin de la liste des noms
301 -> Changement de mode
305 -> Changement de nick
JOIN / QUIT (messages d’événement, pas de code numérique)
=============================================================

=============================================================
ERR (Erreurs standard): reponse d'erreur
401 -> Aucun nick ou channel trouvé
403 -> Channel introuvable
404 -> Impossible d'envoyer sur le canal
409 -> aucun parametre (ping/pong)
412 -> Aucun message a envoyer
421 -> Commande inconnue
431 -> Aucun nick fourni
432 -> nick invalide
433 -> nick déjà utilisé
441 -> Utilisateur pas dans le channel
442 -> Non membre du channel
451 -> Non enregistré
461 -> Paramètres insuffisants
462 -> Déjà enregistré
464 -> Mauvais mot de passe
467 -> Clé de channel déjà définie
471 -> Channel plein
472 -> Mode inconnu
473 -> Channel sur invitation
475 -> Mauvais mdp du channel
482 -> Pas opérateur
501 -> Mode utilisateur inconnu
502 -> tente de changer les modes d'un autre user
=============================================================
*/
