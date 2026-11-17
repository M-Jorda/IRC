#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

// Le serveur : garde le port et le mot de passe, ouvre le socket d'écoute (start)
// et fait tourner la boucle poll (run). Possède tous les fd et les ferme lui-même :
// il ne doit donc jamais être copié.
class Server
{
public:
	Server(int port, const std::string& password);
	~Server();

	const std::string&	getPassword() const;

private:
	int			_port;
	std::string	_password;

	// Copie interdite (C++98) : déclarés, jamais définis.
	Server(const Server& other);
	Server&	operator=(const Server& other);
};

#endif
