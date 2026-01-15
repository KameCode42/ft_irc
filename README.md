# ft_irc

## 🧠 Projet 42 – ft_irc

**ft_irc** est un projet réseau de l'école 42 qui consiste à créer un serveur IRC (Internet Relay Chat) en C++, conforme à la RFC1459 (et d'autres variantes pour les bonus).

Le serveur accepte plusieurs connexions clients et permet aux utilisateurs de communiquer via des channels, de s’échanger des messages privés, et de gérer des permissions/modérateurs.

---

## 🚀 Objectifs du projet

- Implémenter un serveur IRC complet en C++
- Utiliser des sockets bas niveau (TCP)
- Gérer plusieurs clients via `poll()` ou `select()`
- Supporter les commandes IRC classiques : `JOIN`, `PART`, `NICK`, `USER`, `PRIVMSG`, etc.
- Respecter les réponses et codes définis par la RFC
- (Bonus) Ajouter le support SSL, des bots ou la connexion multi-serveurs

---

## 🧩 Répartition du travail

Le projet est divisé en **3 blocs principaux** :

### 🧱 Bloc A – Serveur & Connexions (Responsable : Dev 1)
- Setup du projet (Makefile, architecture)
- Gestion du socket serveur : `bind`, `listen`, `accept`
- Boucle principale avec `poll()` ou `select()`
- Suivi des clients connectés
- Fermeture propre, gestion des timeouts

### 💬 Bloc B – Commandes & Channels (Responsable : Dev 2)
- Parsing et exécution des commandes IRC :
  - `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `QUIT`, `NOTICE`, `MODE`, `TOPIC`
- Gestion des structures `Client` et `Channel`
- Gestion des permissions (opérateur, kick, ban, invite)
- Gestion des modes de channels et d’utilisateurs

### 🤝 Bloc C – En commun
> Ces parties seront développées ensemble pour assurer la cohérence globale du serveur.

- Authentification (`PASS`, `NICK`, `USER`) avec vérifications
- Réponses formatées RFC (codes, messages, erreurs)
- Gestion des erreurs (`ERR_UNKNOWNCOMMAND`, `ERR_NICKNAMEINUSE`, etc.)
- Tests avec clients réels (WeeChat, irssi, netcat)
- Intégration finale et bonus éventuels

---

## 🧪 Commandes supportées

| Commande | Description |
|----------|-------------|
| `PASS` | Authentification par mot de passe |
| `NICK` | Définition du pseudonyme |
| `USER` | Informations utilisateur |
| `JOIN` / `PART` | Rejoindre / quitter un channel |
| `PRIVMSG` / `NOTICE` | Envoi de messages |
| `QUIT` | Déconnexion |
| `MODE` | Gestion des modes utilisateur/channel |
| `TOPIC` | Sujet du channel |
| `PING` / `PONG` | Keep-alive |

---

## ⚙️ Compilation

```bash
make
