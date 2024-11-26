#pragma once
# include "ICommand.hpp"

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

