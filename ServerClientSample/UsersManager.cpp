#include "pch.h"
#include "Server.h"


void UserManager::ClientAdded(User newUser) {
	AddUser(newUser);
}

void UserManager::ClientRemoved(SOCKET removedSocket) {
	RemoveUser_BySocket(removedSocket);
}

void UserManager::MessageRecieved(SOCKET userSocket, std::string message) {
	std::string password = "secret\r\n";
	User* user = GetCorrespondingUser(userSocket);

	if (!user->isLoggedIn) {
		if (message == password) {
			user->isLoggedIn = true;
			std::string wrongPasswordMessage = "CORRECT";
			send(user->socket, wrongPasswordMessage.c_str(), wrongPasswordMessage.size() + 1, 0);
		}
		else {
			std::string wrongPasswordMessage = "Wrong Password, Try Again: ";
			send(user->socket, wrongPasswordMessage.c_str(), wrongPasswordMessage.size() + 1, 0);
		}
	}

}


User* UserManager::GetCorrespondingUser(SOCKET sock)
{
	for (int i = 0; i < CurrentUsers.size(); i++) {
		if (sock == CurrentUsers.at(i).socket) {
			return &CurrentUsers.at(i); 
		}
	}
	throw 1;
}

bool UserManager::AddUser(User newUser)
{
	CurrentUsers.insert(CurrentUsers.end(), newUser);
	return true;
}

bool UserManager::RemoveUser(User userToRemove)
{
	for (int i = 0; i < CurrentUsers.size(); i++) {
		if (userToRemove.socket == CurrentUsers.at(i).socket) { 
			//todo: is it the best to remove by a socket?
			CurrentUsers.erase(CurrentUsers.begin() + i);
			return true;
		}
	}
	return false;
}

bool UserManager::RemoveUser_BySocket(SOCKET socket) {
	for (int i = 0; i < CurrentUsers.size(); i++) {
		if (socket == CurrentUsers.at(i).socket) {
			CurrentUsers.erase(CurrentUsers.begin() + i);
			return true;
		}
	}
	return false;
}

