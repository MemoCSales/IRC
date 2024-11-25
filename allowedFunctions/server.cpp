# include <sys/socket.h>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>

int main() {

	// Creating socket
	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd < 0) {
		std::cerr << "Error creating server socket." << std::endl;
		return 1;
	}
	std::cout << "Socket created successfully" << std::endl;


	// Socket config
	int opt = 1;
	if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Error in socket config" << std::endl;
		close(serverSocketFd);
		return 1;
	}

	// Assign socket to port (bind)
	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address)); 	//Struct cleanup

	address.sin_family = AF_INET;				// IPv4
	address.sin_addr.s_addr = INADDR_ANY;		// Listen to all network interfaces
	address.sin_port = htons(8080);				// Port (converted to network byte order)

	if (bind(serverSocketFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::cerr << "Error to bind socket to port" << std::endl;
		close(serverSocketFd);
		return 1;
	}

	// Listening to the assigned socket
	if (listen(serverSocketFd, 5) < 0) {
		std::cerr << "Error in listening to the assigned socket" << std::endl;
		close(serverSocketFd);
		return 1;
	}

	std::cout << "Server listening from port 8080..." << std::endl;


	// Accept connections
	int addrLen = sizeof(address);
	// Get the local address and port number
	socklen_t addr_len;
	if (getsockname(serverSocketFd, (struct sockaddr *)&address, &addr_len) < 0) {
		std::cerr << "Error in getsockname." << std::endl;
		close(serverSocketFd);
		return 1;
	}
	std::cout << "Local IP address: " << inet_ntoa(address.sin_addr) << std::endl;
	std::cout << "Local port number: " << ntohs(address.sin_port) << std::endl;


	int clientFd = accept(serverSocketFd, (struct sockaddr*)&address, (socklen_t*)&addrLen);

	if (clientFd < 0) {
		std::cerr << "Error accepting connection." << std::endl;
		close(serverSocketFd);
		return 1;
	}
	std::cout << "Client Connected!" << std::endl;


	// Receiving data
	char buffer[1024] = {0};
	int bytesReceived = recv(clientFd, buffer, sizeof(buffer), 0);

	if (bytesReceived > 0) {
		std::cout << "Message from client: " << buffer << std::endl;
	}


	close(serverSocketFd);
	return 0;
}
