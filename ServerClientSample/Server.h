#pragma once

#include "pch.h"

#pragma region User

class User
{

public:
	User(std::string h, std::string c);
	User();
	void SetSocket(SOCKET);
	SOCKET socket;

	std::string host;
	std::string service;
	bool isLoggedIn;
};

#pragma endregion

class Server_Toolbox { // tools that a implemented client manager can use to access the server
public:
	virtual void SendMessageToClient(SOCKET socket) = 0;
	virtual void RemoveClient(SOCKET socket) = 0;

};

#pragma region UserManager

class IClientManager {
public:
	virtual void ClientAdded(User newUser) = 0;
	virtual void ClientRemoved(SOCKET removedSocket) = 0;
	virtual void MessageRecieved(SOCKET sock, std::string message)=0;

};

class UserManager : public IClientManager{

public:
	void ClientAdded(User newUser);
	void ClientRemoved(SOCKET removedSocket);
	void MessageRecieved(SOCKET sock, std::string message);
private:
	std::vector<User> CurrentUsers;
	User* GetCorrespondingUser(SOCKET);
	bool AddUser(User);
	bool RemoveUser(User);
	bool RemoveUser_BySocket(SOCKET);

};

#pragma endregion

#pragma region ListeningSocket

class ListeningSocket {
public:
	ListeningSocket();
	ListeningSocket(PCSTR port,
		int ai_family = AF_INET,
		int ai_socktype = SOCK_STREAM,
		int ai_protocol = IPPROTO_TCP,
		int ai_flags = AI_PASSIVE);

	SOCKET Lsock;
	addrinfo* Lsock_addrinfo;
	PCSTR Lsock_port;


	void Bind_Lsock();
	void Listen_Lsock();
	void Shutdown_Lsock();

private:
	addrinfo hints;
	void GetAndSetAddressInfo_Lsock();
	void Create_Lsock(); // used in constructor
};

#pragma endregion

#pragma region IStateManager

class IStateManager {
public:
	virtual void MessageRecieved(User* user, std::string message) = 0;
private:

};

#pragma endregion

#pragma region ServerEngine

class ServerEngine {
public:
	User	AddConnection(SOCKET ListenSocket, fd_set* master);
	void	DropClient(SOCKET sock, fd_set* master);
private:
	SOCKET	AcceptClientSocket(SOCKET ListenSocket, User *user);
};

#pragma endregion

#pragma region Server

class Server {

public:
	Server(PCSTR port, IClientManager* IClientManager);
	bool Init(void);
	void Run(void);
	void Close(void);

	IClientManager*		_ClientManager;

private:
	void __cdecl InitializeWinSock();
	void AcceptMultipleClients(SOCKET ListeningSocket);

	ServerEngine		_ServerEngine;
	ListeningSocket		_ListeningSocket;
	WSADATA* wsaData = &WSADATA();

};
#pragma endregion









