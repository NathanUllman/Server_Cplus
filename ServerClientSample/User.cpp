#include "pch.h"
#include "Server.h"
// should we do ID's to dif users?

User::User( std::string h ,  std::string s )
{
	host = h;
	service = s;
	isLoggedIn = false;
}

User::User()
{
}

void User::SetSocket(SOCKET s) {
	socket = s;
}