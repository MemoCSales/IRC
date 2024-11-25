# Allowed functions

## socket, setsockupt,

Socket programming refers to the method of communication between 2 sockets on the network using a C++ program.

With the socket API we create a connection between the 2 programs running on the network, one of them receives the data by listening to the particular address port, and the other sends the data.

<aside>
💡 Sockets can be viewed as the endpoint of the 2-way communication between the 2 programming in the network.

</aside>

### Types of Sockets

- Stream Sockets: Stream sockets (like TCP) are like making a reliable phone call, ensuring that all information is delivered correctly
- Datagram Sockets: (like UDP) are more liek sending letters, they’re faster but might get lost

## Server Stages

### Creating the Server Socket

We create socket by using the socket() system call. It is defined inside the <sys/socket.h> header file

```cpp
int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
```

- serverSocketFd: It is the file descriptor for the socket
- AF_INET: It specifies the IPv4 protocol family
- SOCK_STREAM: It defines the stream socket (TCP)
- 0: Uses the predefine protocol (TCP for SOCK_STREAM)

### Socket config

```cpp
int opt = 1;
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
	std::cerr << "Error to configure socket options" << std::endl;
	close(server_fd);
	return 1;
}
```

- SOL_SOCKET: Specifies the socket level at which the option is to be set. In this case, it’s the socket layer itself.
- SO_REUSEADDR: The specific socket option being set. This option allows the socket to be bound to an address that is already in use by another socket. This is useful for server applications that need to quickly restart after crash.
- &opt: A pointer to the option value. Here. the address for the opt variable is passed, which contains the value 1.

The primary goal is to configure the socket to allow address reuse. This is typically done in server applications to prevent the “Address already in use” error, which can occur when a server process is restarted quickly. By setting the SO_REUSEADDR option, the operating system allows the socket to bind to the same address as a previously used socket, as long as the previous socket has been closed.

### Assign socket to a port (bind)

```cpp
struct sockaddr_in address;
std::memset(&address, 0, sizeof(address)); // Limpia la estructura

address.sin_family = AF_INET;            // IPv4
address.sin_addr.s_addr = INADDR_ANY;    // Escucha en todas las interfaces de red
address.sin_port = htons(8080);          // Puerto (convertido a orden de bytes de red)

if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    std::cerr << "Error al enlazar el socket al puerto" << std::endl;
    close(server_fd);
    return 1;
}
```

- sockaddr_in: It is the data type that is used to store the address of the socket
- htons(): This function is used to convert the unsigned int from machine byte order to network byte order
- INADDR_ANY: It is used when we don’t want to bind our socket to any particular IP and instead make it listen to all the available IPs

### Listen for connections

```cpp
if (listen(server_fd, 5) < 0) {
    std::cerr << "Error al configurar el socket para escuchar" << std::endl;
    close(server_fd);
    return 1;
}
std::cout << "Servidor escuchando en el puerto 8080..." << std::endl;

```

- 5: is the max number of connections in the queue.

### Accept connections

```cpp
int addrlen = sizeof(address);
int client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
if (client_fd < 0) {
    std::cerr << "Error al aceptar la conexión" << std::endl;
    close(server_fd);
    return 1;
}
std::cout << "Cliente conectado!" << std::endl;

```
- addrlen: Declares an integer variable addrlen and initializes it with the size of the address struct. This variable will be used to store the size of the client address info.
- client_fd: This is a system call used to accept a new incoming connection on the listening socket server_fd.
    - server_fd: The fd of the listening socket
    - (struct sockaddr*)&address: A pointer to the address structure, which will be filled with the client’s address info
    - (socklen_t*)&addrlen: A pointer to the addrlen variable, which will be updated with the actual size of the client address info.

If successful the accept call extracts the client’s address info and creates a new socket for communication with the client.

### Sending and receiving data

Sending data:

```cpp
std::string message = "Hola desde el servidor!\n";
send(client_fd, message.c_str(), message.size(), 0);

```

Receiving data:

```cpp
char buffer[1024] = {0};
int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
if (bytes_received > 0) {
    std::cout << "Mensaje recibido: " << buffer << std::endl;
}

```

### getsockname

You use the `getsockname` function to retrieve the local address and port number assigned to a socket. This is particularly useful in the following scenarios:

**1. Implicit Binding:**

- When you call `connect()` without explicitly calling `bind()` first, the system automatically assigns a port number to the socket.
- `getsockname()` allows you to determine this assigned port number.

**2. Multiple Interfaces and IP Addresses:**

- If your system has multiple network interfaces or IP addresses, `getsockname()` helps you identify the specific interface and IP address used for the connection.

**3. Debugging and Logging:**

- You can use the retrieved information for debugging purposes, logging, or monitoring network activity.

**4. Server Applications:**

- In server applications, `getsockname()` can be useful for determining the local address and port number that the server is listening on. This information can be helpful for configuration and troubleshooting.

### poll()

poll() allows you to monitor multiple sockets to detect events like:

- New connections
- Ready to read data
- Availability to send data

<aside>
💡

This is ideal for a server that needs to deal with multiple clients at the same time

</aside>

### poll() structure

```cpp
#include <poll.h>

struct pollfd {
	int fd;        // File descriptor (socket)
	short events;  // Events to monitor (POLLIN, POLLOUT, etc)
	short revents; // Events that actually happened
};
```

### Step by step to use poll()

1. Create a list of sockets to monitor:
    1. Include server socket to accept connections
    2. Add already connected client sockets
2. Configure events:
    1. POLLIN: Read data (includes new connections for the server socket)
    2. POLLOUT: Read data
    3. POLLERR: Error detection
3. Call poll() to wait for events:
    1. poll() blocks (or will wait for a time) until an event occur in any of the sockets
4. Process the detected events:
    1. If server socket has POLLIN → accepts a new connection
    2. if client socket has POLLIN → read the data sent
    3. if the client socket has errors → close the connection

### fcntl function

It is used to manipulate file descriptors

fcntl can be use to set sockets to non-blocking mode, enabling asynchronous I/O operations. This is important for handling a large number of clients efficiently.

- **File Descriptor Flags:**
    - **O_NONBLOCK:** Setting this flag on a socket's file descriptor can make I/O operations non-blocking, allowing the server to handle multiple clients without blocking.
        - This flags indicates that operations like read(), write(), and accept() on this socket will not block.
        
        <aside>
        💡
        
        Instead, they will return immediately if no data is available, allowing the server to handle other sockets
        
        </aside>
        
    - **O_ASYNC:** This flag can be used to trigger asynchronous I/O events, allowing the server to efficiently handle incoming data and outgoing messages.
- **File Descriptor Control:**
    - `fcntl` can be used to control various aspects of file descriptors, such as setting close-on-exec flags or obtaining file status information. These operations can be useful for managing file descriptors associated with client connections and other resources.

    