#pragma once
# include "ICommands.hpp"

class PassCommand : public ICommands {
	public:
		void execute(Client& client, const std::string& args);
};

class NickCommand : public ICommands {
	public:
		void execute(Client& client, const std::string& args);
};

class UserCommand : public ICommands {
	public:
		void execute(Client& client, const std::string& args);
};

