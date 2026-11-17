#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

// Port strict : uniquement des chiffres (ni espace, ni signe), 5 au plus, puis 1–65535.
// Le texte est vérifié AVANT la conversion : avec 5 chiffres au plus, aucun débordement possible.
static bool parsePort(const char* arg, int& port)
{
	std::string	text(arg);

	if (text.empty() || text.size() > 5)
		return false;
	for (std::string::size_type i = 0; i < text.size(); ++i)
		if (!std::isdigit(static_cast<unsigned char>(text[i])))
			return false;

	std::istringstream	iss(text);
	iss >> port;
	return port >= 1 && port <= 65535;
}

// Mot de passe (D1) : non vide, sans espace ni caractère de contrôle, pas de longueur max.
static bool isValidPassword(const std::string& password)
{
	if (password.empty())
		return false;
	for (std::string::size_type i = 0; i < password.size(); ++i)
	{
		unsigned char	c = static_cast<unsigned char>(password[i]);
		if (std::isspace(c) || std::iscntrl(c))
			return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int	port;
	if (!parsePort(argv[1], port))
	{
		std::cerr << "Error: invalid port '" << argv[1]
				  << "': expected an integer between 1 and 65535" << std::endl;
		return 1;
	}

	std::string	password(argv[2]);
	if (!isValidPassword(password))
	{
		std::cerr << "Error: invalid password: must not be empty or contain spaces or control characters"
				  << std::endl;
		return 1;
	}

	return 0;
}
