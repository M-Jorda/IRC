#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

// L'hôtel : garde le port et le mot de passe ; ouvrira la porte (île 2) et fera tourner poll (île 3).
// Possède les fd et les fermera lui-même : il ne doit donc jamais être copié.
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
