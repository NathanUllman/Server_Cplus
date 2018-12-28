
#include "pch.h"
#include "Server_Functions.h"



#define DEFAULT_PORT "5400"
#define DEFAULT_BUFLEN 4096
int iResult;

void __cdecl InitializeWinSock(WSADATA *wsaData) {
	
	WORD ver = MAKEWORD(2, 2);
	iResult = WSAStartup(ver, wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		throw 1;
	}
}


addrinfo CreateHints(void) {
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	return hints;
}

addrinfo* _cdecl GetAddr() {
	struct addrinfo *addrInfo;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &CreateHints(), &addrInfo);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		throw 1;
	}
	return addrInfo;
}

SOCKET _cdecl CreateSocket(addrinfo *result) {
	SOCKET ListenSocket= INVALID_SOCKET;

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket ==  INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw 1;
	}
	return ListenSocket;
}

void _cdecl BindSocket(SOCKET ListenSocket, addrinfo *result) {
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		throw 1;
	}
	freeaddrinfo(result);
}

void _cdecl ListenToServerSocket(SOCKET ListenSocket) {

	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		throw 1;
	}
}

SOCKET _cdecl AcceptClientSocket(SOCKET ListenSocket) {

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
		closesocket(ListenSocket);
		WSACleanup();
		throw 1;
	}

	// get info of the client
	if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV,0) == 0) {
		printf(host);
		printf(service);
	}
	else {
		printf("Could not get client info");
	}

	// No longer need server socket	
	closesocket(ListenSocket);
	return ClientSocket;

}

void _cdecl CommunicateWithClient(SOCKET ClientSocket) {
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				throw 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			throw 1;
		}

	} while (iResult > 0);

}

void _cdecl ShutdownSocket(SOCKET ClientSocket) {

	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		throw 1;
	}
}