#pragma once
# include "Client.hpp"

class ICommands {
	public:
		virtual ~ICommands() {};
		virtual void execute(Client& client, const std::string& args) = 0;
};
