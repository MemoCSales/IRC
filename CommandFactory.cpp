# include "CommandFactory.hpp"

CommandFactory::CommandFactory() {
	commands["CAP"] = new CommandCreatorImpl<Command>(CAP);
	commands["PASS"] = new CommandCreatorImpl<Command>(PASS);
	commands["NICK"] = new CommandCreatorImpl<Command>(NICK);
	commands["USER"] = new CommandCreatorImpl<Command>(USER);
	
}

CommandFactory::~CommandFactory() {
	for (std::map<std::string, CommandCreator*>::iterator it = commands.begin(); it != commands.end(); it++)
	{
		delete it->second;
	}
	
}

CommandPtr CommandFactory::createCommand(const std::string& commandName) {
	// todo: move commands map here instead
	std::map<std::string, CommandCreator*>::iterator it = commands.find(commandName);
	if (it != commands.end()) {
		return (*(it->second))();
	}
	return NULL;
}
