# ft_irc — Répartition du travail

Proposition de découpage du projet en deux moitiés, avec une interface simple entre elles.
**À relire et valider ensemble avant de commencer à coder** (voir « Questions à trancher » en bas).
Les décisions prises depuis, et ce qu'elles changent pour l'autre moitié, sont dans [DECISIONS.md](DECISIONS.md).

## Le principe

On coupe à la frontière **réseau / protocole IRC** :

- la **moitié A** transporte des octets : elle ne sait pas ce qu'est une commande IRC ;
- la **moitié B** comprend IRC : elle ne touche jamais à un socket ni à `poll()`.

Chacun peut tester sa moitié seul (A avec `nc`, B en lui donnant des lignes de texte), et on travaille dans des dossiers séparés, donc peu de conflits git.

## Moitié A — Serveur réseau (M-Jorda)

| # | Bloc | Contenu |
|---|---|---|
| A1 | Socle | Makefile (pas de relink, `-Wall -Wextra -Werror -std=c++98`), `main`, validation de `<port>` et `<password>`, squelette `Server` |
| A2 | Socket d'écoute | `socket`, `setsockopt(SO_REUSEADDR)`, `bind`, `listen`, `fcntl(O_NONBLOCK)` |
| A3 | Boucle `poll()` | **un seul** `poll` pour tout, `accept` des nouveaux clients, création des `Client` |
| A4 | Réception | `recv` uniquement sur `POLLIN`, buffer par client, reconstitution des paquets partiels, découpage sur `\r\n`, appel de `handleLine` |
| A5 | Envoi | `Client::queue`, buffer d'envoi, `POLLOUT`, `send` partiels |
| A6 | Cycle de vie | déconnexions (`recv` == 0, erreur, `POLLHUP`, demande de B), suppression d'un client sans casser la boucle |
| A7 | Robustesse & signaux | `SIGINT` / arrêt propre, `SIGPIPE`, taille max des buffers, jamais de `errno` après `recv`/`send`, zéro fuite |
| A8 | Intégration | brancher la vraie `handleLine` de B, tests avec le client de référence |

## Moitié B — Logique IRC (binôme)

| # | Bloc | Contenu |
|---|---|---|
| B1 | Parsing | une ligne → message : préfixe, commande, paramètres (dont le dernier après `:`) |
| B2 | Enregistrement | `PASS` / `NICK` / `USER`, vérification du mot de passe, messages de bienvenue (001…004) |
| B3 | Messages & canaux | `JOIN`, `PART`, `PRIVMSG` (utilisateur et canal), diffusion aux membres, `QUIT`, `PING`/`PONG` |
| B4 | Classe `Channel` | membres, opérateurs, topic, modes, clé, limite, liste d'invités |
| B5 | Commandes d'opérateur | `KICK`, `INVITE`, `TOPIC`, `MODE` avec `i`, `t`, `k`, `o`, `l` |
| B6 | Réponses numériques | codes d'erreur et de réponse attendus par le client de référence (RFC 2812) |

## L'interface entre A et B

```cpp
// A appelle B pour chaque ligne complète reçue (sans le "\r\n") :
void handleLine(Client& c, const std::string& line);

// B appelle A pour envoyer un message (A le met en tampon et l'envoie quand le socket est prêt) :
void Client::queue(const std::string& msg);   // msg déjà terminé par "\r\n"

// B demande à A de fermer une connexion (QUIT, mauvais mot de passe…) :
void Server::disconnect(Client& c);
```

Tant que B n'est pas prêt, A utilise une fausse `handleLine` qui renvoie la ligne (écho). Tant que A n'est pas prêt, B peut tester ses commandes avec un faux `Client` qui affiche ce qu'on lui envoie.

**Classe `Client`, partagée** : A y met le fd, l'adresse et les buffers de réception et d'envoi. B y met le nick, le user, l'état d'enregistrement et les canaux. On s'accorde sur le `.hpp` avant de coder.
A garantit qu'un `Client` **ne change jamais d'adresse mémoire** tant qu'il est connecté, pour que les canaux de B puissent garder des `Client*` (conteneur à choisir, voir Q-2 de [DECISIONS.md](DECISIONS.md)).

## Arborescence

```
Makefile
README.md
include/
  network/      # A : Server.hpp, Client.hpp, …
  irc/          # B
src/
  main.cpp
  network/      # A
  irc/          # B
obj/            # généré par le Makefile, ignoré par git
tests/          # scripts de test
docs/           # REPARTITION.md (qui fait quoi), DECISIONS.md (décisions A ↔ B)
```

Une classe = un `.hpp` + un `.cpp` du même nom. Chacun reste dans ses dossiers.

## À faire ensemble

- **README.md** (chap. V, en anglais) : 1re ligne en italique avec nos deux logins, sections Description, Instructions, Resources (dont l'usage de l'IA). Chacun rédige la partie sur sa moitié.
- **`Client.hpp`** : à définir ensemble (voir plus haut).
- **Bonus** (chap. VI : transfert de fichiers, bot) : seulement si le mandatory est parfait. Répartition à décider plus tard.
- **Soutenance** : chacun doit pouvoir expliquer **tout** le projet, on se présente nos moitiés une fois finies.

## Questions à trancher

Elles sont suivies, avec leur état, dans [DECISIONS.md](DECISIONS.md).

1. **Client de référence** : c'est lui qui sera utilisé en soutenance, et le serveur doit s'y connecter sans aucune erreur. Proposition : **irssi** (en terminal, très utilisé pour ft_irc, affiche les messages bruts). Alternatives : WeeChat, HexChat. Ton avis ?
2. **Le découpage A / B** te convient-il ?
3. **L'interface** (`handleLine`, `Client::queue`, `Server::disconnect`) : des changements ?
4. **L'arborescence** : d'accord ?
5. **Workflow git** : une branche par moitié avec merge sur `main`, ou commits directs sur `main` ?
