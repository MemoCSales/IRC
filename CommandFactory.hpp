#pragma once
# include <map>
# include <memory>
# include "Commands.hpp"


typedef ICommand* CommandPtr;

class CommandFactory {
	private:
		typedef CommandPtr (*CommandCreator)();
		std::map<std::string, CommandCreator> commands;
	public:
		CommandFactory();
		CommandPtr createCommand(const std::string& commandName);
	private:
		// static = we can call this function without creating an instance of the CommandFactory class
		// private 'cause I only want to create command obj through the createCommand
		static CommandPtr createPassCommand(); // manage manual memory management. delete after no longer nedded
		static CommandPtr createNickCommand();
		static CommandPtr createUserCommand();
};
