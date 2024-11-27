# include "Commands.hpp"
# include "NumericMessages.hpp"
# include "Utils.hpp"


void Command::execute(Client& client, const std::string& args) {
	switch (_type)
	{
	case CAP:
		std::cout << "CAP" << std::endl;
		break;
	case PASS:
		{
			std::string password = Utils::trim(args);
			if (password == client.getCorrectPassword()) {
				client.setAuthenticated(true);
				std::cout << "Client authenticated -> fd: " << client.getFd() << std::endl;
			} else {
				std::string response = ERR_PASSWDMISMATCH;
				// std::cout << response << std::endl;	
				sendErrorAndCloseFd(client.getFd(), response);
			}
		}
		break;
	case NICK:
		{
			std::string newNick = Utils::trim(args);
			std::string oldNick = client.getNick();
			bool nickExists = false;

			//todo: do the check for a valid newNickname : ERR_ERRONEUSNICKNAME 432

			std::map<int, Client*>::iterator it = connections.begin();
			for (; it != connections.end(); it++) {
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
				sendReplyOrError(client.getFd(), response);

				// todo: Inform other clients about the nickname change
			}
			std::cout << "NICK command received. Client nickname changed from: " << oldNick << " to: " << newNick << std::endl;
		}
		break;
	case USER:
		{
			//todo: extend user verification
			// USERLEN and so on
			std::string newUser = Utils::trim(args);
			client.setUser(newUser);
		}
		break;
	}
}

// // CAP Command
// void CapCommand::execute(Client& client, const std::string& args) {
	
// }

// // PASS Command
// void PassCommand::execute(Client& client, const std::string& args) {
// 	std::string password = Utils::trim(args);
// 	if (password == client.getCorrectPassword()) {
// 		client.setAuthenticated(true);
// 		std::cout << "Client authenticated -> fd: " << client.getFd() << std::endl;
// 	} else {
// 		std::string response = ERR_PASSWDMISMATCH;
// 		// std::cout << response << std::endl;	
// 		sendErrorAndCloseFd(client.getFd(), response);
// 	}
// }


// // NICK Command
// void NickCommand::execute(Client& client, const std::string& args) {
// 	std::string newNick = Utils::trim(args);
// 	std::string oldNick = client.getNick();
// 	bool nickExists = false;

// 	//todo: do the check for a valid newNickname : ERR_ERRONEUSNICKNAME 432

// 	std::map<int, Client*>::iterator it = connections.begin();
// 	for (; it != connections.end(); it++) {
// 		if (it->second->getNick() == newNick) {
// 			nickExists = true;
// 			break;
// 		}
// 	}
// 	if (nickExists) {
// 		std::string response = ERR_NICKNAMEINUSE(newNick);
// 		sendErrorAndCloseFd(client.getFd(), response);
// 	} else {
// 		client.setNick(newNick);
// 		std::string user = client.getUser();
// 		std::string host = "localhost";

// 		// Acknowledge the NICK command was successfull and print the new nick
// 		std::string response = RPL_NICKCHANGE(oldNick, user, host, newNick);
// 		sendReplyOrError(client.getFd(), response);

// 		// todo: Inform other clients about the nickname change
// 	}
// 	std::cout << "NICK command received. Client nickname changed from: " << oldNick << " to: " << newNick << std::endl;
// }


// void UserCommand::execute(Client& client, const std::string& args) {
// 	//todo: extend user verification
// 	// USERLEN and so on
// 	std::string newUser = Utils::trim(args);
// 	client.setUser(newUser);
// }
