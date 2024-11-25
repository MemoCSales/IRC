# include <sys/socket.h>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>

int main() {
	// Creating socket
	int clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocketFd < 0) {
		std::cerr << "Error creating client socket." << std::endl;
		return 1;
	}
	std::cout << "Client socket created successfully" << std::endl;

	// Set Socket options
	int opt = 1;
	if (setsockopt(clientSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Error in socket config." << std::endl;
		close(clientSocketFd);
		return 1;
	}

	// Assign socket to port (bind)
	struct sockaddr_in serverAddress;
	std::memset(&serverAddress, 0, sizeof(serverAddress)); 	//Struct cleanup

	serverAddress.sin_family = AF_INET;				// IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY;		// Listen to all network interfaces
	// serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
	serverAddress.sin_port = htons(8080);				// Port (converted to network byte order)

	// Sending connection request
	if (connect(clientSocketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Error connecting client to server." << std::endl;
		close(clientSocketFd);
		return 1;
	}

	std::cout << "Connected to the server!" << std::endl;

	while (true) {
		// Get user input
		std::string message;
		std::cout << "Enter message (or type 'exit' to quit): ";
		std::getline(std::cin, message);

		// Exit condition
		if (message == "exit") {
			std::cout << "Closing connection to the server..." << std::endl;
			break;
		}

		// Send the message to the server
		if (send(clientSocketFd, message.c_str(), message.size(), 0) < 0) {
			std::cerr << "Error sending message to server." << std::endl;
			break;
		}


		// Receiving data from server
		char buffer[1024];
		memset(buffer, 0, 1024);
		int bytesReceived = recv(clientSocketFd, buffer, 1024, 0);
		if (bytesReceived < 0) {
			std::cerr << "Error receiving data from server" << std::endl;
			break;
		}
		
		std::cout << "Server's message:" << buffer << std::endl;
	}

	// Closing socket
	close(clientSocketFd);

	return 0;
}