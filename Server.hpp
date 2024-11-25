#pragma once

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <cerrno>
# include <cstdio>
# include <sstream>
// Sockets
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include <fcntl.h>
// Container libraries
# include <vector>
# include <map>

# define IRCPORT 6667
# define PASS "42"

std::map<int, std::string> ircErrorMessages;

void initializeIrcErrorMessages();
void sendError(int clientFd, int errorCode, const std::string& clientNick);