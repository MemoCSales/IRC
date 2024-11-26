#pragma once

# include "Server.hpp"

/* ERROR MESSAGES */
# define ERR_PASSWDMISMATCH		 ":server 464 * :Password incorrect\r\n"
# define ERR_NICKNAMEINUSE(nick) ":server 433 * :" + nick + ":Nickname is already in use\r\n"


/* REPLY MESSAGES */
# define RPL_NICKCHANGE(oldNick, user, host, newNick)  ":" + oldNick + "!" + user + "@" + host + " NICK :" + newNick + "\r\n" 