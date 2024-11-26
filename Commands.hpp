#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include "ICommand.hpp"

//todo: make less classes

class PassCommand : public ICommand {
	public:
		void execute(Client& client, const std::string& args);
};

class NickCommand : public ICommand {
	public:
		void execute(Client& client, const std::string& args);
};

class UserCommand : public ICommand {
	public:
		void execute(Client& client, const std::string& args);
};

#endif