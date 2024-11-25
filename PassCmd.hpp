#pragma once
# include "ICommand.hpp"

class PassCommand : public ICommand {
	public:
		void execute(Client& client, const std::string& args);
};