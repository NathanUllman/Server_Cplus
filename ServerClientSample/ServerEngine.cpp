#include "pch.h"
#include "Server.h"


User ServerEngine::AddConnection(SOCKET ListenSocket,fd_set* master) {
	User newUser;
	SOCKET newClient = AcceptClientSocket(ListenSocket, &newUser);

	// add connection
	FD_SET(newClient, master);

	newUser.socket = newClient;
	printf("New Connection : %s", newUser.host.c_str());
	send(newClient, "Enter Password: ", 15, 0);

	return newUser;

}

void ServerEngine::DropClient(SOCKET sock, fd_set* master) {
	closesocket(sock);
	FD_CLR(sock, master);
}

SOCKET  ServerEngine::AcceptClientSocket(SOCKET ListenSocket, User *user) {

	SOCKET ClientSocket = INVALID_SOCKET;

	struct sockaddr client;
	int clientSize = sizeof(client);

	// prep for client info
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);


	// Accept a client socket
	ClientSocket = accept(ListenSocket, (sockaddr*)&client, &clientSize);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		throw 1;
	}

	// get info of the client
	if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		*user = User(host, service);
	}
	else {
		printf("Could not get client info");
	}

	// No longer need server socket	
	//closesocket(ListenSocket); not used when we are doing muli
	return ClientSocket;

}
