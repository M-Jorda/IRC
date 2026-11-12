# ft_irc — Décisions et points de contact A ↔ B

Ce fichier liste tout ce qu'une moitié décide **et qui touche l'autre**. On le complète au fil du projet, dans les
deux sens. Pour le découpage lui-même, voir [REPARTITION.md](REPARTITION.md).

Légende : ✅ décidé · ❓ à trancher · 🤝 à valider ensemble

## Décisions de A qui concernent B

### ✅ A-1 — Mot de passe du serveur
- `./ircserv <port> <password>` **refuse** au démarrage un mot de passe vide ou contenant des espaces ou des
  caractères de contrôle.
- **Ce que ça change pour B** : le mot de passe ne contient jamais d'espace. Dans `PASS`, la comparaison est une
  simple égalité de chaînes ; pas besoin de gérer le cas `PASS :mot de passe`.
- Le mot de passe sera accessible depuis `Server` (accesseur à définir, voir `Client.hpp` / `Server.hpp`).

### ✅ A-2 — Port
- `main` accepte un entier de 1 à 65535, rien d'autre. Un port < 1024 sans droits root : `bind` échoue et le
  serveur s'arrête avec un message clair.
- **Ce que ça change pour B** : rien.

### ✅ A-3 — IPv4 et IPv6
- Le serveur accepte les deux (un seul socket « double pile », repli sur IPv4 si la machine n'a pas d'IPv6).
- **Ce que ça change pour B** : l'adresse du client est fournie **sous forme de texte** (`std::string`) dans
  `Client`, par ex. `127.0.0.1` ou `::1`. B n'a jamais à manipuler de `sockaddr`. C'est cette chaîne qui sert de
  `host` dans le préfixe `nick!user@host`.

### ✅ A-4 — Écoute sur toutes les interfaces
- Un correcteur peut se connecter depuis une autre machine.
- **Ce que ça change pour B** : rien.

## Questions ouvertes

### ❓ Q-1 — Client de référence
- Proposition : **irssi**. Alternatives : WeeChat, HexChat. C'est lui qui sert en soutenance, et le serveur doit
  s'y connecter sans aucune erreur. Il fixe donc les réponses numériques que B doit envoyer.

### 🤝 Q-2 — Où sont rangés les `Client`
- A va choisir le conteneur des clients. Contrainte importante pour B : **si les canaux de B gardent des
  `Client*` ou `Client&`, ces adresses doivent rester valides** tant que le client est connecté.
- Ce qui est exclu : `std::vector<Client>` (les objets changent d'adresse quand le vecteur grandit).
- Options étudiées : `std::map<int, Client>` ou `std::map<int, Client*>`.
- **Question pour B** : dans `Channel`, tu comptes garder les membres comment (pointeurs, références, fd, nick) ?

### 🤝 Q-3 — Contenu de `Client.hpp`
- A y met : fd, adresse (texte), buffers de réception et d'envoi.
- B y met : nick, user, realname, état d'enregistrement, canaux…
- À écrire ensemble avant de coder, car c'est un contrat : le changer ensuite casse le code de l'autre.

### 🤝 Q-4 — Interface A ↔ B
```cpp
void handleLine(Client& c, const std::string& line);   // A → B : une ligne complète, sans "\r\n"
void Client::queue(const std::string& msg);            // B → A : msg déjà terminé par "\r\n"
void Server::disconnect(Client& c);                    // B → A : fermer la connexion
```
- **Attention pour B** : après `disconnect(c)`, ne plus utiliser `c` (A décidera du moment exact de la
  destruction ; à préciser).

### 🤝 Q-5 — Makefile
- Il est commun. Qui l'écrit ? Les fichiers sont-ils listés un par un (recommandé : beaucoup de correcteurs
  refusent les wildcards) ?

### 🤝 Q-6 — Arborescence et workflow git
- Arborescence proposée dans [REPARTITION.md](REPARTITION.md) (`include/network`, `src/network`, `include/irc`,
  `src/irc`).
- Une branche par moitié + merge sur `main`, ou commits directs sur `main` ?

## Décisions de B qui concernent A

_À compléter par B._

## Historique

| date | qui | quoi |
|---|---|---|
| 2026-11-11 | A | création du fichier ; décisions A-1 à A-4 ; questions Q-1 à Q-6 |
