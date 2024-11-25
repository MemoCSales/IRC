#pragma once
# include "Client.hpp"

class ICommand {
	public:
		virtual ~ICommand() {};
		virtual void execute(Client& client, const std::string& args) = 0;
};
