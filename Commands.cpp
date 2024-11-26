# include "Commands.hpp"
# include "NumericMessages.hpp"

// PASS Command
void PassCommand::execute(Client& client, const std::string& args) {
	std::string password = trim(args);
	if (password == client.getCorrectPassword()) {
		client.setAuthenticated(true);
		std::cout << "Client authenticated -> fd: " << client.getFd() << std::endl;
	} else {
		std::string response = ERR_PASSWDMISMATCH;
		// std::cout << response << std::endl;	
		sendErrorAndCloseFd(client.getFd(), response);
	}
}


// NICK Command

void NickCommand::execute(Client& client, const std::string& args) {
	std::string newNick = trim(args);
	std::string oldNick = client.getNick();
	bool nickExists = false;

	//todo: do the check for a valid newNickname : ERR_ERRONEUSNICKNAME 432

	std::map<int, Client*>::iterator it = connections.begin();
	for (it; it != connections.end(); it++) {
		if (it->second->getNick() == newNick) {
			nickExists = true;
			break;
		}
	}
	if (nickExists) {
		std::string response = ERR_NICKNAMEINUSE(newNick);
		sendErrorAndCloseFd(client.getFd(), response);
	} else {
		client.setNick(newNick);
		std::string user = client.getUser();
		std::string host = "localhost";

		// Acknowledge the NICK command was successfull and print the new nick
		std::string response = RPL_NICKCHANGE(oldNick, user, host, newNick);
		sendReply(client.getFd(), response);

		// todo: Inform other clients about the nickname change
	}
	std::cout << "NICK command received. Client nickname changed from: " << oldNick << " to: " << newNick << std::endl;
}


void UserCommand::execute(Client& client, const std::string& args) {
	//todo: extend user verification
	// USERLEN and so on
	std::string newUser = trim(args);
	client.setUser(newUser);
}