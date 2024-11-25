#pragma once

# include "Server.hpp"
# define DEBUG 0


// todo: check limited client number
class Client {
	private:
		int			_clientFd; // todo: size_t
		std::string	_clientNick;
		std::string	_clientUserName;
		bool		_authenticated;
		//todo: add attribute of participating channels
		std::string _correctPassword;

	public:
		// Client(void);
		Client(int fd, const std::string& correctPassword);
		// Client(const Client &other);
		// Client &operator=(const Client &other);
		~Client();

		// Getters
		int getFd() const;
		std::string getNick() const;
		std::string getUser() const;

		// Setters
		void setNick(const std::string& nick);
		void setUser(const std::string& user);

		// Methods
		bool isAuthenticated() const;
		void handleRead();
	
	private:
		bool checkPassCommand(const std::string& message);
};

std::map<int, Client*> connections;

void printAsciiDecimal(const std::string& str);

// #include "Client.hpp"

// ++++Constructor
// Client::Client(void) {
// 	if (DEBUG) {
// 		std::cout << "Client default constructor called" << std::endl;
// 	}
// }

// ++++Constructor
Client::Client(int fd, const std::string& correctPassword) 
					: _clientFd(fd), _authenticated(false), _correctPassword(correctPassword) {
	if (DEBUG) {
		std::cout << "Client parameter constructor called" << std::endl;
	}
}

// // Copy Constructor
// Client::Client(const Client &other) : "_variable(other._variable)"{
// 	if (DEBUG) {
// 		std::cout << "Client copy constructor called" << std::endl;
// 	}
// }

// // Assignment Operator
// Client &Client::operator=(const Client &other) {
// 	if (DEBUG) {
// 		std::cout << "Client Assignment Operator called" << std::endl;
// 	}
// 	if (this != &other) {
// 		// Copy data from other to this
// 	}
// 	return *this;
// }

// ----Destructor
Client::~Client() {
	if (DEBUG) {
		std::cout << "Client destructor called" << std::endl;
	}
	close(_clientFd);
	std::cout << "Client disconnected -> fd: " << _clientFd << std::endl;
}


// Getters
int Client::getFd() const {
	return _clientFd;
}

std::string Client::getNick() const {
	return _clientNick;
}

std::string Client::getUser() const {
	return _clientUserName;
}

// Setters
void Client::setNick(const std::string& nickName) {
	_clientNick = nickName;
}

void Client::setUser(const std::string& userName) {
	_clientUserName = userName;
}

// Methods
bool Client::isAuthenticated() const {
	return _authenticated;
}


void printAsciiDecimal(const std::string& str) {
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		printf("%d " , (int)str[i]);
	}
	printf("\n");
	
}

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of("\t\n\r");
	size_t last = str.find_last_not_of("\t\n\r");
	if (first == std::string::npos || last == std::string::npos) {
		return "";
	} else {
		return str.substr(first, last - first + 1);
	}
}

void Client::handleRead() {
	char buffer[1024] = {0};
	int bytesReceived = recv(_clientFd, buffer, sizeof(buffer), 0);

	if (bytesReceived == 0) {
		std::cout << "Client desconnected -> fd: " << _clientFd << std::endl;
	} else if (bytesReceived < 0) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			throw std::runtime_error("Error on recv");
		}
	} else {
		// Process message
		std::string message(buffer, bytesReceived);
		std::cout << "Received message from fd " << _clientFd << ": "<< message << std::endl;

		std::istringstream stream(message);
		std::string line;
		while (std::getline(stream, line)) {
			// std::cout << "LINE: " << line << std::endl;
			// Check for PASS command
			if (checkPassCommand(line)) {
				std::cout << "PASS command received" << std::endl;
				std::string password = line.substr(5);
				password = trim(password);
				// std::cout << "R password: " << password << std::endl;
				// std::cout << "R password in ASCII decimal: ";
				// printAsciiDecimal(password);
				// std::cout << "E password: " << _correctPassword << std::endl;
				// std::cout << "E password in ASCII decimal: ";
				// printAsciiDecimal(_correctPassword);
				if (password == _correctPassword) {
					_authenticated = true;
					std::cout << "Client authenticated -> fd: " << _clientFd << std::endl;
				} else {
					// std::cout << "Password incorrect" << std::endl;
					std::string response = ircErrorMessages[464];
					std::cout << response << std::endl;
					send(_clientFd, response.c_str(), response.size(), 0);
					close(_clientFd);
					return ;
				}
			}
			else if (line.substr(0, 4) == "NICK") {
				std::string nick = line.substr(5);
				setNick(trim(nick));
				std::cout << "NICK command received. Client nickname: " << _clientNick << std::endl;
			} else if (line.substr(0, 4) == "USER") {
				std::string user = line.substr(5);
				setUser(trim(user));
				std::cout << "USER command received. Client username: " << _clientUserName << std::endl;
			}
		}

		// Send answer to client
		std::string response = "Message received\n";
		send(_clientFd, response.c_str(), response.size(), 0);
	}
}

// Function to check for the PASS command
bool Client::checkPassCommand(const std::string& message) {
	const std::string passCommand = "PASS";
	std::string line;
	std::stringstream ss(message);
	if (message.compare(0, passCommand.length(), passCommand) == 0) {
		return true;
	}
	return false;
}
