#pragma once

# include "Server.hpp"
# define DEBUG 0

std::map<int, ConnectionHandler*> connections;

class ConnectionHandler {
	private:
		int			_clientFd;
		std::string	_clientNick;
		std::string	_clientUserName;
		bool		_authenticated;
		//todo: add attribute of participating channels

	public:
		// ConnectionHandler(void);
		ConnectionHandler(int fd);
		// ConnectionHandler(const ConnectionHandler &other);
		// ConnectionHandler &operator=(const ConnectionHandler &other);
		~ConnectionHandler();

		// Getters
		int getFd() const;

		// Methods
		bool isAuthenticated() const;
		void handleRead();
	
	private:
		bool checkPassCommand(const std::string& message);
};


// #include "ConnectionHandler.hpp"

// ++++Constructor
// ConnectionHandler::ConnectionHandler(void) {
// 	if (DEBUG) {
// 		std::cout << "ConnectionHandler default constructor called" << std::endl;
// 	}
// }

// ++++Constructor
ConnectionHandler::ConnectionHandler(int fd) : _clientFd(fd), _authenticated(false) {
	if (DEBUG) {
		std::cout << "ConnectionHandler parameter constructor called" << std::endl;
	}
}

// // Copy Constructor
// ConnectionHandler::ConnectionHandler(const ConnectionHandler &other) : "_variable(other._variable)"{
// 	if (DEBUG) {
// 		std::cout << "ConnectionHandler copy constructor called" << std::endl;
// 	}
// }

// // Assignment Operator
// ConnectionHandler &ConnectionHandler::operator=(const ConnectionHandler &other) {
// 	if (DEBUG) {
// 		std::cout << "ConnectionHandler Assignment Operator called" << std::endl;
// 	}
// 	if (this != &other) {
// 		// Copy data from other to this
// 	}
// 	return *this;
// }

// ----Destructor
ConnectionHandler::~ConnectionHandler() {
	if (DEBUG) {
		std::cout << "ConnectionHandler destructor called" << std::endl;
	}
	close(_clientFd);
	std::cout << "Client disconnected -> fd: " << _clientFd << std::endl;
}


// Getters
int ConnectionHandler::getFd() const {
	return _clientFd;
}


// Methods
bool ConnectionHandler::isAuthenticated() const {
	return _authenticated;
}

void ConnectionHandler::handleRead() {
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

		// Check for PASS command
		if (checkPassCommand(message)) {
			std::cout << "PASS command received" << std::endl;
			
		}

		// Send answer to client
		std::string response = "Message received\n";
		send(_clientFd, response.c_str(), response.size(), 0);
	}
}

// Function to check for the PASS command
bool ConnectionHandler::checkPassCommand(const std::string& message) {
	const std::string passCommand = "PASS";
	if (message.compare(0, passCommand.length(), passCommand) == 0) {
		return true;
	}
	return false;
}