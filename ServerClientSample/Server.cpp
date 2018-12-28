#include "pch.h"
#include "Server.h"
void test(SOCKET test) {

}
Server::Server( PCSTR  port, IClientManager* IClientManager) {
	InitializeWinSock();
 	_ListeningSocket = ListeningSocket(port);

	_ClientManager = IClientManager;

	//void(*foo)(SOCKET);
	//foo = &test;

	//Server_Toolbox* yeah;
	//yeah->RemoveClient = foo;


}



bool Server::Init(void)
{
	_ListeningSocket.Bind_Lsock();
	_ListeningSocket.Listen_Lsock();

	return TRUE;
}

void Server::Run(void)
{
	SOCKET Lsock = _ListeningSocket.Lsock;
	AcceptMultipleClients(Lsock);
}

void Server::Close(void)
{
	_ListeningSocket.Shutdown_Lsock();
}

void __cdecl Server::InitializeWinSock() {
	int iResult;
	WORD ver = MAKEWORD(2, 2);
	iResult = WSAStartup(ver, wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		throw 1;
	}
}


void Server::AcceptMultipleClients(SOCKET ListeningSocket)
{
	fd_set master;
	FD_ZERO(&master);
	FD_SET(ListeningSocket, &master);
	printf("Listening on Port 5400...\r\n");

	while (true) {
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr); // sockets end up in copy

		for (int i = 0; i < socketCount; i++) { // cycle through all sockets that 'raised an event' or were 'updated"
			SOCKET updatedSocket = copy.fd_array[i];
			if (updatedSocket == ListeningSocket) { // if listening socket 'raised event' we have a new connection	
				User newUser = _ServerEngine.AddConnection(ListeningSocket,&master);
				_ClientManager->ClientAdded(newUser);
			}
			else { // accept a new message
				char message[4096];
				ZeroMemory(message, 4096);
				int bytesIn = recv(updatedSocket, message, 4096, 0); // get message

				if (bytesIn <= 0) { //Client Left
					_ServerEngine.DropClient(updatedSocket, &master);
					_ClientManager->ClientRemoved(updatedSocket);
				}
				else {
					_ClientManager->MessageRecieved(updatedSocket, message);
				}
			}
		}
	}
}



//void _cdecl Server::CommunicateWithClient() {
//	int iResult;
//	int iSendResult;
//	char recvbuf[4096];
//
//	// Receive until the peer shuts down the connection
//	do {
//
//		iResult = recv(ClientSocket, recvbuf, 4096, 0);
//		if (iResult > 0) {
//			printf("Bytes received: %d\n", iResult);
//
//			// Echo the buffer back to the sender
//			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
//			if (iSendResult == SOCKET_ERROR) {
//				printf("send failed with error: %d\n", WSAGetLastError());
//				closesocket(ClientSocket);
//				WSACleanup();
//				throw 1;
//			}
//			printf("Bytes sent: %d\n", iSendResult);
//		}
//		else if (iResult == 0)
//			printf("Connection closing...\n");
//		else {
//			printf("recv failed with error: %d\n", WSAGetLastError());
//			closesocket(ClientSocket);
//			WSACleanup();
//			throw 1;
//		}
//
//	} while (iResult > 0);
//
//}
