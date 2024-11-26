# include "CommandFactory.hpp"

CommandFactory::CommandFactory() {
	commands["PASS"] = &createPassCommand;
	commands["NICK"] = &createNickCommand;
	commands["USER"] = &createUserCommand;
	
}

CommandFactory::~CommandFactory() {}

CommandPtr CommandFactory::createCommand(const std::string& commandName) {
	// todo: move commands map here instead
	std::map<std::string, CommandCreator>::iterator it = commands.find(commandName);
	if (it != commands.end()) {
		return it->second();
	}
	return NULL;
}

CommandPtr CommandFactory::createPassCommand() {
	return new PassCommand();
}

CommandPtr CommandFactory::createNickCommand() {
	return new NickCommand();
}

CommandPtr CommandFactory::createUserCommand() {
	return new UserCommand();
}
