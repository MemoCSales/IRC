# include "CommandParser.hpp"
# include "CommandFactory.hpp"
# include "NumericMessages.hpp"
# include "Utils.hpp"

CommandParser::CommandParser() {
	commandFactory = new CommandFactory();
}

CommandParser::~CommandParser() {
	delete commandFactory;
}

void CommandParser::parseAndExecute(Client& client, const std::string& message) {
	std::istringstream stream(message);
	std::string commandName;
	std::string args;

	stream >> commandName >> args;
	std::cout << "cmdName: " << commandName << std::endl;
	std::cout << "Args" << args << std::endl;
	std::cout << "Args in ascii: ";
	Utils::printAsciiDecimal(args);
	std::getline(stream, args);
	args = Utils::trim(args);

	CommandPtr command = commandFactory->createCommand(commandName);
	if (command) {
		command->execute(client, args);
		delete command;
	} else {
		std::string response = ERR_UNKNOWNCOMMAND(commandName);
		sendReplyOrError(client.getFd(), response);
	}
}