#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <string>
# include <sstream>
# include <unistd.h>
# include <map>

# include "Server.hpp"
# include "CommandParser.hpp"
# define DEBUG 0
class Utils;

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
		std::string getCorrectPassword() const;

		// Setters
		void setNick(const std::string& nick);
		void setUser(const std::string& user);
		void setAuthenticated(bool);

		// Methods
		bool isAuthenticated() const;
		void handleRead();
	
	private:
		bool checkPassCommand(const std::string& message);
};

extern std::map<int, Client*> connections;

#endif
