# include "PassCmd.hpp"

void PassCommand::execute(Client& client, const std::string& args) {
	std::string password = args;
	if (password == client.getCorrectPassword()) {
		client.setAuthenticated(true);
		std::cout << "Client authenticated -> fd: " << client.getFd() << std::endl;
	} else {
		std::string response = ircErrorMessages[464];
		std::cout << response << std::endl;	
		send(client.getFd(), response.c_str(), response.size(), 0);
		close(client.getFd());
	}
}