#include "pch.h"
#include "Server.h"


ListeningSocket::ListeningSocket() {

}

ListeningSocket::ListeningSocket(	PCSTR port,int ai_family, int ai_socktype, int ai_protocol, int ai_flags ) 
{
	//create hints for args
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = ai_family; // ip4
	hints.ai_socktype = ai_socktype;
	hints.ai_protocol = ai_protocol;
	hints.ai_flags = ai_flags;

	//set port
	Lsock_port = port;

	//get and set port addr info 
	GetAndSetAddressInfo_Lsock();

	// create Lsock
	Create_Lsock();
}

 void ListeningSocket::GetAndSetAddressInfo_Lsock() {
	int iResult;
	iResult = getaddrinfo(NULL, Lsock_port, &hints, &Lsock_addrinfo);

#pragma region Error Handling
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		throw 1;
	}
#pragma endregion
}

void  ListeningSocket::Create_Lsock(){
	Lsock = socket(Lsock_addrinfo->ai_family, Lsock_addrinfo->ai_socktype, Lsock_addrinfo->ai_protocol);

#pragma region Error Handling
	if (Lsock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(Lsock_addrinfo);
		WSACleanup();
		throw 1;
	}
#pragma endregion	
}

void ListeningSocket::Bind_Lsock(){
	int iResult;
	iResult = bind(Lsock, Lsock_addrinfo->ai_addr, (int)Lsock_addrinfo->ai_addrlen);

#pragma region Error Handling
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(Lsock_addrinfo);
		closesocket(Lsock);
		WSACleanup();
		throw 1;
	}
	//freeaddrinfo(Lsock_addrinfo); keep it around just in case
#pragma endregion
}

void ListeningSocket::Listen_Lsock() {
	int iResult;
	iResult = listen(Lsock, SOMAXCONN);

#pragma region Error Handling
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(Lsock);
		WSACleanup();
		throw 1;
	}
#pragma endregion
}

void ListeningSocket::Shutdown_Lsock() {
	int iResult;
	iResult = shutdown(Lsock, SD_SEND);

#pragma region Error Handling

	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(Lsock);
		WSACleanup();
		throw 1;
	}

#pragma endregion	
}