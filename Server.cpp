# include "Server.hpp"

void initializeIrcErrorMessages() {
	ircErrorMessages[464] = "Password incorrect";
}

void sendError(int clientFd, int errorCode, const std::string& clientNick) {
	std::string response = clientNick + " : " + std::to_string(errorCode) + " " +
							" " + ircErrorMessages[errorCode] + "\r\n";
	send(clientFd, response.c_str(), response.size(), 0);
}

// Function to check for the PASS command
bool checkPassCommand(const std::string& message) {
	const std::string passCommand = "PASS";
	if (message.compare(0, passCommand.length(), passCommand) == 0) {
		return true;
	}
	return false;
}

int setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << "fcntl(F_GETFL)" << std::endl;
		return -1;
	}

	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1) {
		std::cerr << "fcntl(F_SETFL)" << std::endl;
		return -1;
	}

	return 0;
}

int main() {

	initializeIrcErrorMessages();
	// Create socket
	int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::cerr << "Error creating server socket." << std::endl;
		return 1;
	}
	std::cout << "Socket created successfully" << std::endl;

	// Set socket options
	int opt = 1;
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Error in socket options" << std::endl;
		close(serverFd);
		return 1;
	}
	
	setNonBlocking(serverFd);

	// Assign socket to port (bind)
	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address));

	address.sin_family = AF_INET;
	// address.sin_addr.s_addr = INADDR_ANY;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(IRCPORT);

	if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Error to find socket to port." << std::endl;
		close(serverFd);
		return 1;
	}

	// Listening to the assigned socket
	if (listen(serverFd, 5) < 0) {
		std::cerr << "Error in listening to the assigned socket" << std::endl;
		close(serverFd);
		return 1;
	}

	std::cout << "Server listening from port " << IRCPORT << "..." << std::endl;


	// Creating a list of sockets for poll()
	std::vector<struct pollfd> pollFds;

	//Add server socket to vector
	struct pollfd serverPollFd = {serverFd, POLLIN, 0};
	pollFds.push_back(serverPollFd);

	//main while loop
	while (true) {
		// Call poll to detect events
		int pollCount = poll(pollFds.data(), pollFds.size(), -1);	// Waits undefinedtly haha
		if (pollCount < 0) {
			std::cerr << "Error in poll()" << std::endl;
			break;
		}

		// Loop through sockets to process events
		for (size_t i = 0; i < pollFds.size(); i++) {
			if (pollFds[i].revents & POLLIN) {
				// If server socket -> accept new connection
				if (pollFds[i].fd == serverFd) {
					int clientFd = accept(serverFd, NULL, NULL);
					if (clientFd < 0) {
						perror("Error to accept connection");
						continue;
					}

					// Configure new non blocking socket
					setNonBlocking(clientFd);

					// Add new client to poll() list
					struct pollfd clientPollFd = {clientFd, POLLIN, 0};
					pollFds.push_back(clientPollFd);

					std::cout << "New client connected -> fd: " << clientFd << std::endl;
				}
				// If client -> read data
				else {
					char buffer[1024] = {0};
					int bytesReceived = recv(pollFds[i].fd, buffer, sizeof(buffer), 0);
					if (bytesReceived == 0) {
						std::cout << "Client desconnected -> fd: " << pollFds[i].fd << std::endl;
						close(pollFds[i].fd);
						pollFds.erase(pollFds.begin() + i);
						--i;
					} else if (bytesReceived < 0) {
						if (errno != EAGAIN && errno != EWOULDBLOCK) {
							// Real error, close connection
							perror("Error on recv");
							close(pollFds[i].fd);
							pollFds.erase(pollFds.begin() + i);
							--i;
						}
					} else {
						// Process message
						std::string message(buffer, bytesReceived);
						std::cout << "Received message: " << message << std::endl;

						// Check for PASS command
						if (checkPassCommand(message)) {
							std::cout << "PASS command received" << std::endl;
							
						}

						// Send answer to client
						std::string response = "Message received\n";
						send(pollFds[i].fd, response.c_str(), response.size(), 0);
					}
				}
			}
		}
	}

	close(serverFd);

	return 0;
}